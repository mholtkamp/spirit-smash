#ifndef FIELD_H
#define FIELD_H

#include "Platform.h"
#include "Matter.h"

class Field
{
public:

    Field();
    virtual ~Field();

    void SetGame(class Game* pGame);

    virtual void Generate();

    virtual void Update();

protected:

    class Game* m_pGame;
    Platform* m_arPlatforms;
    Matter* m_arMatter;
    float m_arSpawns[4][3];
    float m_arKillExtents[4];

};


#endif