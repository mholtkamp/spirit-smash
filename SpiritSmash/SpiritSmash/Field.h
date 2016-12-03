#ifndef FIELD_H
#define FIELD_H

#include "Platform.h"
#include "Matter.h"

class Field
{
public:

    Field();
    virtual ~Field();

    virtual void Generate();

    virtual void Update();

    void SpawnSpirit(class Spirit* pSpirit);

    int IsOutOfKillBounds(float fX,
                          float fY);

protected:

    class Game* m_pGame;
    Platform* m_arPlatforms;
    Matter* m_arMatter;
    float m_arSpawns[4][3];
    float m_arKillExtents[4];

};


#endif