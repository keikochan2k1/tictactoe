/*
 * game.hpp
 *
 *  Created on: 5 Jul 2013
 *      Author: Fredrick Lockert
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class CBoard;

class CGame
{
public:
	CGame();
	~CGame();

	int Initialise();
	int Run();

private:
	SDL_Window*   m_pWindow;
        SDL_Renderer* m_pRenderer;
        int           m_WindowWidth;
        int           m_WindowHeight;
	CBoard*       m_pBoard;
	Uint32        m_StartTime;
	Uint32        m_CurrentTime;
	Uint32        m_LastTime;
	bool          m_bQuit;
	bool          m_bPlayerTurn;
	bool          m_bAI;
	int           m_GameState;
        TTF_Font*     m_pFont;
	SDL_Color     mTextColour;
	SDL_Texture*  mMessageStartup;
	SDL_Texture*  mMessagePlayerOneTurn;
	SDL_Texture*  mMessagePlayerTwoTurn;
	SDL_Texture*  mMessagePlayerOneWon;
	SDL_Texture*  mMessagePlayerTwoWon;
	SDL_Texture*  mMessageAIWon;
	SDL_Rect      mMessageLocation;
};

#endif /* GAME_HPP_ */
