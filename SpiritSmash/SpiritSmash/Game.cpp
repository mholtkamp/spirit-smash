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

static float COLOR_1[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
static float COLOR_2[4] = { 1.0f, 0.2f, 0.2f, 1.0f };
static float COLOR_3[4] = { 0.2f, 1.0f, 0.2f, 1.0f };
static float COLOR_4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static float* COLORS[4] = { COLOR_1, COLOR_2, COLOR_3, COLOR_4 };

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

    m_pHUD = 0;

    m_pScene = new Scene();
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

void Game::Reset()
{
    // Reset ALL the data. 

    m_nState = STATE_PLAYING;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (m_arSpirit[i] != 0)
        {
            m_arSpirit[i]->Reset();
        }
    }
    m_pField->Reset();
    m_pHUD->Reset();

    // Reset the timer to make sure there is no crazy jumps
    m_timer.Start();
    m_fDeltaTime = 0.0f;

    m_nDebugMode = 0;

    m_quadBlack.SetVisible(0);
    m_textWinner.SetVisible(0);
}

void Game::SetCurrentScene()
{
    SetScene(m_pScene);
}

void Game::Start(int nPlayers,
                 int nField)
{
    int i = 0;

    assert(nPlayers >= 2);
    assert(nPlayers <= MAX_PLAYERS);

    m_nNumPlayers = nPlayers;

    // First generate the battlefield
    if (m_pField == 0)
    {
        switch (nField)
        {
        case FIELD_TYPE_FOREST:
            m_pField = new ForestField();
            break;
        default:
            break;
        }

        m_pField->Generate();
    }


    // Create the spirits that will be controlled by players
    for (i = 0; i < nPlayers; i++)
    {
        if (m_arSpirit[i] == 0)
        {
            m_arSpirit[i] = new Spirit();
            m_arSpirit[i]->SetPlayerIndex(i);
        }
        m_pField->SetStartPosition(m_arSpirit[i], i);
    }

    // Setup the HUD that will display lives/percents
    if (m_pHUD == 0)
    {
        m_pHUD = new HUD();
    }
    m_pHUD->Reset();

    // Initialize gameover glyphs owned by Game
    InitializeGlyphs();

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
        if (m_arSpirit[i] != 0)
        {
            delete m_arSpirit[i];
            m_arSpirit[i] = 0;
        }
    }

    if (m_pField != 0)
    {
        delete m_pField;
        m_pField = 0;
    }

    if (m_pHUD != 0)
    {
        delete m_pHUD;
        m_pHUD = 0;
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

        // After all players have moved, resolve their attacks
        ResolveAttacks();

        // Then resolve orbs
        ResolveOrbs();

        // Check if we should display gameover screen.
        CheckGameOver();
    }
    else if (m_nState == STATE_GAMEOVER)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (IsControllerConnected(i) &&
                IsControllerButtonJustDown(VCONT_START, i))
            {
                m_nState = STATE_FINISHED;
            }
        }
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

void Game::ResolveOrbs()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Spirit* pOwner = m_arSpirit[i];

        if (pOwner == 0)
        {
            // this spirit doesn't exist.
            continue;
        }

        Orb* arOrbs = pOwner->GetOrbArray();

        for (int j = 0; j < SPIRIT_MAX_ORBS; j++)
        {
            Orb* pOrb = &arOrbs[j];

            if (pOrb->GetState() == Orb::ORB_LAUNCHED)
            {
                // The orb is being launched so we gotta check if it overlaps 
                // any spirits first
                int nHit = 0;

                for (int k = 0; k < MAX_PLAYERS; k++)
                {
                    if (k == i)
                    {
                        // Do not check if the player's own orb overlaps with themselves.
                        continue;
                    }

                    if (m_arSpirit[k] == 0)
                    {
                        // This player doesn't exist
                        continue;
                    }
                    
                    // Check if the orb overlaps the other player's spirit
                    if (pOrb->GetMatter()->Overlaps(m_arSpirit[k]->GetMatter()))
                    {
                        // It does! Let the orb calculate the way in which
                        // it applies a hit to the spirit.
                        pOrb->ApplyHit(m_arSpirit[k]);

                        nHit = 1;
                    }
                }

                // If the orb hit any player, deactivate it
                if (nHit != 0)
                {
                    pOrb->SetState(Orb::ORB_INACTIVE);
                }
            }
        }
    }
}

Field* Game::GetField()
{
    return m_pField;
}

HUD* Game::GetHUD()
{
    return m_pHUD;
}

void Game::InitializeGlyphs()
{
    // Set up the winner quad/text
    m_quadBlack.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_quadBlack.SetDimensions(2.0f, 2.0f);
    m_quadBlack.SetPosition(-1.0f, -1.0f);
    m_quadBlack.SetVisible(0);
    m_textWinner.SetPosition(WINNER_X, 
                             WINNER_Y);
    m_textWinner.SetScale(WINNER_TEXT_SCALE_X,
                          WINNER_TEXT_SCALE_Y);
    m_textWinner.SetVisible(0);

    m_pScene->AddGlyph(&m_quadBlack);
    m_pScene->AddGlyph(&m_textWinner);
}

void Game::SetGameOver(Spirit* pWinner)
{
    m_nState = STATE_GAMEOVER;

    m_quadBlack.SetVisible(1);
    m_textWinner.SetVisible(1);

    char arBuffer[32] = { 0 };
    if (pWinner != 0)
    {
        sprintf(arBuffer, "Player %d Wins!", pWinner->GetPlayerIndex() + 1);
    }
    else
    {
        sprintf(arBuffer, "DRAW");
    }
    
    m_textWinner.SetText(arBuffer);
    if (pWinner != 0)
    {
        m_textWinner.SetColor(COLORS[pWinner->GetPlayerIndex()]);
    }
    else
    {
        m_textWinner.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

int Game::IsFinished()
{
    return (m_nState == STATE_FINISHED);
}

void Game::CheckGameOver()
{
    int nRemaining = 0;

    Spirit* pSpirit = 0;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (m_arSpirit[i] != 0 &&
            !m_arSpirit[i]->IsEliminated())
        {
            nRemaining++;
            pSpirit = m_arSpirit[i];
        }
    }

    if (nRemaining <= 1)
    {
        SetGameOver(pSpirit);
    }
}