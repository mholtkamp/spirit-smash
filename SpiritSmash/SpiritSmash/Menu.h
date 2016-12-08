#ifndef MENU_H
#define MENU_H

#include "Scene.h"
#include "Quad.h"
#include "Text.h"
#include "ParticleSystem.h"

class Menu
{
public:

    Menu();
    ~Menu();

    int Update();

    int GetNumPlayers();

    void SetCurrentScene();

private:

    void InitializeCamera();
    void InitializeGlyphs();
    void InitializeParticles();
    void RegisterScene();

    int m_nNumPlayers;

    Scene m_scene;
    Camera m_camera;
    Quad m_quadTitle;
    Text m_textPlayers;
    ParticleSystem m_arParticles[4];

    // This particle is necessary to prevent a bug with Vakz that 
    // causes the first particle being rendered to update very slowly.
    // Has to be some incorrect state setup, but I don't have time to fix 
    // it right now :[
    ParticleSystem m_testParticle;
};

#endif