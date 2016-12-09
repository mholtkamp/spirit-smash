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

enum GameState
{
    STATE_MENU = 0,
    STATE_GAME = 1
};

static int s_nGameState = STATE_MENU;

int main()
{
	SetFullScreen(1);
	Initialize();
    SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Load all game assets 
    LoadAssets();

    Menu* pMenu = new Menu();
    Game* pGame = Game::CreateInstance();

    pMenu->SetCurrentScene();

	while ((GetStatus() & VAKZ_QUIT) == 0)
	{
		// Vakz Update
		Update();

        // Update based on state
        switch (s_nGameState)
        {
        case STATE_MENU:
        {
            int nStart = pMenu->Update();

            if (nStart != 0)
            {
                pGame->Start(pMenu->GetNumPlayers(), FIELD_TYPE_FOREST);
                pGame->SetCurrentScene();
                s_nGameState = STATE_GAME;
            }
            break;
        }
        case STATE_GAME:
            pGame->Update();

            if (pGame->IsFinished())
            {
                s_nGameState = STATE_MENU;
                pGame->Reset();

                // There seems to be only a very small video memory leak
                // when destroying a game, and maybe a very small
                // system memory leak too, however I didn't verify this.
                // Destroying and reinstantiating the game singleton 
                // seems to be the safest way of restoring the state
                // for a new game.
                Game::DestroyInstance();
                pGame = Game::CreateInstance();
                pMenu->SetCurrentScene();
            }
            break;
        }
        
        // Vakz Render
		Render();
	}

    Game::DestroyInstance();

    DestroyAssets();

	exit(0);
}
