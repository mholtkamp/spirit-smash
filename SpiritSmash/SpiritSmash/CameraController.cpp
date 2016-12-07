#include "CameraController.h"
#include "VInput.h"
#include "Game.h"
#include "Spirit.h"
#include "Constants.h"

#include <math.h>
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

    float arSumPos[3] = { 0.0f, 0.0f, 0.0f};
    float fCamZ = 0.0f;

    FindCenterPosition(arSumPos);

    fCamZ = CalculateCameraZ(arSumPos);

    m_pCamera->SetPosition(arSumPos[0],
                           arSumPos[1],
                           fCamZ);
}

void CameraController::FindCenterPosition(float* arSumPos)
{
    int nSpirits = 0;
    Spirit* pSpirit = 0;
    float* arPos = 0;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        pSpirit = Game::GetInstance()->GetSpirit(i);

        if (pSpirit != 0 &&
            pSpirit->IsEliminated() == 0)
        {
            arPos = pSpirit->GetPosition();

            arSumPos[0] += arPos[0];
            arSumPos[1] += arPos[1];
            nSpirits++;
        }
    }

    arSumPos[0] /= nSpirits;
    arSumPos[1] /= nSpirits;
}

float CameraController::CalculateCameraZ(float* arCenter)
{
    float fZ = CAMERA_Z_OFFSET;
    float fMaxDist = 0.0f;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Spirit* pSpirit = Game::GetInstance()->GetSpirit(i);

        if (pSpirit != 0 &&
            pSpirit->IsEliminated() == 0)
        {
            float* arPos = pSpirit->GetPosition();
            
            float fDX = arPos[0] - arCenter[0];
            float fDY = arPos[1] - arCenter[1];

            float fDist = static_cast<float>(sqrt(fDX*fDX + fDY*fDY));

            if (fDist > fMaxDist)
            {
                fMaxDist = fDist;
            }
        }
    }

    fZ += fMaxDist * CAMERA_Z_SCALE_FACTOR;

    return fZ;
}