#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Camera.h"

class CameraController
{
public:

    CameraController();
    ~CameraController();

    void SetCamera(Camera* pCamera);

    void Update();

    void Update_Debug();
    
    void AdjustAdaptiveCamera();


private:

    void FindCenterPosition(float* arSum);

    float CalculateCameraZ(float* arCenter);

    int m_nDebugMode;

    Camera* m_pCamera;

    float m_arDebugPosition[3];
};



#endif