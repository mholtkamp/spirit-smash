#ifndef ATTACK_VOLUME_H
#define ATTACK_VOLUME_H

#include "Constants.h"

#include "Matter.h"
#include "OrientedBoxCollider.h"

class AttackVolume
{
public:

    AttackVolume();
    ~AttackVolume();

    float GetPriority();

    void SetElapsedTime(float fTime);

    int OverlapsSpirit(class Spirit* pSpirit);

    void SetPosition(float fX,
                     float fY,
                     float fZ);

    Matter* GetMatter();

    void ResetHitPlayers();

    int HasHitPlayer(int nPlayerIndex);

    void SetHitPlayer(int nPlayerIndex);

private:

    static void CreateSharedCollider();

    static OrientedBoxCollider* s_pAttackCollider;

    void CalculateCurrentPriority();

    Matter m_matter;

    float m_fPriority;

    float m_fTime;

    int m_arHitPlayers[MAX_PLAYERS];
};


#endif