#include "Field.h"
#include "Game.h"
#include "Spirit.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <assert.h>

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
                         MIDDLEGROUND_Z);
}

void Field::GetRandomSpawnLocation(float* arOutPos)
{
    assert(arOutPos != 0);

    int nRand = rand() % FIELD_NUM_SPAWNS;

    arOutPos[0] = m_arSpawns[nRand][0];
    arOutPos[1] = m_arSpawns[nRand][1];
    arOutPos[2] = MIDDLEGROUND_Z;
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

const float* Field::GetClampBounds()
{
    return m_arClampBounds;
}

void Field::SetStartPosition(Spirit* pSpirit, int nIndex)
{
    assert(pSpirit != 0);
    assert(nIndex >= 0);
    assert(nIndex < MAX_PLAYERS);

    pSpirit->SetPosition(m_arSpawns[nIndex][0],
                         m_arSpawns[nIndex][1],
                         MIDDLEGROUND_Z);
}

void Field::GetKillExtents(float* arOutExtents)
{
    assert(arOutExtents != 0);

    for (int i = 0; i < 4; i++)
    {
        arOutExtents[i] = m_arKillExtents[i];
    }
}

void Field::Reset()
{

}