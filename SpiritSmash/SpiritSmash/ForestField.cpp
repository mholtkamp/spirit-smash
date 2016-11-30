#include "ForestField.h"
#include "Constants.h"
#include "Game.h"

ForestField::ForestField()
{
    m_arPlatforms = new Platform[FIELD_FOREST_PLATFORM_COUNT];

    // Spawn 0
    m_arSpawns[0][0] = 7.0f;
    m_arSpawns[0][1] = 7.0f;
    m_arSpawns[0][2] = 0.0f;

    // Spawn 1
    m_arSpawns[0][0] = 7.0f;
    m_arSpawns[0][1] = 7.0f;
    m_arSpawns[0][2] = 0.0f;

    // Spawn 2
    m_arSpawns[0][0] = 7.0f;
    m_arSpawns[0][1] = 7.0f;
    m_arSpawns[0][2] = 0.0f;

    // Spawn 3
    m_arSpawns[0][0] = 7.0f;
    m_arSpawns[0][1] = 7.0f;
    m_arSpawns[0][2] = 0.0f;

    // Kill Extents
    m_arKillExtents[KILL_LEFT]   = -0.0f;
    m_arKillExtents[KILL_RIGHT]  = 30.0f;
    m_arKillExtents[KILL_TOP]    = 25.0f;
    m_arKillExtents[KILL_BOTTOM] = -5.0f;
}

ForestField::~ForestField()
{
    if (m_arPlatforms != 0)
    {
        delete [] m_arPlatforms;
        m_arPlatforms = 0;
    }
}

void ForestField::Update()
{
    Field::Update();

    int i = 0;
    Spirit* pSpirit = 0;
    float fX = 0.0f;
    float fY = 0.0f;

    // Check if a player has been knocked off.
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        pSpirit = m_pGame->GetSpirit(i);

        if (pSpirit != 0 &&
            pSpirit->IsAlive())
        {
            fX = pSpirit->GetPosition()[0];
            fY = pSpirit->GetPosition()[1];

            if (fX > m_arKillExtents[KILL_RIGHT] ||
                fX < m_arKillExtents[KILL_LEFT] ||
                fY > m_arKillExtents[KILL_TOP] ||
                fY < m_arKillExtents[KILL_BOTTOM])
            {
                pSpirit->Kill();
            }

        }
    }
}

void ForestField::Generate()
{
    // Middleground (gameplay) platforms
    m_arPlatforms[0].Generate(2, 13.0f, 16.0f, MIDDLEGROUND_Z);
    m_arPlatforms[1].Generate(3, 0.0f,  14.0f, MIDDLEGROUND_Z);
    m_arPlatforms[2].Generate(3, 24.0f, 14.0f, MIDDLEGROUND_Z);
    m_arPlatforms[3].Generate(2, 8.0f,  12.0f, MIDDLEGROUND_Z);
    m_arPlatforms[4].Generate(2, 18.0f, 12.0f, MIDDLEGROUND_Z);
    m_arPlatforms[5].Generate(5, 10.0f, 7.0f,  MIDDLEGROUND_Z);
    m_arPlatforms[6].Generate(2, 3.0f,  3.0f,  MIDDLEGROUND_Z);
    m_arPlatforms[7].Generate(3, 12.0f, 1.0f,  MIDDLEGROUND_Z);
    m_arPlatforms[8].Generate(2, 23.0f, 3.0f,  MIDDLEGROUND_Z);

    // Background platforms
    m_arPlatforms[9].Generate(2,  20.0f, 16.0f, BACKGROUND_Z);
    m_arPlatforms[10].Generate(2, 22.0f, 9.0f,  BACKGROUND_Z);
    m_arPlatforms[11].Generate(2, 3.0f,  7.0f,  BACKGROUND_Z);

    //m_arPlatforms[0].Generate(7, 0.0f, 0.0f, 0.0f);
    //m_arPlatforms[1].Generate(3, -2.0f, 5.0f, 0.0f);
    //m_arPlatforms[2].Generate(3, 5.0f, 5.0f, 0.0f);
    //m_arPlatforms[3].Generate(3, 12.0f, 5.0f, 0.0f);
    //m_arPlatforms[4].Generate(3, 5.0f, 8.0f, 0.0f);
}