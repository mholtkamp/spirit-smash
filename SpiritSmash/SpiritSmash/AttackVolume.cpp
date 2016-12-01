#include "AttackVolume.h"
#include "Constants.h"
#include "Spirit.h"
#include "Game.h"
#include "Assets.h"

#include <math.h>

OrientedBoxCollider* AttackVolume::s_pAttackCollider = 0;

AttackVolume::AttackVolume()
{
    CreateSharedCollider();

    
    m_matter.SetMaterial(g_pDefaultMaterial);
    m_matter.SetMesh(g_pAttackDebugMesh);
    m_matter.SetTexture(0);
    
    m_matter.SetScale(1.0f, 1.0f, 1.0f);
    m_matter.AddCollider(s_pAttackCollider);
    m_matter.EnableColliderRendering();
    Game::GetInstance()->GetScene()->AddActor(&m_matter);

    m_fPriority = 0.0f;
    m_fTime = 0.0f;
}

AttackVolume::~AttackVolume()
{

}

float AttackVolume::GetPriority()
{
    return m_fPriority;
}

void AttackVolume::SetElapsedTime(float fTime)
{
    m_fTime = fTime;

    CalculateCurrentPriority();
}

int AttackVolume::OverlapsSpirit(Spirit* pSpirit)
{
    return m_matter.Overlaps(pSpirit->GetMatter());
}

void AttackVolume::CreateSharedCollider()
{
    if (s_pAttackCollider == 0)
    {
        s_pAttackCollider = new OrientedBoxCollider();
        s_pAttackCollider->SetColor(1.0f, 1.0f, 0.0f, 0.4f);
        s_pAttackCollider->SetHalfExtents(ATTACK_VOLUME_HALF_EXTENT_X, 
                                          ATTACK_VOLUME_HALF_EXTENT_Y,
                                          ATTACK_VOLUME_HALF_EXTENT_Z);
        s_pAttackCollider->SetRelativePosition(0.0f, 0.0f, 0.0f);
    }
}

void AttackVolume::CalculateCurrentPriority()
{
    m_fPriority = -abs(m_fTime - SPIRIT_ATTACK_TIME / 2.0f) + SPIRIT_ATTACK_TIME/2.0f;

    if (m_fPriority < 0.0f)
    {
        m_fPriority = 0.0f;
    }
    else if (m_fPriority > SPIRIT_ATTACK_TIME / 2.0f)
    {
        m_fPriority = SPIRIT_ATTACK_TIME / 2.0f;
    }
}

void AttackVolume::SetPosition(float fX,
                               float fY,
                               float fZ)
{
    m_matter.SetPosition(fX, fY, fZ);
}

Matter* AttackVolume::GetMatter()
{
    return &m_matter;
}