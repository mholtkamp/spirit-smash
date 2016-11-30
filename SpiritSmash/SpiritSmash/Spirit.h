#ifndef SPIRIT_H
#define SPIRIT_H

#include "Matter.h"
#include "OrientedBoxCollider.h"

class Spirit
{
public:

    enum AnimState
    {
        ANIM_INVALID    = -1,
        ANIM_IDLE       = 0,
        ANIM_MOVE       = 1,
        ANIM_FALL       = 3,
        ANIM_CHARGE     = 4,
        NUM_ANIM_STATES = 5
    };

	Spirit();
	~Spirit();

	void Update();

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

    static void CreateSharedCollider();

    void Update_Kinematics();
    void Update_Orientation();
    void Update_Animation();
    void CheckJump();
    void CheckGrounded();
    void ApplyGravity();
    void ApplyDrag();
    void AssignProperTexture();

    // Reference to the game
    class Game* m_pGame;

	Matter m_matter;
    
    float m_arPosition[3];

    int m_nPlayerIndex;
    int m_nPercent;
    int m_nLives;

    int m_nGrounded;

    float m_fSpeed;
    float m_fAcceleration;

    // Frames since jump has been pressed.
    int m_nJumpPressed;
    int m_nJustJumped;

    int m_nAlive;

    float m_fXVelocity;
    float m_fYVelocity;

    int m_nDirection;

    int m_nAnimState;

    // Shared collider. Always same size for all spirits
    static OrientedBoxCollider* s_pSpiritCollider;

};

#endif