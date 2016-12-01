#ifndef ASSETS_H
#define ASSETS_H

#include "StaticMesh.h"
#include "Texture.h"
#include "AnimatedMesh.h"
#include "DiffuseMaterial.h"
#include "ToonMaterial.h"
#include "FullbrightMaterial.h"
#include "Sound.h"

// Texture assets
extern Texture* g_pSpirit1Tex;
extern Texture* g_pSpirit2Tex;
extern Texture* g_pSpirit3Tex;
extern Texture* g_pSpirit4Tex;
extern Texture* g_pPlatformEndTex;
extern Texture* g_pPlatformMid1Tex;
extern Texture* g_pPlatformMid2Tex;
extern Texture* g_pSkysphereTex;

// Static Mesh assets
extern StaticMesh* g_pPlatformLeftMesh;
extern StaticMesh* g_pPlatformMidMesh;
extern StaticMesh* g_pPlatformRightMesh;
extern StaticMesh* g_pSkysphereMesh;

// Animated Mesh assets
extern AnimatedMesh* g_pSpiritAnimMesh;

// Shared Materials
extern Material*           g_pDefaultMaterial;
extern DiffuseMaterial*    g_pDefaultDiffuseMaterial;
extern ToonMaterial*       g_pDefaultToonMaterial;
extern FullbrightMaterial* g_pDefaultFullbrightMaterial;

void LoadAssets();

void DestroyAssets();

#endif