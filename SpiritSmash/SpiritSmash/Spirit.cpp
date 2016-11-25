#include "Spirit.h"
#include "VInput.h"
#include "Assets.h"
#include "Game.h"
#include "Constants.h"

Spirit::Spirit()
{
    // Setup graphical component 
    m_matter.SetMesh(g_pSpiritAnimMesh);
    m_matter.SetLoopMode(Matter::LOOP_PING_PONG);
    m_matter.SetAnimation("Idle");
    m_matter.StartAnimation();

    m_matter.SetTexture(g_pSpiritTex);
    m_matter.SetMaterial(g_pDefaultToonMaterial);
    m_matter.SetPosition(0.0f, 0.0f, 0.0f);

    // Attributes
    m_nPlayerIndex = 0;
    m_nPercent     = 0;
    m_nLives       = 1;

    m_arPosition[0] = 0.0f;
    m_arPosition[1] = 0.0f;
    m_arPosition[2] = 0.0f;

    m_fSpeed = SPIRIT_DEFAULT_SPEED;
    m_nJumpPressed = SPIRIT_JUMP_FRAME_WINDOW;

    m_nAlive = 0;
}

Spirit::~Spirit()
{

}

void Spirit::Update()
{
    float fXVelocity = 0.0f;
    
    // Get relevant input.
    if (IsKeyDown(VKEY_LEFT))
    {
        fXVelocity = -m_fSpeed;
    }
    else if (IsKeyDown(VKEY_RIGHT))
    {
        fXVelocity = m_fSpeed;
    }

    if (IsKeyDown(VKEY_SPACE))
    {
        m_nJumpPressed = 0;
    }

    // Move spirit horizontally.
    m_arPosition[0] += m_fSpeed * m_pGame->DeltaTime();

    // Update the matter
    m_matter.SetPosition(m_arPosition[0],
                         m_arPosition[1],
                         m_arPosition[2]);
}

void Spirit::SetGame(Game* pGame)
{
    m_pGame = pGame;
}

int Spirit::GetPercent()
{
    return m_nPercent;
}

int Spirit::GetLives()
{
    return m_nLives;
}

int Spirit::GetPlayerIndex()
{
    return m_nPlayerIndex;
}


void Spirit::SetPlayerIndex(int nIndex)
{
    m_nPlayerIndex = nIndex;
}

void Spirit::SetLives(int nLives)
{
    m_nLives = nLives;
}

void Spirit::SetPercent(int nPercent)
{
    m_nPercent = nPercent;
}

int Spirit::IsAlive()
{
    return m_nAlive;
}

void Spirit::Kill()
{
    m_nAlive = 0;
}