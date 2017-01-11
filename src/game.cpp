/*
 * game.cpp
 *
 *  Created on: 2013.07.05
 *      Author: Fredrick Lockert
 *  Updated on: 10 Jan 2017
 */

#include "game.hpp"
#include "board.hpp"

//=============================================================================
// Name: Game
// Desc: Tower of pimps.
//=============================================================================
CGame::CGame()
{
	m_pWindow       = NULL;
    m_pRenderer     = NULL;
	m_pBoard        = NULL;
    m_pFont         = NULL;
	m_WindowWidth   = 325;
	m_WindowHeight  = 400;
	m_StartTime     = 0;
	m_CurrentTime   = 0;
	m_LastTime      = 0;
	m_bQuit         = false;
	m_bPlayerTurn   = true;
	m_bAI           = false;
	m_GameState     = GAMESTATE_STARTUP;
}

//=============================================================================
// Name: ~Game
// Desc: Tower of netherrack.
//=============================================================================
CGame::~CGame()
{
	if(mMessageStartup != NULL)
		SDL_DestroyTexture(mMessageStartup);

	if(mMessagePlayerOneTurn != NULL)
		SDL_DestroyTexture(mMessagePlayerOneTurn);

	if(mMessagePlayerTwoTurn != NULL)
		SDL_DestroyTexture(mMessagePlayerTwoTurn);

	if(mMessagePlayerOneWon != NULL)
		SDL_DestroyTexture(mMessagePlayerOneWon);

	if(mMessagePlayerTwoWon != NULL)
		SDL_DestroyTexture(mMessagePlayerTwoWon);

	if(mMessageAIWon != NULL)
		SDL_DestroyTexture(mMessageAIWon);

    if(m_pFont != NULL)
        TTF_CloseFont(m_pFont);

	if(m_pBoard != NULL)
		delete m_pBoard;

    if(m_pRenderer != NULL)
        SDL_DestroyRenderer(m_pRenderer);

	if(m_pWindow != NULL)
		SDL_DestroyWindow(m_pWindow);

    TTF_Quit();

	SDL_Quit();
}

//=============================================================================
// Name: Initialise
// Desc: Initialises SDL, then loads all assets. Returns 0 on success, else -1
//       on failure.
//=============================================================================
int CGame::Initialise()
{
	int returnCode = SDL_Init(SDL_INIT_VIDEO);

	if(returnCode != 0)
	{
		fprintf(stderr, "ERROR: Unable to initialise: %s\n", SDL_GetError());
		return returnCode;
	}

    m_pWindow = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN);

	if(m_pWindow == NULL)
	{
		fprintf(stderr, "ERROR: Unable to create window: %s\n", SDL_GetError());
		return -1;
	}

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

    if(m_pRenderer == NULL)
    {
        fprintf(stderr, "ERROR: Unable to create renderer: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);

	m_pBoard = new CBoard();

	if(TTF_Init() != 0)
	{
		fprintf(stderr, "Error: Unable to initialise fonts: %s\n", TTF_GetError());
		return -1;
	}

    m_pFont = TTF_OpenFont("../data/assets/ammyshandwriting.ttf", 18);

    if(m_pFont == NULL)
    {
        fprintf(stderr, "Error: Unable to initialise fonts: %s\n", TTF_GetError());
        return -1;
    }

    mTextColour = {0, 0, 0};
	mMessageLocation = {15, 25, 0, 0};

	// Load assets into memory.
    m_pBoard->SetBackground(m_pRenderer, "../data/assets/background.bmp");
    m_pBoard->SetPlayer1(m_pRenderer, "../data/assets/x.bmp");
    m_pBoard->SetPlayer2(m_pRenderer, "../data/assets/o.bmp");

    SDL_Surface* temp = TTF_RenderText_Blended(m_pFont, "Press Ctrl+N to start a new game.", mTextColour);
    mMessageStartup = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Turn: Player One", mTextColour);
    mMessagePlayerOneTurn = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Turn: Player Two", mTextColour);
    mMessagePlayerTwoTurn = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Player One Won The Game!", mTextColour);
    mMessagePlayerOneWon = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Player Two Won The Game!", mTextColour);
    mMessagePlayerTwoWon = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "AI Won The Game!", mTextColour);
    mMessageAIWon = SDL_CreateTextureFromSurface(m_pRenderer, temp);

	SDL_FreeSurface(temp);

	return 0;
}

//=============================================================================
// Name: Run
// Desc: Game loop, keeps track of time and renders the game and what not.
//=============================================================================
int CGame::Run()
{
	m_pBoard->NewBoard();

	m_StartTime = SDL_GetTicks();
	m_LastTime = SDL_GetTicks();

	while(!m_bQuit)
	{
		m_CurrentTime = SDL_GetTicks();

		SDL_Event event;
		if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				m_bQuit = true;

			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					if(m_GameState == GAMESTATE_INPROGRESS)
					{
						if((m_bPlayerTurn == true && m_bAI == true) || (m_bPlayerTurn == false && m_bAI == false) || (m_bPlayerTurn == true && m_bAI == false))
						{
							if(m_pBoard->Update(m_bPlayerTurn, event.button.x, event.button.y) == 1)
							{
								if(m_bPlayerTurn)
									m_bPlayerTurn = false;
								else
									m_bPlayerTurn = true;

								if(m_pBoard->GameWon(true) == GAMESTATE_PLAYERWON)
									m_GameState = GAMESTATE_PLAYERWON;
								else if(m_pBoard->GameWon(false) == GAMESTATE_AIWON)
									m_GameState = GAMESTATE_AIWON;
							}
						}
					}
				}
			}

			if(m_GameState == GAMESTATE_INPROGRESS)
			{
				if(m_bPlayerTurn == false && m_bAI == true)
				{
					m_pBoard->SimulateAI();

					m_bPlayerTurn = true;

					if(m_pBoard->GameWon(false) == GAMESTATE_AIWON)
						m_GameState = GAMESTATE_AIWON;
				}
			}

            if(event.type == SDL_KEYUP)
			{
				if(event.key.keysym.mod == KMOD_LCTRL)
				{
					// New game.
					if(event.key.keysym.sym == SDLK_n)
					{
						m_pBoard->NewBoard();
						m_bPlayerTurn = true;
						m_GameState = GAMESTATE_INPROGRESS;
					}
					// Turn AI on or off.
					else if(event.key.keysym.sym == SDLK_a)
					{
                        if(m_bAI == true)
							m_bAI = false;
						else
							m_bAI = true;
					}
					// Quits the application.
					else if(event.key.keysym.sym == SDLK_q)
						m_bQuit = true;
				}
			}
		}

		// Only update every 300 milliseconds.
		if(m_LastTime - m_CurrentTime > 0.3)
		{
            SDL_RenderClear(m_pRenderer);
            SDL_RenderCopy(m_pRenderer, m_pBoard->GetBackground(), NULL, m_pBoard->GetSize());

            for(unsigned int i = 0; i < m_pBoard->marks.size(); i++)
                SDL_RenderCopy(m_pRenderer, m_pBoard->marks[i].texture, NULL, &m_pBoard->marks[i].location);

            if(m_GameState == GAMESTATE_STARTUP)
			{
				SDL_QueryTexture(mMessageStartup, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, mMessageStartup, NULL, &mMessageLocation);
			}
            else if(m_GameState == GAMESTATE_INPROGRESS && m_bPlayerTurn == true)
			{
				SDL_QueryTexture(mMessagePlayerOneTurn, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, mMessagePlayerOneTurn, NULL, &mMessageLocation);
			}
            else if(m_GameState == GAMESTATE_INPROGRESS && m_bPlayerTurn == false && m_bAI == false)
			{
				SDL_QueryTexture(mMessagePlayerTwoTurn, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, mMessagePlayerTwoTurn, NULL, &mMessageLocation);
			}
            else if(m_GameState == GAMESTATE_PLAYERWON)
			{
				SDL_QueryTexture(mMessagePlayerOneWon, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, mMessagePlayerOneWon, NULL, &mMessageLocation);

				SDL_QueryTexture(mMessageStartup, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = mMessageLocation.y + mMessageLocation.h;
                SDL_RenderCopy(m_pRenderer, mMessageStartup, NULL, &mMessageLocation);
			}
            else if(m_GameState == GAMESTATE_AIWON && m_bAI == false)
			{
				SDL_QueryTexture(mMessagePlayerTwoWon, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, mMessagePlayerTwoWon, NULL, &mMessageLocation);

				SDL_QueryTexture(mMessageStartup, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = mMessageLocation.y + mMessageLocation.h;
                SDL_RenderCopy(m_pRenderer, mMessageStartup, NULL, &mMessageLocation);
			}
            else if(m_GameState == GAMESTATE_AIWON && m_bAI == true)
			{
				SDL_QueryTexture(mMessageAIWon, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, mMessageAIWon, NULL, &mMessageLocation);

				SDL_QueryTexture(mMessageStartup, NULL, NULL, &mMessageLocation.w, &mMessageLocation.h);
                mMessageLocation.x = (m_WindowWidth - mMessageLocation.w) / 2;
				mMessageLocation.y = mMessageLocation.y + mMessageLocation.h;
                SDL_RenderCopy(m_pRenderer, mMessageStartup, NULL, &mMessageLocation);
			}

            SDL_RenderPresent(m_pRenderer);

			m_LastTime = SDL_GetTicks();
		}
	}

	return 0;
}
