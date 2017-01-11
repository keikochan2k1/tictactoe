/*
 * game.hpp
 *
 *  Created on: 5 Jul 2013
 *      Author: Fredrick Lockert
 *  Update on: 11 Jan 2017
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gamestates.hpp"

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
    SDL_Texture*  m_pMessageStartup;
    SDL_Texture*  m_pMessagePlayerOneTurn;
    SDL_Texture*  m_pMessagePlayerTwoTurn;
    SDL_Texture*  m_pMessagePlayerOneWon;
    SDL_Texture*  m_pMessagePlayerTwoWon;
    SDL_Texture*  m_pMessageAIWon;
    SDL_Color     m_TextColour;
    SDL_Rect      m_MessageLocation;

    TTF_Font*     m_pFont;

    CBoard*       m_pBoard;

    int           m_WindowWidth;
    int           m_WindowHeight;
    eGameStates   m_GameState;

    unsigned int  m_StartTime;
    unsigned int  m_CurrentTime;
    unsigned int  m_LastTime;

    bool          m_bQuit;
    bool          m_bPlayerTurn;
    bool          m_bAI;
};

#endif /* GAME_HPP_ */
