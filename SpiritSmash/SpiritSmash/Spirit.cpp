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

    m_matter.SetTexture(g_pSpiritTex);
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
    m_nLives       = 1;

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

    

    m_nAlive = 0;
}

Spirit::~Spirit()
{

}

void Spirit::Update()
{
    Update_Kinematics();
    Update_Orientation();
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

    if (IsControllerButtonDown(VCONT_A, m_nPlayerIndex))
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
    m_matter.Translate(fDeltaTime * m_fXVelocity, 0.0f, 0.0f);

    // Translate in Y if not grounded.
    if (m_nGrounded == 0)
    {
        int nHit = 0;
        nHit = m_matter.Translate(0.0f, fDeltaTime * m_fYVelocity, 0.0f);

        if (nHit != 0)
        {
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

void Spirit::Update_Animation()
{
    switch (m_nAnimState)
    {
    case ANIM_IDLE:

        if (m_nJustJumped != 0)
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

        if (m_nJustJumped != 0)
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
        if (m_nGrounded != 0)
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
    m_nAlive = 0;
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
        m_matter.SetTexture(g_pSpiritTex);
        break;
    case 1:
        m_matter.SetTexture(g_pSpiritTex);
        break;
    case 2:
        m_matter.SetTexture(g_pSpiritTex);
        break;
    case 3:
        m_matter.SetTexture(g_pSpiritTex);
        break;
    }
}