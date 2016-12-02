#include "Orb.h"
#include "Constants.h"
#include "Assets.h"
#include "Game.h"

OrientedBoxCollider* Orb::s_pSharedCollider = 0;
DiffuseMaterial* Orb::s_pSharedMaterial = 0;

static const float ORB_COLOR[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

Orb::Orb()
{
    CreateSharedObjects();

    m_nActive = 0;
    m_fSpeed = 0.0f;
    m_fSize = ORB_MIN_SIZE;

    m_matter.SetMesh(g_pOrbMesh);
    m_matter.SetTexture(0);
    m_matter.SetMaterial(s_pSharedMaterial);
    m_matter.AddCollider(s_pSharedCollider);
    m_matter.EnableColliderRendering();
    Game::GetInstance()->GetScene()->AddActor(&m_matter);

    m_light.SetRadius(ORB_LIGHT_RADIUS_SCALE * m_fSize);
    m_light.SetColor(ORB_COLOR[0],
                     ORB_COLOR[1],
                     ORB_COLOR[2]);
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

    if (s_pSharedMaterial == 0)
    {
        s_pSharedMaterial = new DiffuseMaterial();
        s_pSharedMaterial->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
    }
}

Orb::~Orb()
{

}

void Orb::Update()
{
    float fDeltaTime = Game::GetInstance()->DeltaTime();
}

int Orb::IsActive()
{
    return m_nActive;
}

Matter* Orb::GetMatter()
{
    return &m_matter;
}