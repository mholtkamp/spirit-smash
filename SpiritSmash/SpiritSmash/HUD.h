#ifndef HUD_H
#define HUD_H

#include "Constants.h"
#include "Text.h"
#include "Quad.h"

class HUD
{
public:

    HUD();
    ~HUD();

    void InitializePosition(int nIndex);

    void RegisterGlyphs(int nIndex);

    void SetPercent(int nPlayerIndex,
                    int nPercent);

    void SetLives(int nPlayerIndex,
                  int nLives);

    void SetPlayerEnable(int nIndex,
                         int nEnable);

    void SetColors(int nIndex,
                   float* arColor);

    void SetTextures(int nIndex);

    void Reset();

private:

    Text m_arPercents[MAX_PLAYERS];
    Quad m_arLifeBubbles[MAX_PLAYERS][SPIRIT_STARTING_LIVES];
    Quad m_arBackground[MAX_PLAYERS];
    int m_arPlayerEnabled[MAX_PLAYERS];
};

#endif