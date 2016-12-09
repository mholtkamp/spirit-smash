#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "Spirit.h"
#include "HUD.h"
#include "CameraController.h"
#include "Scene.h"
#include "Timer.h"

class Game
{
    enum GameState
    {
        STATE_INVALID   = -1,
        STATE_WAITING   = 0,
        STATE_PLAYING   = 1,
        STATE_GAMEOVER  = 2,
        STATE_FINISHED  = 3,
        STATE_PAUSED    = 4,
        NUM_GAME_STATES = 5
    };

public:

    static Game* CreateInstance();
    static void DestroyInstance();
    static Game* GetInstance();

    ~Game();

    void Update();

    void ResolveAttacks();

    void ResolveOrbs();

    Spirit* GetSpirit(int nPlayerIndex);

    Field* GetField();

    HUD* GetHUD();

    void Start(int nPlayers,
               int nField);

    void End();

    float DeltaTime();

    Scene* GetScene();

    Camera* GetCamera();

    int InDebugMode();

    void InitializeGlyphs();

    void SetCurrentScene();

    void SetGameOver(Spirit* pWinner);

    int IsFinished();

private:

    Game();

    void CheckGameOver();

    static Game* s_pInstance;

    Scene* m_pScene;
    Camera* m_pCamera;

    Field*  m_pField;
    Spirit* m_arSpirit[4];
    HUD* m_pHUD;
    int m_nNumPlayers;
    int m_nState;

    Timer m_timer;
    float m_fDeltaTime;

    int m_nDebugMode;

    Quad m_quadBlack;
    Text m_textWinner;

    CameraController m_cameraController;
};

#endif