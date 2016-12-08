#include "Menu.h"
#include "VInput.h"
#include "Constants.h"
#include "Assets.h"
#include "Vakz.h"

static float COLOR_1_MIN[4] = { 0.2f, 0.2f, 1.0f, 0.2f };
static float COLOR_2_MIN[4] = { 1.0f, 0.2f, 0.2f, 0.2f };
static float COLOR_3_MIN[4] = { 0.2f, 1.0f, 0.2f, 0.2f };
static float COLOR_4_MIN[4] = { 0.4f, 0.4f, 0.4f, 0.2f };
static float* PARTICLE_MIN_COLORS[4] = { COLOR_1_MIN, COLOR_2_MIN, COLOR_3_MIN, COLOR_4_MIN };

static float COLOR_1_MAX[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
static float COLOR_2_MAX[4] = { 1.0f, 0.2f, 0.2f, 1.0f };
static float COLOR_3_MAX[4] = { 0.2f, 1.0f, 0.2f, 1.0f };
static float COLOR_4_MAX[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static float* PARTICLE_MAX_COLORS[4] = { COLOR_1_MAX, COLOR_2_MAX, COLOR_3_MAX, COLOR_4_MAX };

Menu::Menu()
{
    m_nNumPlayers = MAX_PLAYERS;

    InitializeCamera();
    InitializeParticles();
    InitializeGlyphs();
    RegisterScene();
}

Menu::~Menu()
{

}

int Menu::Update()
{
    int nRet = 0;

    // Check if any of the controllers hit left or right to
    // change the player count.
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsControllerButtonJustDown(i, VCONT_R1))
        {
            m_nNumPlayers++;
        }
        else if (IsControllerButtonJustDown(i, VCONT_L1))
        {
            m_nNumPlayers--;
        }

        if (IsControllerButtonJustDown(i, VCONT_START))
        {
            nRet = 1;
        }

        // Clamp num players
        if (m_nNumPlayers < MIN_PLAYERS)
        {
            m_nNumPlayers = MIN_PLAYERS;
        }

        if (m_nNumPlayers > MAX_PLAYERS)
        {
            m_nNumPlayers = MAX_PLAYERS;
        }
    }

    return nRet;
}

int Menu::GetNumPlayers()
{
    return m_nNumPlayers;
}

void Menu::RegisterScene()
{
    m_scene.AddGlyph(&m_quadTitle);
    m_scene.AddGlyph(&m_textPlayers);

    // Add actor particles
    m_scene.AddActor(&m_testParticle);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_scene.AddActor(&m_arParticles[i]);
    }
}

void Menu::InitializeCamera()
{
    m_camera.SetPosition(2.0f, 1.0f, 5.0f);
    m_camera.SetRotation(0.0f, 0.0f, 0.0f);
    m_scene.SetCamera(&m_camera);
}

void Menu::InitializeParticles()
{
    float arGravity[3] = { 0.0f, 0.0f, 0.0f };
    float arMinVelocity[3] = { 0.0f, TITLE_PARTICLES_MIN_YVEL, 0.0f };
    float arMaxVelocity[3] = { 0.0f, TITLE_PARTICLES_MAX_YVEL, 0.0f };

    m_testParticle.SetParticleCount(1);
    m_testParticle.SetOrigin(-999.0f, -999.0f, 999.0f);
    m_testParticle.Initialize();

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_arParticles[i].SetGenerate(1);
        m_arParticles[i].SetParticleCount(TITLE_PARTICLES_COUNT);
        m_arParticles[i].SetVelocity(arMinVelocity, arMaxVelocity);
        m_arParticles[i].SetGravity(arGravity);
        m_arParticles[i].SetColor(PARTICLE_MIN_COLORS[i], PARTICLE_MAX_COLORS[i]);

        m_arParticles[i].SetOrigin(TITLE_PARTICLES_X + TITLE_PARTICLES_SPACING*i, 0.0f, 0.0f);
        m_arParticles[i].SetLifetime(TITLE_PARTICLES_MIN_LIFETIME, TITLE_PARTICLES_MAX_LIFETIME);
        m_arParticles[i].SetSize(5.0f, 10.0f);
        m_arParticles[i].Initialize();
    }
}

void Menu::InitializeGlyphs()
{
    m_quadTitle.SetPosition(TITLE_X,
                            TITLE_Y);
    m_quadTitle.SetDimensions(TITLE_WIDTH,
                              TITLE_HEIGHT);
    m_quadTitle.SetTexture(g_pTitleTex);

    m_textPlayers.SetText("Players: 4");
    m_textPlayers.SetPosition(NUM_PLAYERS_X,
                              NUM_PLAYERS_Y);
    m_textPlayers.SetScale(0.6f, 1.0f);
    m_textPlayers.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Menu::SetCurrentScene()
{
    SetScene(&m_scene);
}