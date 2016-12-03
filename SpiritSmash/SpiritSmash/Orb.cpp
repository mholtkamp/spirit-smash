#include "Orb.h"
#include "Constants.h"
#include "Assets.h"
#include "Game.h"

#include "Matter.h"

#include <assert.h>

OrientedBoxCollider* Orb::s_pSharedCollider = 0;
RimlitMaterial *Orb::s_pSharedMat1 = 0;
RimlitMaterial *Orb::s_pSharedMat2 = 0;
RimlitMaterial *Orb::s_pSharedMat3 = 0;
RimlitMaterial *Orb::s_pSharedMat4 = 0;

static float ORB_BASE_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
static float SPIRIT_1_ORB_COLOR[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
static float SPIRIT_2_ORB_COLOR[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
static float SPIRIT_3_ORB_COLOR[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
static float SPIRIT_4_ORB_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

Orb::Orb()
{
    CreateSharedObjects();

    SetState(ORB_INACTIVE);
    m_fSpeed = 0.0f;
    m_fSize = ORB_MIN_SIZE;

    m_matter.SetMesh(g_pOrbMesh);
    m_matter.SetTexture(0);
    m_matter.SetMaterial(s_pSharedMat1);
    m_matter.AddCollider(s_pSharedCollider);
    m_matter.EnableColliderRendering();
    m_matter.SetScale(ORB_MIN_SIZE, ORB_MIN_SIZE, ORB_MIN_SIZE);
    Game::GetInstance()->GetScene()->AddActor(&m_matter);

    m_light.SetRadius(ORB_LIGHT_RADIUS_SCALE * m_fSize);
    m_light.SetIntensity(ORB_LIGHT_INTENSITY * m_fSize);
    m_light.SetColor(SPIRIT_1_ORB_COLOR[0],
                     SPIRIT_1_ORB_COLOR[1],
                     SPIRIT_1_ORB_COLOR[2]);
    Box boxLight;
    boxLight.m_arExtent[0] = ORB_LIGHT_BOX_EXTENTS;
    boxLight.m_arExtent[1] = ORB_LIGHT_BOX_EXTENTS;
    boxLight.m_arExtent[2] = ORB_LIGHT_BOX_EXTENTS;
    m_light.SetBox(boxLight);
    Game::GetInstance()->GetScene()->AddActor(&m_light);
}

void Orb::Reset()
{
    m_fSize = ORB_MIN_SIZE;
    m_fSpeed = 0.0f;
    m_matter.SetScale(ORB_MIN_SIZE, ORB_MIN_SIZE, ORB_MIN_SIZE);
    m_light.SetRadius(ORB_LIGHT_RADIUS_SCALE * m_fSize);
    m_light.SetIntensity(ORB_LIGHT_INTENSITY * m_fSize);
}

void Orb::CreateSharedObjects()
{
    if (s_pSharedCollider == 0)
    {
        s_pSharedCollider = new OrientedBoxCollider();
        s_pSharedCollider->SetHalfExtents(ORB_HALF_EXTENTS,
                                          ORB_HALF_EXTENTS,
                                          ORB_HALF_EXTENTS);
        s_pSharedCollider->SetRelativePosition(0.0f, 0.0f, 0.0f);
        s_pSharedCollider->SetColor(1.0f, 1.0f, 1.0f, 0.4f);
    }

    if (s_pSharedMat1 == 0)
    {
        s_pSharedMat1 = new RimlitMaterial();
        s_pSharedMat1->SetColor(ORB_BASE_COLOR);
        s_pSharedMat1->SetRimColor(SPIRIT_1_ORB_COLOR);
        s_pSharedMat1->SetRimSize(ORB_RIM_SIZE);
        s_pSharedMat1->SetRimStyle(RIM_STYLE_SOFT);
    }
    if (s_pSharedMat2 == 0)
    {
        s_pSharedMat2 = new RimlitMaterial();
        s_pSharedMat2->SetColor(ORB_BASE_COLOR);
        s_pSharedMat2->SetRimColor(SPIRIT_2_ORB_COLOR);
        s_pSharedMat2->SetRimSize(ORB_RIM_SIZE);
        s_pSharedMat2->SetRimStyle(RIM_STYLE_SOFT);
    }
    if (s_pSharedMat3 == 0)
    {
        s_pSharedMat3 = new RimlitMaterial();
        s_pSharedMat3->SetColor(ORB_BASE_COLOR);
        s_pSharedMat3->SetRimColor(SPIRIT_3_ORB_COLOR);
        s_pSharedMat3->SetRimSize(ORB_RIM_SIZE);
        s_pSharedMat3->SetRimStyle(RIM_STYLE_SOFT);
    }
    if (s_pSharedMat4 == 0)
    {
        s_pSharedMat4 = new RimlitMaterial();
        s_pSharedMat4->SetColor(ORB_BASE_COLOR);
        s_pSharedMat4->SetRimColor(SPIRIT_4_ORB_COLOR);
        s_pSharedMat4->SetRimSize(ORB_RIM_SIZE);
        s_pSharedMat4->SetRimStyle(RIM_STYLE_SOFT);
    }
}

Orb::~Orb()
{

}

void Orb::Update()
{
    float fDeltaTime = Game::GetInstance()->DeltaTime();

    if (m_nState == ORB_LAUNCHED)
    {
        float arNewPos[3] = {0.0f};
        const float* arCurPos = m_matter.GetPosition();

        arNewPos[0] = arCurPos[0] + m_fSpeed * fDeltaTime;
        arNewPos[1] = arCurPos[1];
        arNewPos[2] = arCurPos[2];

        m_matter.SetPosition(arNewPos[0],
                             arNewPos[1],
                             arNewPos[2]);
        m_light.SetPosition(arNewPos[0],
                            arNewPos[1],
                            arNewPos[2]);

        if (IsOutOfKillBounds())
        {
            SetState(ORB_INACTIVE);
        }
    }

    if (m_nState == ORB_CHARGING)
    {
        float* arSpiritPos = m_pOwner->GetPosition();
        
        float fOrbX = arSpiritPos[0];
        float fOrbY = arSpiritPos[1] + ORB_SPIRIT_OFFSET_Y;

        if (m_pOwner->GetDirection() == DIRECTION_RIGHT)
        {
            fOrbX += ORB_SPIRIT_OFFSET_X;
        }
        else
        {
            fOrbX -= ORB_SPIRIT_OFFSET_X;
        }

        m_matter.SetPosition(fOrbX, fOrbY, MIDDLEGROUND_Z);
        m_light.SetPosition(fOrbX, fOrbY, MIDDLEGROUND_Z);
    }
}

int Orb::IsOutOfKillBounds()
{
    Field* pField = Game::GetInstance()->GetField();
    const float* arPos = m_matter.GetPosition();

    return pField->IsOutOfKillBounds(arPos[0], arPos[1]);
}

int Orb::GetState()
{
    return m_nState;
}

void Orb::SetState(int nState)
{
    m_nState = nState;

    if (m_nState == ORB_INACTIVE)
    {
        m_matter.SetVisible(0);
        m_light.SetEnable(0);
    }
    else
    {
        m_matter.SetVisible(1);
        m_light.SetEnable(1);
    }
}

Matter* Orb::GetMatter()
{
    return &m_matter;
}

void Orb::SetOwner(int nPlayerIndex)
{
    assert(nPlayerIndex >= 0);
    assert(nPlayerIndex < MAX_PLAYERS);

    m_nOwner = nPlayerIndex;
    m_pOwner = Game::GetInstance()->GetSpirit(nPlayerIndex);

    switch (m_nOwner)
    {
    case 0:
        m_matter.SetMaterial(s_pSharedMat1);
        m_light.SetColor(SPIRIT_1_ORB_COLOR[0],
                         SPIRIT_1_ORB_COLOR[1],
                         SPIRIT_1_ORB_COLOR[2]);
        break;
    case 1:
        m_matter.SetMaterial(s_pSharedMat2);
        m_light.SetColor(SPIRIT_2_ORB_COLOR[0],
                         SPIRIT_2_ORB_COLOR[1],
                         SPIRIT_2_ORB_COLOR[2]);
        break;
    case 2:
        m_matter.SetMaterial(s_pSharedMat3);
        m_light.SetColor(SPIRIT_3_ORB_COLOR[0],
                         SPIRIT_3_ORB_COLOR[1],
                         SPIRIT_3_ORB_COLOR[2]);
        break;
    case 3:
        m_matter.SetMaterial(s_pSharedMat4);
        m_light.SetColor(SPIRIT_4_ORB_COLOR[0],
                         SPIRIT_4_ORB_COLOR[1],
                         SPIRIT_4_ORB_COLOR[2]);
        break;
    default:
        break;
    }
}

int Orb::IncreaseSize(float fDeltaSize)
{
    int nRet = 0;

    m_fSize += fDeltaSize;

    if (m_fSize >= ORB_MAX_SIZE)
    {
        m_fSize = ORB_MAX_SIZE;
        nRet = 1;
    }

    m_matter.SetScale(m_fSize, m_fSize, m_fSize);
    m_light.SetIntensity(m_fSize * ORB_LIGHT_RADIUS_SCALE);
    return nRet;
}

void Orb::Launch(float fDirection)
{
    m_fSpeed = ORB_BASE_SPEED + m_fSize * ORB_SPEED_SIZE_SCALE;
    
    if (fDirection < 0)
    {
        m_fSpeed = -m_fSpeed;
    }

    float* arSpiritPos = m_pOwner->GetPosition();
    int nRight = m_pOwner->GetDirection() == DIRECTION_RIGHT;
    m_matter.SetPosition(arSpiritPos[0] + (nRight ? ORB_SPIRIT_OFFSET_X : -ORB_SPIRIT_OFFSET_X),
                         arSpiritPos[1] + ORB_SPIRIT_OFFSET_Y,
                         MIDDLEGROUND_Z);
    m_light.SetPosition(arSpiritPos[0] + (nRight ? ORB_SPIRIT_OFFSET_X : -ORB_SPIRIT_OFFSET_X),
                        arSpiritPos[1] + ORB_SPIRIT_OFFSET_Y,
                        MIDDLEGROUND_Z);

    m_matter.SetScale(m_fSize, m_fSize, m_fSize);
    m_light.SetIntensity(m_fSize * ORB_LIGHT_RADIUS_SCALE);

    SetState(ORB_LAUNCHED);
}

void Orb::ApplyHit(Spirit* pTarget)
{
    assert(pTarget != m_pOwner);

    int nDamage = static_cast<int>((m_fSize - ORB_MIN_SIZE) * ORB_DAMAGE_SCALE);

    // Clamp the damage
    if (nDamage < ORB_MIN_DAMAGE)
        nDamage = ORB_MIN_DAMAGE;
    if (nDamage > ORB_MAX_DAMAGE)
        nDamage = ORB_MAX_DAMAGE;

    // Only apply hit if the orb is at least half charged
    if (m_fSize > (ORB_MAX_SIZE - ORB_MIN_SIZE) / 2 + ORB_MIN_SIZE)
    {
        const float* arMatterPos = m_matter.GetPosition();
        float arInstPos[3] = { arMatterPos[0],
            arMatterPos[1],
            arMatterPos[2] };

        arInstPos[1] -= ORB_INSTIGATOR_OFFSET;

        pTarget->ApplyHit(arInstPos, nDamage);
    }
    else
    {
        // Orb not big enough so do not hit, but do apply damage.
        pTarget->ApplyDamage(nDamage);
    }
}
