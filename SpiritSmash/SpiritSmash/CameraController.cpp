#include "CameraController.h"
#include "VInput.h"
#include "Game.h"
#include "Spirit.h"
#include "Constants.h"

#include <assert.h>


CameraController::CameraController()
{
    m_nDebugMode = 0;
    m_pCamera    = 0;

    m_arDebugPosition[0] = 0.0f;
    m_arDebugPosition[1] = 0.0f;
    m_arDebugPosition[2] = CAMERA_Z_OFFSET;
}

CameraController::~CameraController()
{

}

void CameraController::SetCamera(Camera* pCamera)
{
    assert(pCamera != 0);

    m_pCamera = pCamera;
}

void CameraController::Update()
{
    // Check if debug mode is being toggled.
    if (IsKeyJustDown(VKEY_C))
    {
        m_nDebugMode = !m_nDebugMode;
    }

    // Perform positional updates on camera
    if (m_nDebugMode != 0)
    {
        Update_Debug();
    }
    else
    {
        AdjustAdaptiveCamera();
    }
}

void CameraController::Update_Debug()
{
    float fDeltaTime = Game::GetInstance()->DeltaTime();
    float fDistance = DEBUG_CAMERA_SPEED * fDeltaTime;

    if (IsKeyDown(VKEY_LEFT))
        m_arDebugPosition[0] -= fDistance;
    if (IsKeyDown(VKEY_RIGHT))
        m_arDebugPosition[0] += fDistance;
    if (IsKeyDown(VKEY_DOWN))
        m_arDebugPosition[1] -= fDistance;
    if (IsKeyDown(VKEY_UP))
        m_arDebugPosition[1] += fDistance;
    if (IsKeyDown(VKEY_I))
        m_arDebugPosition[2] -= fDistance;
    if (IsKeyDown(VKEY_O))
        m_arDebugPosition[2] += fDistance;

    m_pCamera->SetPosition(m_arDebugPosition[0],
                           m_arDebugPosition[1],
                           m_arDebugPosition[2]);
}

void CameraController::AdjustAdaptiveCamera()
{
    // For now, just lock onto player 1
    Spirit* pPlayer1 = Game::GetInstance()->GetSpirit(0);

    float* arPos = pPlayer1->GetPosition();

    m_pCamera->SetPosition(arPos[0],
                           arPos[1],
                           CAMERA_Z_OFFSET);
}