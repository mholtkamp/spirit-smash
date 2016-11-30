#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "Spirit.h"
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
        STATE_FINISHED  = 2,
        STATE_PAUSED    = 3,
        NUM_GAME_STATES = 4
    };

public:

    static Game* CreateInstance();
    static void DestroyInstance();
    static Game* GetInstance();

    ~Game();

    void Update();

    Spirit* GetSpirit(int nPlayerIndex);

    Field* GetField();

    void Start(int nPlayers,
               int nField);

    void End();

    float DeltaTime();

    Scene* GetScene();

    Camera* GetCamera();

    int InDebugMode();

private:

    Game();

    static Game* s_pInstance;

    Scene* m_pScene;
    Camera* m_pCamera;

    Field*  m_pField;
    Spirit* m_arSpirit[4];
    int m_nNumPlayers;
    int m_nState;

    Timer m_timer;
    float m_fDeltaTime;

    int m_nDebugMode;

    CameraController m_cameraController;
};

#endif