#include "Spirit.h"
#include "VInput.h"
#include "Assets.h"
#include "Game.h"
#include "Constants.h"

#include <math.h>
#include <assert.h>

OrientedBoxCollider* Spirit::s_pSpiritCollider = 0;

Spirit::Spirit()
{
    // Attempt to create a shared collider
    // Will not make one if it already exists
    Spirit::CreateSharedCollider();

    m_pGame = Game::GetInstance();

    // Setup graphical component 
    m_matter.SetMesh(g_pSpiritAnimMesh);
    m_matter.SetLoopMode(Matter::LOOP_PING_PONG);
    m_matter.SetAnimation("Idle");
    m_matter.StartAnimation();

    m_matter.SetTexture(g_pSpirit1Tex);
    m_matter.SetMaterial(g_pDefaultMaterial);
    m_matter.SetPosition(15.0f, 12.0f, MIDDLEGROUND_Z);
    m_matter.SetScale(SPIRIT_SCALE, SPIRIT_SCALE, SPIRIT_SCALE);
    m_matter.AddCollider(s_pSpiritCollider);
    m_matter.SetPhysical(1);
    m_matter.SetMobile(1);
    m_matter.EnableColliderRendering();

    m_pGame->GetScene()->AddActor(&m_matter);

    // Attributes
    m_nPlayerIndex = 0;
    m_nPercent     = 0;
    m_nLives       = SPIRIT_STARTING_LIVES;
    m_nDamage      = SPIRIT_DEFAULT_DAMAGE;
    m_nJumpCharges = 0;

    m_arPosition[0] = 15.0f;
    m_arPosition[1] = 12.0f;
    m_arPosition[2] = MIDDLEGROUND_Z;

    m_nGrounded = 0;
    m_fSpeed = SPIRIT_DEFAULT_SPEED;
    m_fAcceleration = SPIRIT_DEFAULT_ACCELERATION;
    m_nJumpPressed = SPIRIT_JUMP_FRAME_WINDOW;
    m_nJustJumped = 0;

    m_nDirection = DIRECTION_RIGHT;

    m_fXVelocity = 0.0f;
    m_fYVelocity = 0.0f;

    m_nAnimState = ANIM_IDLE;

    m_fAttackTime = 0.0f;

    m_nAlive = 1;
    m_nAttacking = 0;
}

Spirit::~Spirit()
{

}

void Spirit::Update()
{
    Update_Kinematics();
    Update_Orientation();
    Update_Attack();
    Update_Animation();
}

void Spirit::Update_Kinematics()
{
    float fAccelX = 0.0f;
    float fDeltaTime = Game::GetInstance()->DeltaTime();

    // Reset the just jumped flag;
    m_nJustJumped = 0;

    // Get relevant input.
    if (GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex) < -DEAD_ZONE)
    {
        fAccelX = -m_fAcceleration;
    }
    else if (GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex) > DEAD_ZONE)
    {
        fAccelX = m_fAcceleration;
    }

    if (IsControllerButtonJustDown(VCONT_A, m_nPlayerIndex))
    {
        m_nJumpPressed = 0;
    }

    CheckJump();

    if (m_nGrounded == 0)
    {
        ApplyGravity();
    }

    if (fAccelX == 0.0f ||
        abs(m_fXVelocity) > m_fSpeed)
    {
        ApplyDrag();
    }

    if (abs(m_fXVelocity) <= m_fSpeed)
    {
        // If the xvelocity is smaller than this spirit's speed
        // We want to increase his xvelocity
        m_fXVelocity += fAccelX * fDeltaTime;

        // Clamp
        if (m_fXVelocity > m_fSpeed)
            m_fXVelocity = m_fSpeed;
        if (m_fXVelocity < -m_fSpeed)
            m_fXVelocity = -m_fSpeed;
    }

    // Translate in X
    int nHit = 0;
    nHit = m_matter.Translate(fDeltaTime * m_fXVelocity, 0.0f, 0.0f);

    // Bounce the player if going faster than allowed speed.
    if (nHit != 0 &&
        abs(m_fXVelocity) > m_fSpeed)
    {
        m_fXVelocity = -m_fXVelocity;
    }

    // Translate in Y if not grounded.
    if (m_nGrounded == 0)
    {
        nHit = 0;
        nHit = m_matter.Translate(0.0f, fDeltaTime * m_fYVelocity, 0.0f);

        if (nHit != 0)
        {
            if (m_fYVelocity < 0.0f)
            {
                m_nJumpCharges = 1;
            }

            m_fYVelocity = 0.0f;
        }
    }

    // Sync position with 
    const float* arPos = m_matter.GetPosition();
    m_arPosition[0] = arPos[0];
    m_arPosition[1] = arPos[1];
    m_arPosition[2] = arPos[2];

    CheckGrounded();
}

void Spirit::Update_Orientation()
{
    // Only update the orientation if not attacking.
    if (m_nAttacking == 0)
    {
        // First, just determined the direction
        if (GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex) < -DEAD_ZONE)
        {
            m_nDirection = DIRECTION_LEFT;
            m_matter.SetRotation(0.0f, -90.0f, 0.0f);
        }
        else if (GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex) > DEAD_ZONE)
        {
            m_nDirection = DIRECTION_RIGHT;
            m_matter.SetRotation(0.0f, 90.0f, 0.0f);
        }
    }
}

void Spirit::Update_Attack()
{
    if (m_nAttacking == 0)
    {
        // The character isn't attacking, but check to see if
        // they pressed X. If so, begin the attack.
        if (IsControllerButtonDown(VCONT_X, m_nPlayerIndex) &&
            HasControl() != 0)
        {
            m_nAttacking = 1;
            m_fAttackTime = 0.0f;
            m_attackVolume.ResetHitPlayers();
        }
    }
    
    if (m_nAttacking != 0)
    {
        m_fAttackTime += Game::GetInstance()->DeltaTime();

        m_attackVolume.SetElapsedTime(m_fAttackTime);

        // Set the attack volume's position based on this spirit's position.
        float arPos[3] = { m_arPosition[0], 
                           m_arPosition[1],
                           m_arPosition[2]};

        arPos[0] += (m_nDirection == DIRECTION_LEFT) ? -ATTACK_VOLUME_X_OFFSET : ATTACK_VOLUME_X_OFFSET;

        m_attackVolume.SetPosition(arPos[0], arPos[1], arPos[2]);

        // The attack will be resolved in the AttackResolver::Update() method

        if (m_fAttackTime > SPIRIT_ATTACK_TIME)
        {
            m_nAttacking = 0;
        }
    }

}

void Spirit::Update_Animation()
{
    switch (m_nAnimState)
    {
    case ANIM_IDLE:

        if (m_nAttacking != 0)
        {
            m_nAnimState = ANIM_ATTACK;
            m_matter.SetAnimation("Attack");
        }
        else if (m_nJustJumped != 0)
        {
            m_nAnimState = ANIM_FALL;
            m_matter.SetAnimation("Fall");
            m_matter.PlayAnimationOnce("Jump");
        }
        else if (m_nGrounded == 0)
        {
            m_nAnimState = ANIM_FALL;
            m_matter.SetAnimation("Fall");
        }
        else if (abs(GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex)) > DEAD_ZONE)
        {
            m_nAnimState = ANIM_MOVE;
            m_matter.SetAnimation("Move");
        }
        break;

    case ANIM_MOVE:

        if (m_nAttacking != 0)
        {
            m_nAnimState = ANIM_ATTACK;
            m_matter.SetAnimation("Attack");
        }
        else if (m_nJustJumped != 0)
        {
            m_nAnimState = ANIM_FALL;
            m_matter.SetAnimation("Fall");
            m_matter.PlayAnimationOnce("Jump");
        }
        else if (m_nGrounded == 0)
        {
            m_nAnimState = ANIM_FALL;
            m_matter.SetAnimation("Fall");
        }
        else if (abs(GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex)) <= DEAD_ZONE)
        {
            m_nAnimState = ANIM_IDLE;
            m_matter.SetAnimation("Idle");
        }
        break;

    case ANIM_FALL:
        if (m_nAttacking != 0)
        {
            m_nAnimState = ANIM_ATTACK;
            m_matter.SetAnimation("Attack");
            m_matter.PlayAnimationOnce("Attack");
        }
        else if (m_nJustJumped != 0)
        {
            m_nAnimState = ANIM_FALL;
            m_matter.SetAnimation("Fall");
            m_matter.PlayAnimationOnce("Jump");
        }
        else if (m_nGrounded != 0)
        {
            if (abs(GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex)) < DEAD_ZONE)
            {
                m_nAnimState = ANIM_MOVE;
                m_matter.SetAnimation("Move");
            }
            else
            {
                m_nAnimState = ANIM_IDLE;
                m_matter.SetAnimation("Idle");
            }
        }
        break;

    case ANIM_CHARGE:

        break;

    case ANIM_ATTACK:
        if (m_nAttacking == 0)
        {
            if (m_nGrounded == 0)
            {
                m_nAnimState = ANIM_FALL;
                m_matter.SetAnimation("Fall");
            }
            else if (abs(GetControllerAxisValue(VCONT_AXIS_LTHUMB_X, m_nPlayerIndex)) < DEAD_ZONE)
            {
                m_nAnimState = ANIM_MOVE;
                m_matter.SetAnimation("Move");
            }
            else
            {
                m_nAnimState = ANIM_IDLE;
                m_matter.SetAnimation("Idle");
            }
        }
        break;

    default:
        break;
    }
}

int Spirit::GetPercent()
{
    return m_nPercent;
}

int Spirit::GetLives()
{
    return m_nLives;
}

int Spirit::GetPlayerIndex()
{
    return m_nPlayerIndex;
}


void Spirit::SetPlayerIndex(int nIndex)
{
    assert(nIndex >= 0);
    assert(nIndex < MAX_PLAYERS);

    m_nPlayerIndex = nIndex;

    AssignProperTexture();
}

void Spirit::SetLives(int nLives)
{
    m_nLives = nLives;
}

void Spirit::SetPercent(int nPercent)
{
    m_nPercent = nPercent;
}

int Spirit::IsAlive()
{
    return m_nAlive;
}

void Spirit::Kill()
{
    m_nLives--;
    m_nPercent = 0;
    m_fXVelocity = 0.0f;
    m_fYVelocity = 0.0f;
    Game::GetInstance()->GetField()->SpawnSpirit(this);
}

float* Spirit::GetPosition()
{
    return m_arPosition;
}

void Spirit::CreateSharedCollider()
{
    // Prepare static collider if it doesn't exist
    if (s_pSpiritCollider == 0)
    {
        s_pSpiritCollider = new OrientedBoxCollider();
        s_pSpiritCollider->SetHalfExtents(SPIRIT_HALF_EXTENT_X,
                                         SPIRIT_HALF_EXTENT_Y,
                                         SPIRIT_HALF_EXTENT_Z);
        s_pSpiritCollider->SetRelativePosition(0.0f, 0.0f, 0.0f);
        s_pSpiritCollider->SetColor(1.0f, 0.0f, 0.0f, 0.4f);
    }
}


void Spirit::CheckJump()
{
    if (m_nGrounded != 0 &&
        m_nJumpPressed < SPIRIT_JUMP_FRAME_WINDOW)
    {
        m_nGrounded = 0;
        m_fYVelocity = SPIRIT_JUMP_VELOCITY;
        m_nJustJumped = 1;
    }
    else if (m_nGrounded == 0 &&
             m_nJumpPressed == 0 &&
             m_nJumpCharges > 0)
    {
        m_nJumpCharges--;
        m_fYVelocity = SPIRIT_JUMP_VELOCITY;
        m_nJustJumped = 1;
    }
    m_nJumpPressed++;
}

void Spirit::CheckGrounded()
{
    int nHit = 0;

    nHit = m_matter.Translate(0.0f, -SPIRIT_GROUNDING_SWEEP_DISTANCE, 0.0f);

    if (nHit != 0)
    {
        m_nGrounded = 1;
    }
    else
    {
        m_nGrounded = 0;
    }
    
    // Revert to original position
    m_matter.SetPosition(m_arPosition[0],
                         m_arPosition[1],
                         m_arPosition[2]);
}

void Spirit::ApplyGravity()
{
    m_fYVelocity -= GRAVITY * Game::GetInstance()->DeltaTime();
}

void Spirit::ApplyDrag()
{
    if (m_fXVelocity > 0.0f)
    {
        m_fXVelocity -= Game::GetInstance()->DeltaTime() * SPIRIT_DRAG;

        if (m_fXVelocity < 0.0f)
        {
            m_fXVelocity = 0.0f;
        }
    }
    else if (m_fXVelocity < 0.0f)
    {
        m_fXVelocity += Game::GetInstance()->DeltaTime() * SPIRIT_DRAG;

        if (m_fXVelocity > 0.0f)
        {
            m_fXVelocity = 0.0f;
        }
    }
}

void Spirit::AssignProperTexture()
{
    switch (m_nPlayerIndex)
    {
    case 0:
        m_matter.SetTexture(g_pSpirit1Tex);
        break;
    case 1:
        m_matter.SetTexture(g_pSpirit2Tex);
        break;
    case 2:
        m_matter.SetTexture(g_pSpirit3Tex);
        break;
    case 3:
        m_matter.SetTexture(g_pSpirit4Tex);
        break;
    }
}

int Spirit::HasControl()
{
    return 1;
}

Matter* Spirit::GetMatter()
{
    return &m_matter;
}

AttackVolume* Spirit::GetAttackVolume()
{
    return &m_attackVolume;
}

int Spirit::IsAttacking()
{
    return m_nAttacking;
}

int Spirit::GetDamage()
{
    return m_nDamage;
}

void Spirit::ApplyHit(float* arInstigatorPos,
    int nDamage)
{
    m_nPercent += nDamage;

    float arDir[3] = { 0.0f, 0.0f, 0.0f };

    // Calculate the vector pointing from instigator position 
    // to this spirit's position
    arDir[0] = m_arPosition[0] - arInstigatorPos[0];
    arDir[1] = m_arPosition[1] - arInstigatorPos[1];
    arDir[2] = 0.0f;

    // Normalize this direction.
    float fMag = sqrt(arDir[0] * arDir[0] + arDir[1] * arDir[1]);

    if (fMag != 0.0f)
    {
        arDir[0] /= fMag;
        arDir[1] /= fMag;
    }

    float fKnockbackSpeed = SPIRIT_BASE_KNOCKBACK_SPEED + m_nPercent * SPIRIT_PERCENT_KNOCKBACK_MULTIPLIER;

    m_fXVelocity = fKnockbackSpeed * arDir[0];
    m_fYVelocity = fKnockbackSpeed * arDir[1];
}

void Spirit::SetPosition(float fX,
                         float fY,
                         float fZ)
{
    m_arPosition[0] = fX;
    m_arPosition[1] = fY;
    m_arPosition[2] = fZ;
    
    m_matter.SetPosition(fX, fY, fZ);
}