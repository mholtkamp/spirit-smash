#include "Field.h"
#include "Game.h"
#include "Spirit.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

Field::Field()
{
    m_pGame = Game::GetInstance();

    SYSTEMTIME st;
    GetSystemTime(&st);

    srand(st.wMilliseconds);
}

Field::~Field()
{

}

void Field::Generate()
{


}

void Field::Update()
{

}

void Field::SpawnSpirit(Spirit* pSpirit)
{
    int nRand = rand() % 4;

    pSpirit->SetPosition(m_arSpawns[nRand][0],
                         m_arSpawns[nRand][1],
                         0.0f);
}

int Field::IsOutOfKillBounds(float fX,
                             float fY)

{
    if (fX > m_arKillExtents[KILL_RIGHT])
        return 1;
    else if (fX < m_arKillExtents[KILL_LEFT])
        return 1;
    else if (fY > m_arKillExtents[KILL_TOP])
        return 1;
    else if (fY < m_arKillExtents[KILL_BOTTOM])
        return 1;

    return 0;
}