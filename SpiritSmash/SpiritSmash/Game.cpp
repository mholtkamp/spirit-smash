#include "Game.h"
#include "Spirit.h"
#include "Constants.h"
#include "ForestField.h"
#include "Timer.h"
#include "VInput.h"
#include "Assets.h"
#include <assert.h>
#include "Vakz.h"

Game* Game::s_pInstance = 0;

Game* Game::CreateInstance()
{
    if (s_pInstance == 0)
    {
        s_pInstance = new Game();
    }

    return s_pInstance;
}

void Game::DestroyInstance()
{
    if (s_pInstance != 0)
    {
        delete s_pInstance;
        s_pInstance = 0;
    }
}

Game* Game::GetInstance()
{
    return s_pInstance;
}

Game::Game()
{
    m_pField = 0;

    m_arSpirit[0] = 0;
    m_arSpirit[1] = 0;
    m_arSpirit[2] = 0;
    m_arSpirit[3] = 0;

    m_pScene = new Scene();
    SetScene(m_pScene);
    m_pScene->SetAmbientLight(LIGHTING_AMBIENT_INTENSITY, 
                              LIGHTING_AMBIENT_INTENSITY, 
                              LIGHTING_AMBIENT_INTENSITY, 
                              LIGHTING_AMBIENT_INTENSITY);

    // Create camera
    m_pCamera = new Camera();
    m_pCamera->SetProjectionType(Camera::CAMERA_PERSPECTIVE);
    m_pCamera->SetPosition(0.0f, 0.0f, CAMERA_Z_OFFSET);
    m_pScene->SetCamera(m_pCamera);

    // Assign camera to camera controller
    m_cameraController.SetCamera(m_pCamera);
    
    m_fDeltaTime = 0.0f;

    m_nState = STATE_PLAYING;
}

Game::~Game()
{
    if (m_pScene != 0)
    {
        delete m_pScene;
        m_pScene = 0;
    }

    if (m_pCamera != 0)
    {
        delete m_pCamera;
        m_pCamera = 0;
    }    
}

void Game::Start(int nPlayers,
                 int nField)
{
    int i = 0;

    assert(nPlayers >= 0);
    assert(nPlayers < MAX_PLAYERS);

    m_nNumPlayers = nPlayers;

    for (i = 0; i < nPlayers; i++)
    {
        m_arSpirit[i] = new Spirit();
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

    m_pField->Generate();

    // Disable any test rendering
    Matter::SetGlobalColliderRenderingEnable(0);

    // Begin frame timer 
    m_timer.Start();
}

void Game::End()
{
    int i = 0;

    for (i = 0; i < m_nNumPlayers; i++)
    {
        delete m_arSpirit[i];
        m_arSpirit[i] = 0;
    }

    if (m_pField != 0)
    {
        delete m_pField;
        m_pField = 0;
    }
}

void Game::Update()
{
    assert(m_nState > STATE_INVALID);
    assert(m_nState < NUM_GAME_STATES);

    int i = 0;

    m_timer.Stop();
    m_fDeltaTime = m_timer.Time();
    m_timer.Start();

    if (IsKeyJustDown(VKEY_Z))
    {
        m_nDebugMode = !m_nDebugMode;

        if (m_nDebugMode != 0)
        {
            m_pScene->EnableMatterOctreeRendering();
            Matter::SetGlobalColliderRenderingEnable(1);
        }
        else
        {
            m_pScene->DisableMatterOctreeRendering();
            Matter::SetGlobalColliderRenderingEnable(0);
        }
    }

    if (m_nState == STATE_PLAYING)
    {
        for (i = 0; i < m_nNumPlayers; i++)
        {
            m_arSpirit[i]->Update();
        }

        m_pField->Update();

        m_cameraController.Update();
    }
}

Spirit* Game::GetSpirit(int nPlayerIndex)
{
    assert(nPlayerIndex >= 0);
    assert(nPlayerIndex < MAX_PLAYERS);

    return m_arSpirit[nPlayerIndex];
}

Scene* Game::GetScene()
{
    return m_pScene;
}

Camera* Game::GetCamera()
{
    return m_pCamera;
}

float Game::DeltaTime()
{
    return m_fDeltaTime;
}

int Game::InDebugMode()
{
    return m_nDebugMode;
}