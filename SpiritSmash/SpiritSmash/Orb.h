#ifndef ORB_H
#define ORB_H

#include "Matter.h"
#include "RimlitMaterial.h"
#include "PointLight.h"
#include "OrientedBoxCollider.h"

class Orb
{

public:

    enum OrbState
    {
        ORB_INACTIVE = 0,
        ORB_CHARGING = 1,
        ORB_LAUNCHED = 2
    };

    Orb();
    ~Orb();

    void Reset();

    static void CreateSharedObjects();

    void Update();
    
    Matter* GetMatter();

    int GetState();

    void SetState(int nState);

    void SetOwner(int nPlayerIndex);

    int IncreaseSize(float fDeltaSize);

    void Launch(float fDirection);

    void ApplyHit(class Spirit* pTarget);

private:

    int IsOutOfKillBounds();

    class Spirit* m_pOwner;
    int m_nOwner;
    int m_nState;
    float m_fSpeed;
    float m_fSize;

    Matter m_matter;
    PointLight m_light;

    static OrientedBoxCollider* s_pSharedCollider;
    static RimlitMaterial* s_pSharedMat1;
    static RimlitMaterial* s_pSharedMat2;
    static RimlitMaterial* s_pSharedMat3;
    static RimlitMaterial* s_pSharedMat4;
};

#endif