#ifndef ASSETS_H
#define ASSETS_H

#include "StaticMesh.h"
#include "Texture.h"
#include "AnimatedMesh.h"
#include "DiffuseMaterial.h"
#include "ToonMaterial.h"
#include "Sound.h"

// Texture assets
extern Texture* g_pSpiritTex;
extern Texture* g_pPlatformEndTex;
extern Texture* g_pPlatformMid1Tex;
extern Texture* g_pPlatformMid2Tex;

// Static Mesh assets
extern StaticMesh* g_pPlatformLeftMesh;
extern StaticMesh* g_pPlatformMidMesh;
extern StaticMesh* g_pPlatformRightMesh;
extern StaticMesh* g_pSkysphereMesh;

// Animated Mesh assets
extern AnimatedMesh* g_pSpiritAnimMesh;

// Shared Materials
extern ToonMaterial*    g_pDefaultMaterial;

void LoadAssets();

void DestroyAssets();

#endif