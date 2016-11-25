#include "Game.h"
#include "Spirit.h"
#include "Constants.h"
#include "ForestField.h"
#include <assert.h>

Game::Game()
{
    m_pField = 0;

    m_arSpirit[0] = 0;
    m_arSpirit[1] = 0;
    m_arSpirit[2] = 0;
    m_arSpirit[3] = 0;
}

void Game::Start(int nPlayers,
                 int nField)
{
    int i = 0;

    for (i = 0; i < nPlayers; i++)
    {
        m_arSpirit[i] = new Spirit();
        m_arSpirit[i]->SetGame(this);
        m_arSpirit[i]->SetPlayerIndex(i);
    }

    switch (nField)
    {
        case FIELD_TYPE_FOREST:
            m_pField = new ForestField();
            break;
        default:
            break;
    }
}

Spirit* Game::GetSpirit(int nPlayerIndex)
{
    assert(nPlayerIndex > 0);
    assert(nPlayerIndex < MAX_PLAYERS);

    return m_arSpirit[nPlayerIndex];
}