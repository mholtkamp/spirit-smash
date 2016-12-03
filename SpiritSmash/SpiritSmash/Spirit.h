#ifndef SPIRIT_H
#define SPIRIT_H

#include "AttackVolume.h"
#include "Orb.h"

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
        ANIM_ATTACK     = 5,
        NUM_ANIM_STATES = 6
    };

    enum Action
    {
        ACTION_NONE      = 0,
        ACTION_ATTACK    = 1,
        ACTION_CHARGE    = 2,
        ACTION_RELEASE   = 3,
        NUM_ACTIONS      = 4
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

    void ApplyHit(float* arInstigatorPos,
                  int nDamage);

    void ApplyDamage(int nDamage);

    int IsAlive();
    int IsAttacking();
    int GetDamage();
    int GetDirection();

    void SetPosition(float fX, 
                     float fY,
                     float fZ);

    int HasControl();
    void Kill();

    Orb* GetOrbArray();

    Matter* GetMatter();

    AttackVolume* GetAttackVolume();

private:

    static void CreateSharedCollider();

    void Update_Kinematics();
    void Update_Orientation();
    void Update_Attack();
    void Update_Charge();
    void Update_Release();
    void Update_Animation();
    void Update_Orbs();

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
    int m_nDamage;

    int m_nGrounded;

    float m_fSpeed;
    float m_fAcceleration;

    // Frames since jump has been pressed.
    int m_nJumpPressed;
    int m_nJustJumped;

    int m_nAlive;
    int m_nAction;
    int m_nJumpCharges;

    float m_fXVelocity;
    float m_fYVelocity;

    int m_nDirection;

    int m_nAnimState;

    float m_fAttackTime;

    AttackVolume m_attackVolume;

    Orb m_arOrbs[SPIRIT_MAX_ORBS];
    int m_nWorkingOrb;

    // Shared collider. Always same size for all spirits
    static OrientedBoxCollider* s_pSpiritCollider;



};

#endif