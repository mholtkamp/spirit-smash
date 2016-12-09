#include "HUD.h"
#include "Game.h"
#include "Assets.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static float COLOR_1[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
static float COLOR_2[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
static float COLOR_3[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
static float COLOR_4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

static float* COLORS[4] = { 
                            COLOR_1,
                            COLOR_2,
                            COLOR_3,
                            COLOR_4
                           };
HUD::HUD()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (Game::GetInstance()->GetSpirit(i) != 0)
        {
            SetPlayerEnable(i, 1);
        }
        else
        {
            SetPlayerEnable(i, 0);
        }

        RegisterGlyphs(i);
    }

    Reset();
}

HUD::~HUD()
{

}

void HUD::Reset()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (Game::GetInstance()->GetSpirit(i) != 0)
        {
            SetPlayerEnable(i, 1);
        }
        else
        {
            SetPlayerEnable(i, 0);
        }

        SetLives(i, SPIRIT_STARTING_LIVES);
        SetPercent(i, 0);
        SetColors(i, COLORS[i]);
        SetTextures(i);
        InitializePosition(i);
        SetPercent(i, 0);
    }
}

void HUD::SetPercent(int nPlayerIndex,
                     int nPercent)
{
    assert(nPercent >= 0);
    assert(nPlayerIndex >= 0);
    assert(nPlayerIndex < MAX_PLAYERS);

    char arString[16] = { 0 };
    sprintf(arString, "%d", nPercent);

    m_arPercents[nPlayerIndex].SetText(arString);

    // Adjust the position of the text based on 
    // the length of the text
    float fX = HUD_X + HUD_SPACING * nPlayerIndex;
    float fY = HUD_Y;

    if (strlen(arString) == 1)
    {
        m_arPercents[nPlayerIndex].SetPosition(fX + HUD_PERCENT_OFF_X_1,
                                               fY + HUD_PERCENT_OFF_Y);
    }
    else if (strlen(arString) == 2)
    {
        m_arPercents[nPlayerIndex].SetPosition(fX + HUD_PERCENT_OFF_X_2,
                                               fY + HUD_PERCENT_OFF_Y);
    }
    else
    {
        m_arPercents[nPlayerIndex].SetPosition(fX + HUD_PERCENT_OFF_X_3,
                                               fY + HUD_PERCENT_OFF_Y);
    }
}

void HUD::RegisterGlyphs(int nIndex)
{
    assert(nIndex >= 0);
    assert(nIndex < MAX_PLAYERS);

    Scene* pScene = Game::GetInstance()->GetScene();
    pScene->AddGlyph(&m_arBackground[nIndex]);
    pScene->AddGlyph(&m_arPercents[nIndex]);
    
    for (int i = 0; i < SPIRIT_STARTING_LIVES; i++)
    {
        pScene->AddGlyph(&m_arLifeBubbles[nIndex][i]);
    }
}

void HUD::SetLives(int nPlayerIndex,
                   int nLives)
{
    assert(nLives >= 0);
    assert(nLives <= SPIRIT_STARTING_LIVES);
    assert(nPlayerIndex >= 0);
    assert(nPlayerIndex < MAX_PLAYERS);

    for (int i = 0; i < SPIRIT_STARTING_LIVES; i++)
    {
        if (m_arPlayerEnabled[nPlayerIndex] != 0)
        {
            m_arLifeBubbles[nPlayerIndex][i].SetVisible(nLives > i ? 1 : 0);
        }
    }
}

void HUD::SetPlayerEnable(int nIndex,
                          int nEnable)
{
    assert(nIndex >= 0);
    assert(nIndex < MAX_PLAYERS);

    // Set enable flag
    m_arPlayerEnabled[nIndex] = nEnable;

    // Set visibility of glyphs according to enable flag
    for (int i = 0; i < SPIRIT_STARTING_LIVES; i++)
    {
        m_arLifeBubbles[nIndex][i].SetVisible(nEnable);
    }
    
    m_arPercents[nIndex].SetVisible(nEnable);
    m_arBackground[nIndex].SetVisible(nEnable);
    
}

void HUD::SetColors(int nIndex, 
                    float* arColor)
{
    assert(nIndex >= 0);
    assert(nIndex < MAX_PLAYERS);

    m_arPercents[nIndex].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_arBackground[nIndex].SetColor(arColor[0],
                                    arColor[1],
                                    arColor[2],
                                    HUD_BACKGROUND_ALPHA);
    
    for (int i = 0; i < SPIRIT_STARTING_LIVES; i++)
    {
        m_arLifeBubbles[nIndex][i].SetColor(arColor);
    }

    // Special case for 4th guy,
    // since his color white
    if (nIndex == 3)
    {
        m_arPercents[nIndex].SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

}

void HUD::InitializePosition(int nIndex)
{
    assert(nIndex >= 0);
    assert(nIndex < MAX_PLAYERS);

    float fX = HUD_X + HUD_SPACING * nIndex;
    float fY = HUD_Y;

    // Set the backgrounds first
    m_arBackground[nIndex].SetPosition(fX + HUD_BACKGROUND_OFF_X,
                                       fY + HUD_BACKGROUND_OFF_Y);
    m_arBackground[nIndex].SetDimensions(HUD_BACKGROUND_WIDTH, 
                                         HUD_BACKGROUND_HEIGHT);
    
    // Percents next
    m_arPercents[nIndex].SetPosition(fX + HUD_PERCENT_OFF_X,
                                     fY + HUD_PERCENT_OFF_Y);
    m_arPercents[nIndex].SetScale(HUD_PERCENT_SCALE_X,
                                  HUD_PERCENT_SCALE_Y);

    // Lastly the bubbles
    for (int i = 0; i < SPIRIT_STARTING_LIVES; i++)
    {
        m_arLifeBubbles[nIndex][i].SetPosition(fX + HUD_BUBBLE_OFF_X + i*HUD_BUBBLE_SPACING,
                                               fY + HUD_BUBBLE_OFF_Y);
        m_arLifeBubbles[nIndex][i].SetDimensions(HUD_BUBBLE_WIDTH,
                                                 HUD_BUBBLE_HEIGHT);
    }
}

void HUD::SetTextures(int nIndex)
{
    assert(nIndex >= 0);
    assert(nIndex < MAX_PLAYERS);

    m_arBackground[nIndex].SetTexture(g_pCircleTex);
    
    for (int i = 0; i < SPIRIT_STARTING_LIVES; i++)
    {
        m_arLifeBubbles[nIndex][i].SetTexture(g_pCircleTex);
    }
}