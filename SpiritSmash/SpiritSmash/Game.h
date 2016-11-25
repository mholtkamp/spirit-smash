#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "Spirit.h"
#include "Scene.h"

extern Scene* s_pScene;

class Game
{
public:

    Game();
    ~Game();

    void Update();

    Spirit* GetSpirit(int nPlayerIndex);

    Field* GetField();

    void Start(int nPlayers,
               int nField);

    float DeltaTime();

private:

    Field*  m_pField;
    Spirit* m_arSpirit[4];
};

#endif