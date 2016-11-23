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
extern Texture* g_pPlatformMid1;
extern Texture* g_pPlatformMid2;

// Static Mesh assets
extern StaticMesh* g_pPlatformEndMesh;
extern StaticMesh* g_pPlatformMidMesh;
extern StaticMesh* g_pSkysphereMesh;

// Animated Mesh assets
extern AnimatedMesh* g_pSpiritAnimMesh;

// Shared Materials
extern DiffuseMaterial* g_pDefaultDiffuseMaterial;

void LoadAssets();

void DestroyAssets();

#endif