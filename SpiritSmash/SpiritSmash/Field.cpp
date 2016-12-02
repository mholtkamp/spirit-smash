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
