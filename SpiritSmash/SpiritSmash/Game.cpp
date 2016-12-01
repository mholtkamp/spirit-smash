#include "Game.h"
#include "Spirit.h"
#include "Constants.h"
#include "ForestField.h"
#include "Timer.h"
#include "VInput.h"
#include "Assets.h"
#include <assert.h>

#include "Vakz.h"
#include <stdio.h>

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

    switch (nField)
    {
    case FIELD_TYPE_FOREST:
        m_pField = new ForestField();
        break;
    default:
        break;
    }

    m_pField->Generate();

    for (i = 0; i < nPlayers; i++)
    {
        m_arSpirit[i] = new Spirit();
        m_arSpirit[i]->SetPlayerIndex(i);
    }

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

    // After all players have moved, resolve their attacks
    ResolveAttacks();
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

void Game::ResolveAttacks()
{
    Spirit* pAttacker = 0;
    Spirit* pTarget = 0;

    // For each spirit, check if it is attacking.
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (m_arSpirit[i] != 0 &&
            m_arSpirit[i]->IsAttacking())
        {
            // This spirit is attacking, so find all spirits that 
            // overlap its attack volume.
            pAttacker = m_arSpirit[i];

            for (int j = 0; j < MAX_PLAYERS; j++)
            {
                pTarget = m_arSpirit[j];

                if (pTarget != 0 &&
                    pTarget != pAttacker &&
                    pAttacker->GetAttackVolume()->HasHitPlayer(j) == 0 &&
                    pAttacker->GetAttackVolume()->GetMatter()->Overlaps(pTarget->GetMatter()))
                {
                    // The attacking spirit has hit another spirit.
                    if (pTarget->IsAttacking() &&
                        pTarget->GetAttackVolume()->GetMatter()->Overlaps(pAttacker->GetMatter()))
                    {
                        // The target spirit also hit the attacking spirit, so determine who will 
                        // get hit based on priority.
                        if (pTarget->GetAttackVolume()->GetPriority() <= pAttacker->GetAttackVolume()->GetPriority())
                        {
                            // The target had lower attack priority. Hit the player
                            pTarget->ApplyHit(pAttacker->GetPosition(),
                                              pAttacker->GetDamage());

                            pAttacker->GetAttackVolume()->SetHitPlayer(j);
                        }
                        else
                        {
                            // The target had higher priority... so really the target is the attacker.
                            // Oh how the tables have turned... The attacker will handle the hit application
                        }
                    }
                    else
                    {
                        // The target isn't attacking or its attack isn't hitting the attacker.
                        // So apply a hit to the target.
                        pTarget->ApplyHit(pAttacker->GetPosition(),
                                          pAttacker->GetDamage());

                        pAttacker->GetAttackVolume()->SetHitPlayer(j);
                    }
                }
            }
        }
    }


}