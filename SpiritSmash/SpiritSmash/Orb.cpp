#include "Orb.h"
#include "Constants.h"
#include "Assets.h"
#include "Game.h"

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

    m_nState = ORB_INACTIVE;
    m_fSpeed = 0.0f;
    m_fSize = ORB_MIN_SIZE;

    m_matter.SetMesh(g_pOrbMesh);
    m_matter.SetTexture(0);
    m_matter.SetMaterial(s_pSharedMat1);
    m_matter.AddCollider(s_pSharedCollider);
    m_matter.EnableColliderRendering();
    Game::GetInstance()->GetScene()->AddActor(&m_matter);

    m_light.SetRadius(ORB_LIGHT_RADIUS_SCALE * m_fSize);
    m_light.SetColor(SPIRIT_1_ORB_COLOR[0],
                     SPIRIT_1_ORB_COLOR[1],
                     SPIRIT_1_ORB_COLOR[2]);
    m_light.SetEnable(0);
    Game::GetInstance()->GetScene()->AddActor(&m_light);
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
}

int Orb::GetState()
{
    return m_nState;
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