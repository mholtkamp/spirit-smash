#include "Vakz.h"
#include "Scene.h"
#include "VGL.h"
#include "VInput.h"
#include "Log.h"
#include "Timer.h"

#include "Constants.h"
#include "Game.h"
#include "Assets.h"


#include <stdio.h>
#include <math.h>


int main()
{
	SetFullScreen(1);
	Initialize();

    // Load all game assets 
    LoadAssets();

    Game* pGame = Game::CreateInstance();

    // Start a game with X player on the forest field
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
