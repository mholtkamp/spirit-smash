#include "Vakz.h"
#include "Scene.h"
#include "VGL.h"
#include "VInput.h"
#include "Log.h"
#include "Timer.h"

#include "Constants.h"
#include "Game.h"
#include "Menu.h"
#include "Assets.h"


#include <stdio.h>
#include <math.h>

int main()
{
	SetFullScreen(1);
	Initialize();
    SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Load all game assets 
    LoadAssets();

    Menu* pMenu = new Menu();
    Game* pGame = Game::CreateInstance();

    // Start a game with X player on the forest field
    pMenu->SetCurrentScene();
    pGame->Start(4, FIELD_TYPE_FOREST);

	while ((GetStatus() & VAKZ_QUIT) == 0)
	{
		// Vakz Update
		Update();

        pGame->Update();

        // Vakz Render
		Render();
	}

    Game::DestroyInstance();

    DestroyAssets();

	exit(0);
}
