#ifndef PLATFORM_H
#define PLATFORM_H

#include "Matter.h"
#include "OrientedBoxCollider.h"

class Platform
{
public:
    Platform();
    ~Platform();

    void Generate(int nSize,
                  float fX,
                  float fY,
                  float fZ);

protected:

    static OrientedBoxCollider* s_pPlatformCollider;

    int m_nSize;

    Matter* m_arMatter;
};

#endif