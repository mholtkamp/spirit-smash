#ifndef ORB_H
#define ORB_H

#include "Matter.h"
#include "DiffuseMaterial.h"
#include "PointLight.h"
#include "OrientedBoxCollider.h"

class Orb
{

public:
    Orb();
    ~Orb();

    static void CreateSharedObjects();

    void Update();
    
    Matter* GetMatter();

    int IsActive();

private:

    int m_nActive;
    float m_fSpeed;
    float m_fSize;

    Matter m_matter;
    PointLight m_light;

    static OrientedBoxCollider* s_pSharedCollider;
    static DiffuseMaterial* s_pSharedMaterial;
};

#endif