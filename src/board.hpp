/*
 * board.hpp
 *
 *  Created on: 6 Jul 2013
 *      Author: Fredrick Lockert
 *  Updated on: 11 Jan 2017
 */

#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <vector>
#include <string>

#include <SDL2/SDL_render.h>
#include "gamestates.hpp"

struct Marks
{
    std::string name;
    SDL_Rect location;
    SDL_Texture* texture;
};

struct Size
{
    int x;
    int y;
};

class CBoard
{
public:
    CBoard();
    ~CBoard();

    void NewBoard();

    int SetBackground(SDL_Renderer* renderer, std::string path);
    int SetPlayer1(SDL_Renderer* renderer, std::string path);
    int SetPlayer2(SDL_Renderer* renderer, std::string path);

    int  Update(bool player, int x, int y);
    eGameStates GameWon(bool player);
    void SimulateAI();

    SDL_Texture* GetBackground();
    SDL_Rect* GetSize();

public:
    Size boardOffset;
    Size markSize;

    std::vector<Marks> marks;

private:
    bool AIPotentialWin();
    bool AIBlock();
    bool AIRandom();

private:
    SDL_Texture* m_pBackground;
    SDL_Texture* m_pPlayer1;
    SDL_Texture* m_pPlayer2;

    SDL_Rect     m_BackgroundSize;
};


#endif /* BOARD_HPP_ */
