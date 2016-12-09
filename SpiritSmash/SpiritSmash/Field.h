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

    virtual void Reset();

    void SpawnSpirit(class Spirit* pSpirit);

    int IsOutOfKillBounds(float fX,
                          float fY);

    void GetRandomSpawnLocation(float* arOutPos);

    void GetKillExtents(float* arOutExtents);

    void SetStartPosition(class Spirit* pSpirit, int nIndex);

    const float* GetClampBounds();

protected:

    class Game* m_pGame;
    Platform* m_arPlatforms;
    Matter* m_arMatter;
    float m_arSpawns[4][3];
    float m_arKillExtents[4];
    float m_arClampBounds[4];

};


#endif