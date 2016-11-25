#ifndef SPIRIT_H
#define SPIRIT_H

#include "Matter.h"

class Spirit
{
public:

	Spirit();
	~Spirit();

	void Update();

    void SetGame(class Game* pGame);

    void SetPlayerIndex(int nIndex);
    void SetLives(int nLives);
    void SetPercent(int nPercent);

    int GetPercent();
    int GetLives();
    int GetPlayerIndex();

    float* GetPosition();

    int IsAlive();
    void Kill();

private:

    // Reference to the game
    class Game* m_pGame;

	Matter m_matter;
    
    float m_arPosition[3];

    int m_nPlayerIndex;
    int m_nPercent;
    int m_nLives;

    float m_fSpeed;

    // Frames since jump has been pressed.
    int m_nJumpPressed;

    int m_nAlive;


};

#endif