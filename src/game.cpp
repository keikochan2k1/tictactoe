/*
 * game.cpp
 *
 *  Created on: 05 Jul 2013
 *      Author: Fredrick Lockert
 *  Updated on: 11 Jan 2017
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
    m_pMessageStartup       = NULL;
    m_pMessagePlayerOneTurn = NULL;
    m_pMessagePlayerTwoTurn = NULL;
    m_pMessagePlayerOneWon  = NULL;
    m_pMessagePlayerTwoWon  = NULL;
    m_pMessageAIWon         = NULL;
    m_pMessageGameOver      = NULL;

	m_WindowWidth   = 325;
	m_WindowHeight  = 400;
	m_StartTime     = 0;
	m_CurrentTime   = 0;
	m_LastTime      = 0;
    m_bQuit         = false;
    m_bPlayerTurn   = true;
	m_bAI           = false;
    m_eGameState     = GAMESTATE_STARTUP;

    m_TextColour = {0, 0, 0, 0};
    m_MessageLocation = {15, 25, 0, 0};
}

//=============================================================================
// Name: ~Game
// Desc: Tower of netherrack.
//=============================================================================
CGame::~CGame()
{
    if(m_pMessageStartup != NULL)
        SDL_DestroyTexture(m_pMessageStartup);

    if(m_pMessagePlayerOneTurn != NULL)
        SDL_DestroyTexture(m_pMessagePlayerOneTurn);

    if(m_pMessagePlayerTwoTurn != NULL)
        SDL_DestroyTexture(m_pMessagePlayerTwoTurn);

    if(m_pMessagePlayerOneWon != NULL)
        SDL_DestroyTexture(m_pMessagePlayerOneWon);

    if(m_pMessagePlayerTwoWon != NULL)
        SDL_DestroyTexture(m_pMessagePlayerTwoWon);

    if(m_pMessageAIWon != NULL)
        SDL_DestroyTexture(m_pMessageAIWon);

    if(m_pMessageGameOver != NULL)
        SDL_DestroyTexture(m_pMessageGameOver);

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

	// Load assets into memory.
    m_pBoard->SetBackground(m_pRenderer, "../data/assets/background.bmp");
    m_pBoard->SetPlayer1(m_pRenderer, "../data/assets/x.bmp");
    m_pBoard->SetPlayer2(m_pRenderer, "../data/assets/o.bmp");

    SDL_Surface* temp = TTF_RenderText_Blended(m_pFont, "Press Ctrl+N to start a new game.", m_TextColour);
    m_pMessageStartup = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Turn: Player One", m_TextColour);
    m_pMessagePlayerOneTurn = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Turn: Player Two", m_TextColour);
    m_pMessagePlayerTwoTurn = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Player One Won The Game!", m_TextColour);
    m_pMessagePlayerOneWon = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Player Two Won The Game!", m_TextColour);
    m_pMessagePlayerTwoWon = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "AI Won The Game!", m_TextColour);
    m_pMessageAIWon = SDL_CreateTextureFromSurface(m_pRenderer, temp);

    temp = TTF_RenderText_Blended(m_pFont, "Game Over!", m_TextColour);
    m_pMessageGameOver = SDL_CreateTextureFromSurface(m_pRenderer, temp);

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
                    if(m_eGameState == GAMESTATE_INPROGRESS)
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
                                    m_eGameState = GAMESTATE_PLAYERWON;
								else if(m_pBoard->GameWon(false) == GAMESTATE_AIWON)
                                    m_eGameState = GAMESTATE_AIWON;

                                if(m_eGameState != GAMESTATE_PLAYERWON || m_eGameState != GAMESTATE_AIWON)
                                {
                                    if(m_pBoard->GameOver() == GAMESTATE_GAMEOVER)
                                        m_eGameState = GAMESTATE_GAMEOVER;
                                }
							}
						}
					}
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
                        m_eGameState = GAMESTATE_INPROGRESS;
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
        } // End SDL_PollEvent

        if(m_eGameState == GAMESTATE_INPROGRESS)
        {
            if(m_bPlayerTurn == false && m_bAI == true)
            {
                m_pBoard->SimulateAI();
                m_bPlayerTurn = true;

                if(m_pBoard->GameWon(false) == GAMESTATE_AIWON)
                    m_eGameState = GAMESTATE_AIWON;

                if(m_eGameState != GAMESTATE_AIWON)
                {
                    if(m_pBoard->GameOver() == GAMESTATE_GAMEOVER)
                        m_eGameState = GAMESTATE_GAMEOVER;
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

            if(m_eGameState == GAMESTATE_STARTUP)
			{
                SDL_QueryTexture(m_pMessageStartup, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, m_pMessageStartup, NULL, &m_MessageLocation);
			}
            else if(m_eGameState == GAMESTATE_INPROGRESS && m_bPlayerTurn == true)
			{
                SDL_QueryTexture(m_pMessagePlayerOneTurn, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, m_pMessagePlayerOneTurn, NULL, &m_MessageLocation);
			}
            else if(m_eGameState == GAMESTATE_INPROGRESS && m_bPlayerTurn == false && m_bAI == false)
			{
                SDL_QueryTexture(m_pMessagePlayerTwoTurn, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, m_pMessagePlayerTwoTurn, NULL, &m_MessageLocation);
			}
            else if(m_eGameState == GAMESTATE_PLAYERWON)
			{
                SDL_QueryTexture(m_pMessagePlayerOneWon, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, m_pMessagePlayerOneWon, NULL, &m_MessageLocation);

                SDL_QueryTexture(m_pMessageStartup, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = m_MessageLocation.y + m_MessageLocation.h;
                SDL_RenderCopy(m_pRenderer, m_pMessageStartup, NULL, &m_MessageLocation);
			}
            else if(m_eGameState == GAMESTATE_AIWON && m_bAI == false)
			{
                SDL_QueryTexture(m_pMessagePlayerTwoWon, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, m_pMessagePlayerTwoWon, NULL, &m_MessageLocation);

                SDL_QueryTexture(m_pMessageStartup, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = m_MessageLocation.y + m_MessageLocation.h;
                SDL_RenderCopy(m_pRenderer, m_pMessageStartup, NULL, &m_MessageLocation);
			}
            else if(m_eGameState == GAMESTATE_AIWON && m_bAI == true)
			{
                SDL_QueryTexture(m_pMessageAIWon, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, m_pMessageAIWon, NULL, &m_MessageLocation);

                SDL_QueryTexture(m_pMessageStartup, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = m_MessageLocation.y + m_MessageLocation.h;
                SDL_RenderCopy(m_pRenderer, m_pMessageStartup, NULL, &m_MessageLocation);
			}
            else if(m_eGameState == GAMESTATE_GAMEOVER)
            {
                SDL_QueryTexture(m_pMessageGameOver, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = 25;
                SDL_RenderCopy(m_pRenderer, m_pMessageGameOver, NULL, &m_MessageLocation);

                SDL_QueryTexture(m_pMessageStartup, NULL, NULL, &m_MessageLocation.w, &m_MessageLocation.h);
                m_MessageLocation.x = (m_WindowWidth - m_MessageLocation.w) / 2;
                m_MessageLocation.y = m_MessageLocation.y + m_MessageLocation.h;
                SDL_RenderCopy(m_pRenderer, m_pMessageStartup, NULL, &m_MessageLocation);
            }

            SDL_RenderPresent(m_pRenderer);

			m_LastTime = SDL_GetTicks();
		}
	}

	return 0;
}
