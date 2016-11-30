#include "Assets.h"
#include "Constants.h"

// Texture assets
Texture* g_pSpiritTex                       = 0;
Texture* g_pPlatformEndTex                  = 0;
Texture* g_pPlatformMid1Tex                 = 0;
Texture* g_pPlatformMid2Tex                 = 0;
Texture* g_pSkysphereTex = 0;

// Static Mesh assets
StaticMesh* g_pPlatformLeftMesh             = 0;
StaticMesh* g_pPlatformMidMesh              = 0;
StaticMesh* g_pPlatformRightMesh             = 0;
StaticMesh* g_pSkysphereMesh                = 0;

// Animated Mesh assets
AnimatedMesh* g_pSpiritAnimMesh             = 0;

// Shared Materials
Material*           g_pDefaultMaterial = 0;
DiffuseMaterial*    g_pDefaultDiffuseMaterial = 0;
ToonMaterial*       g_pDefaultToonMaterial = 0;
FullbrightMaterial* g_pDefaultFullbrightMaterial = 0;

// Assets loaded flag
static int s_nLoaded = 0;

void LoadAssets()
{
    if (s_nLoaded == 0)
    {
        // Textures 
        g_pSpiritTex = new Texture();
        g_pSpiritTex->LoadBMP("Data/Textures/spirit1_tex.bmp");

        g_pPlatformEndTex = new Texture();
        g_pPlatformEndTex->LoadBMP("Data/Textures/platform_end_tex.bmp");

        g_pPlatformMid1Tex = new Texture();
        g_pPlatformMid1Tex->LoadBMP("Data/Textures/platform_mid_1_tex.bmp");

        g_pPlatformMid2Tex = new Texture();
        g_pPlatformMid2Tex->LoadBMP("Data/Textures/platform_mid_2_tex.bmp");

        g_pSkysphereTex = new Texture();
        g_pSkysphereTex->LoadBMP("Data/Textures/skysphere_tex.bmp");

        // Static Meshes
        g_pPlatformLeftMesh = new StaticMesh();
        g_pPlatformLeftMesh->Load("Data/StaticMeshes/platform_left.obj");

        g_pPlatformMidMesh = new StaticMesh();
        g_pPlatformMidMesh->Load("Data/StaticMeshes/platform_mid.obj");

        g_pPlatformRightMesh = new StaticMesh();
        g_pPlatformRightMesh->Load("Data/StaticMeshes/platform_right.obj");

        g_pSkysphereMesh = new StaticMesh();
        g_pSkysphereMesh->Load("Data/StaticMeshes/skysphere.obj");

        // Animated Meshes
        g_pSpiritAnimMesh = new AnimatedMesh();
        g_pSpiritAnimMesh->Load("Data/AnimatedMeshes/AM_Spirit/spirit.amf");

        // Shared Materials
        g_pDefaultDiffuseMaterial = new DiffuseMaterial();
        g_pDefaultDiffuseMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

        g_pDefaultToonMaterial = new ToonMaterial();
        g_pDefaultToonMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        g_pDefaultToonMaterial->SetLightingIntervalCount(LIGHTING_INTERVALS);
        
        g_pDefaultFullbrightMaterial = new FullbrightMaterial();
        g_pDefaultFullbrightMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

        g_pDefaultMaterial = g_pDefaultDiffuseMaterial;

        s_nLoaded = 1;
    }
}

void DestroyAssets()
{
    // Textures 
    delete g_pSpiritTex;
    delete g_pPlatformEndTex;
    delete g_pPlatformMid1Tex;
    delete g_pPlatformMid2Tex;

    // Static Meshes
    delete g_pPlatformLeftMesh;
    delete g_pPlatformMidMesh;
    delete g_pPlatformRightMesh;
    delete g_pSkysphereMesh;

    // Animated Meshes
    delete g_pSpiritAnimMesh;
    
    // Shared Materials
    delete g_pDefaultMaterial;
}