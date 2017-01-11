/*
 * board.hpp
 *
 *  Created on: 6 Jul 2013
 *      Author: Fredrick Lockert
 */

#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <vector>
#include <string>

#include <SDL2/SDL_render.h>

enum GameStates
{
	GAMESTATE_STARTUP = 0,
	GAMESTATE_INPROGRESS,
	GAMESTATE_PLAYERWON,
	GAMESTATE_AIWON,
	GAMESTATE_GAMEOVER
};

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
	SDL_Texture* GetBackground();
	SDL_Rect* GetSize();
        int SetPlayer1(SDL_Renderer* renderer, std::string path);
        int SetPlayer2(SDL_Renderer* renderer, std::string path);
	int  Update(bool player, int x, int y);
	void SimulateAI();
	int  GameWon(bool player);

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
        SDL_Rect     m_BackgroundSize;
        SDL_Texture* m_pPlayer1;
        SDL_Texture* m_pPlayer2;
};


#endif /* BOARD_HPP_ */
