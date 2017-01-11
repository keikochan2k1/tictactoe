/*
 * board.cpp
 *
 *  Created on: 6 Jul 2013
 *      Author: Fredrick Lockert
 *  Updated on: 10 Jan 2017
 */

#include "board.hpp"

//=============================================================================
// Name: CBoard
// Desc: Allocates default values.
//=============================================================================
CBoard::CBoard()
{
	boardOffset.x = 0;
	boardOffset.y = 75;

	markSize.x = 64;
	markSize.y = 64;

    m_pBackground = NULL;
    m_pPlayer1 = NULL;
    m_pPlayer2 = NULL;
}

//=============================================================================
// Name: ~CBoard
// Desc: Deletes any pointers we may have allocated.
//=============================================================================
CBoard::~CBoard()
{
    if(m_pBackground != NULL)
        SDL_DestroyTexture(m_pBackground);

    if(m_pPlayer1 != NULL)
        SDL_DestroyTexture(m_pPlayer1);

    if(m_pPlayer2 != NULL)
        SDL_DestroyTexture(m_pPlayer2);
}

//=============================================================================
// Name: NewBoard
// Desc: Creates a new empty board.
//=============================================================================
void CBoard::NewBoard()
{
	if(marks.size() != 0)
		marks.clear();

	// 0,0
	SDL_Rect size;
	size.x = 27;
	size.y = boardOffset.y + 27;
	size.w = 64;
	size.h = 64;

	Marks mark1;
	mark1.name = "Top 0.0";
	mark1.location = size;
	mark1.texture = NULL;
	marks.push_back(mark1);

	// 1,0
	size.x = 130;
	size.y = boardOffset.y + 27;

	Marks mark2;
	mark2.name = "Top 1,0";
	mark2.location = size;
	mark2.texture = NULL;
	marks.push_back(mark2);


	// 2,0
	size.x = 232;
	size.y = boardOffset.y + 27;

	Marks mark3;
	mark3.name = "Top 2.0";
	mark3.location = size;
	mark3.texture = NULL;
	marks.push_back(mark3);


	// 0,1
	size.x = 27;
	size.y = boardOffset.y + 127;

	Marks mark4;
	mark4.name = "Top 0.1";
	mark4.location = size;
	mark4.texture = NULL;
	marks.push_back(mark4);


	// 1,1
	size.x = 130;
	size.y = boardOffset.y + 127;

	Marks mark5;
	mark5.name = "Top 1.1";
	mark5.location = size;
	mark5.texture = NULL;
	marks.push_back(mark5);

	// 2,1
	size.x = 230;
	size.y = boardOffset.y + 127;

	Marks mark6;
	mark6.name = "Top 2.1";
	mark6.location = size;
	mark6.texture = NULL;
	marks.push_back(mark6);

	// 0,2
	size.x = 27;
	size.y = boardOffset.y + 232;

	Marks mark7;
	mark7.name = "Top 0.2";
	mark7.location = size;
	mark7.texture = NULL;
	marks.push_back(mark7);

	// 1,2
	size.x = 130;
	size.y = boardOffset.y + 232;

	Marks mark8;
	mark8.name = "Top 1.2";
	mark8.location = size;
	mark8.texture = NULL;
	marks.push_back(mark8);

	// 2,2
	size.x = 230;
	size.y = boardOffset.y + 232;

	Marks mark9;
	mark9.name = "Top 2.2";
	mark9.location = size;
	mark9.texture = NULL;
	marks.push_back(mark9);
}

//=============================================================================
// Name: SetBackground
// Desc: Sets the board background image.
//=============================================================================
int CBoard::SetBackground(SDL_Renderer* renderer, std::string path)
{
	SDL_Surface* temp = SDL_LoadBMP(path.data());

    if(temp == NULL)
    {
        fprintf(stderr, "ERROR: SDL_LoadBMP Failed: %s\n", SDL_GetError());
        return -1;
    }

    m_pBackground = SDL_CreateTextureFromSurface(renderer, temp);

	SDL_Rect size;
	size.x = 0;
	size.y = boardOffset.y;
	size.w = temp->w;
	size.h = temp->h;

    m_BackgroundSize = size;

	SDL_FreeSurface(temp);

    return 1;
}

//=============================================================================
// Name: SetPlayer1
// Desc: Sets the player one image.
//=============================================================================
int CBoard::SetPlayer1(SDL_Renderer* renderer, std::string path)
{
	SDL_Surface* temp = SDL_LoadBMP(path.data());

    if(temp == NULL)
    {
        fprintf(stderr, "ERROR: SDL_LoadBMP Failed: %s\n", SDL_GetError());
        return -1;
    }

	Uint32 color = SDL_MapRGB(temp->format, 246, 246, 246);
	SDL_SetColorKey(temp, SDL_TRUE, color);
    m_pPlayer1 = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

    return 0;
}

//=============================================================================
// Name: SetPlayer2
// Desc: Sets the player two / AI image.
//=============================================================================
int CBoard::SetPlayer2(SDL_Renderer* renderer, std::string path)
{
	SDL_Surface* temp = SDL_LoadBMP(path.data());

    if(temp == NULL)
    {
        fprintf(stderr, "ERROR: SDL_LoadBMP Failed: %s\n", SDL_GetError());
        return -1;
    }

	Uint32 color = SDL_MapRGB(temp->format, 246, 246, 246);
	SDL_SetColorKey(temp, SDL_TRUE, color);
    m_pPlayer2 = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

    return 1;
}

//=============================================================================
// Name: GetBackground
// Desc: Returns a pointer to the board background image.
//=============================================================================
SDL_Texture* CBoard::GetBackground()
{
    return m_pBackground;
}

SDL_Rect* CBoard::GetSize()
{
    return &m_BackgroundSize;
}

//=============================================================================
// Name: Update
// Desc: Checks if the clicked x,y coordinates is within a marks range. If
//       within range, it checks if the texture is empty and allocates the
//       respective texture, else returns -1 if occupied.
//=============================================================================
int CBoard::Update(bool player, int x, int y)
{
	for(unsigned int i = 0; i < marks.size(); i++)
	{
		if(x > marks[i].location.x && x < marks[i].location.x+markSize.x && y > marks[i].location.y && y < marks[i].location.y+markSize.y)
		{
			if(marks[i].texture == NULL)
			{
				if(player)
                    marks[i].texture = m_pPlayer1;
				else
                    marks[i].texture = m_pPlayer2;

				return 1;
			}
		}
	}

	return -1;
}

//=============================================================================
// Name: SimulateAI
// Desc: Goes through a simple list of actions the AI should perform.
//=============================================================================
void CBoard::SimulateAI()
{
    if(AIPotentialWin())
        return;

	if(AIBlock())
		return;

	AIRandom();
}

bool CBoard::AIPotentialWin()
{
    // Vertical lines
    // First Row
    if(marks[0].texture == NULL && marks[3].texture == m_pPlayer2 && marks[6].texture == m_pPlayer2)
    {
        marks[0].texture = m_pPlayer2;
        return true;
    }
    else if(marks[0].texture == m_pPlayer2 && marks[3].texture == NULL && marks[6].texture == m_pPlayer2)
    {
        marks[3].texture = m_pPlayer2;
        return true;
    }
    else if(marks[0].texture == m_pPlayer2 && marks[3].texture == m_pPlayer2 && marks[6].texture == NULL)
    {
        marks[6].texture = m_pPlayer2;
        return true;
    }
    // Second Row
    else if(marks[1].texture == NULL && marks[4].texture == m_pPlayer2 && marks[7].texture == m_pPlayer2)
    {
        marks[1].texture = m_pPlayer2;
        return true;
    }
    else if(marks[1].texture == m_pPlayer2 && marks[4].texture == NULL && marks[7].texture == m_pPlayer2)
    {
        marks[4].texture = m_pPlayer2;
        return true;
    }
    else if(marks[1].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[7].texture == NULL)
    {
        marks[7].texture = m_pPlayer2;
        return true;
    }
    //Third Row
    else if(marks[2].texture == NULL && marks[5].texture == m_pPlayer2 && marks[8].texture == m_pPlayer2)
    {
        marks[2].texture = m_pPlayer2;
        return true;
    }
    else if(marks[2].texture == m_pPlayer2 && marks[5].texture == NULL && marks[8].texture == m_pPlayer2)
    {
        marks[5].texture = m_pPlayer2;
        return true;
    }
    else if(marks[2].texture == m_pPlayer2 && marks[5].texture == m_pPlayer2 && marks[8].texture == NULL)
    {
        marks[8].texture = m_pPlayer2;
        return true;
    }

    // Horizontal lines
    // First Column
    else if(marks[0].texture == NULL && marks[1].texture == m_pPlayer2 && marks[2].texture == m_pPlayer2)
    {
        marks[0].texture = m_pPlayer2;
        return true;
    }
    else if(marks[0].texture == m_pPlayer2 && marks[1].texture == NULL && marks[2].texture == m_pPlayer2)
    {
        marks[1].texture = m_pPlayer2;
        return true;
    }
    else if(marks[0].texture == m_pPlayer2 && marks[1].texture == m_pPlayer2 && marks[2].texture == NULL)
    {
        marks[2].texture = m_pPlayer2;
        return true;
    }
    // Second Column
    else if(marks[3].texture == NULL && marks[4].texture == m_pPlayer2 && marks[5].texture == m_pPlayer2)
    {
        marks[3].texture = m_pPlayer2;
        return true;
    }
    else if(marks[3].texture == m_pPlayer2 && marks[4].texture == NULL && marks[5].texture == m_pPlayer2)
    {
        marks[4].texture = m_pPlayer2;
        return true;
    }
    else if(marks[3].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[5].texture == NULL)
    {
        marks[5].texture = m_pPlayer2;
        return true;
    }
    // Third Column
    else if(marks[6].texture == NULL && marks[7].texture == m_pPlayer2 && marks[8].texture == m_pPlayer2)
    {
        marks[6].texture = m_pPlayer2;
        return true;
    }
    else if(marks[6].texture == m_pPlayer2 && marks[7].texture == NULL && marks[8].texture == m_pPlayer2)
    {
        marks[7].texture = m_pPlayer2;
        return true;
    }
    else if(marks[6].texture == m_pPlayer2 && marks[7].texture == m_pPlayer2 && marks[8].texture == NULL)
    {
        marks[8].texture = m_pPlayer2;
        return true;
    }

    // Diagonal lines
    // Left to Right
    else if(marks[0].texture == NULL && marks[4].texture == m_pPlayer2 && marks[8].texture == m_pPlayer2)
    {
        marks[0].texture = m_pPlayer2;
        return true;
    }
    else if(marks[0].texture == m_pPlayer2 && marks[4].texture == NULL && marks[8].texture == m_pPlayer2)
    {
        marks[4].texture = m_pPlayer2;
        return true;
    }
    else if(marks[0].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[8].texture == NULL)
    {
        marks[8].texture = m_pPlayer2;
        return true;
    }
    // Right to Left
    else if(marks[2].texture == NULL && marks[4].texture == m_pPlayer2 && marks[6].texture == m_pPlayer2)
    {
        marks[2].texture = m_pPlayer2;
        return true;
    }
    else if(marks[2].texture == m_pPlayer2 && marks[4].texture == NULL && marks[6].texture == m_pPlayer2)
    {
        marks[4].texture = m_pPlayer2;
        return true;
    }
    else if(marks[2].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[6].texture == NULL)
    {
        marks[6].texture = m_pPlayer2;
        return true;
    }

    return false;
}

//=============================================================================
// Name: AIRandom
// Desc: Makes the AI do a random move.
//=============================================================================
bool CBoard::AIRandom()
{
	int numTries = 0;

	while(1)
	{
		int move = rand() % 9;

		if(marks[move].texture == NULL)
		{
            marks[move].texture = m_pPlayer2;
			return true;
		}
		else
			numTries++;

		if(numTries == 9)
			return false;
	}
}

//=============================================================================
// Name: AIBlock
// Desc: Checks if the player is about to get three in a row, and blocks it.
//=============================================================================
bool CBoard::AIBlock()
{
	// Top horizontal line
    if(marks[0].texture == m_pPlayer1 && marks[1].texture == m_pPlayer1 && marks[2].texture == NULL)
	{
        marks[2].texture = m_pPlayer2;
		return true;
	}
    else if(marks[0].texture == m_pPlayer1 && marks[1].texture == NULL && marks[2].texture == m_pPlayer1)
	{
        marks[1].texture = m_pPlayer2;
		return true;
	}
    else if(marks[0].texture == NULL && marks[1].texture == m_pPlayer1 && marks[2].texture == m_pPlayer1)
	{
        marks[0].texture = m_pPlayer2;
		return true;
	}
	// Mid horizontal line
    else if(marks[3].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[5].texture == NULL)
	{
        marks[5].texture = m_pPlayer2;
		return true;
	}
    else if(marks[3].texture == m_pPlayer1 && marks[4].texture == NULL && marks[5].texture == m_pPlayer1)
	{
        marks[4].texture = m_pPlayer2;
		return true;
	}
    else if(marks[3].texture == NULL && marks[4].texture == m_pPlayer1 && marks[5].texture == m_pPlayer1)
	{
        marks[3].texture = m_pPlayer2;
	}
	// Bottom horizontal line
    else if(marks[6].texture == m_pPlayer1 && marks[7].texture == m_pPlayer1 && marks[8].texture == NULL)
	{
        marks[8].texture = m_pPlayer2;
		return true;
	}
    else if(marks[6].texture == m_pPlayer1 && marks[7].texture == NULL && marks[8].texture == m_pPlayer1)
	{
        marks[7].texture = m_pPlayer2;
		return true;
	}
    else if(marks[6].texture == NULL && marks[7].texture == m_pPlayer1 && marks[8].texture == m_pPlayer1)
	{
        marks[6].texture = m_pPlayer2;
		return true;
	}
	// Left vertical line
    else if(marks[0].texture == m_pPlayer1 && marks[3].texture == m_pPlayer1 && marks[6].texture == NULL)
	{
        marks[6].texture = m_pPlayer2;
		return true;
	}
    else if(marks[0].texture == m_pPlayer1 && marks[3].texture == NULL && marks[6].texture == m_pPlayer1)
	{
        marks[3].texture = m_pPlayer2;
		return true;
	}
    else if(marks[0].texture == NULL && marks[3].texture == m_pPlayer1 && marks[6].texture == m_pPlayer1)
	{
        marks[0].texture = m_pPlayer2;
		return true;
	}
	// Mid vertical line
    else if(marks[1].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[7].texture == NULL)
	{
        marks[7].texture = m_pPlayer2;
		return true;
	}
    else if(marks[1].texture == m_pPlayer1 && marks[4].texture == NULL && marks[7].texture == m_pPlayer1)
	{
        marks[4].texture = m_pPlayer2;
		return true;
	}
    else if(marks[1].texture == NULL && marks[4].texture == m_pPlayer1 && marks[7].texture == m_pPlayer1)
	{
        marks[1].texture = m_pPlayer2;
		return true;
	}
	// Right vertical line
    else if(marks[2].texture == m_pPlayer1 && marks[5].texture == m_pPlayer1 && marks[8].texture == NULL)
	{
        marks[8].texture = m_pPlayer2;
		return true;
	}
    else if(marks[2].texture == m_pPlayer1 && marks[5].texture == NULL && marks[8].texture == m_pPlayer1)
	{
        marks[5].texture = m_pPlayer2;
		return true;
	}
    else if(marks[2].texture == NULL && marks[5].texture == m_pPlayer1 && marks[8].texture == m_pPlayer1)
	{
        marks[2].texture = m_pPlayer2;
		return true;
	}
	// Left to right cross
    else if(marks[0].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[8].texture == NULL)
	{
        marks[8].texture = m_pPlayer2;
		return true;
	}
    else if(marks[0].texture == m_pPlayer1 && marks[4].texture == NULL && marks[8].texture == m_pPlayer1)
	{
        marks[4].texture = m_pPlayer2;
		return true;
	}
    else if(marks[0].texture == NULL && marks[4].texture == m_pPlayer1 && marks[8].texture == m_pPlayer1)
	{
        marks[0].texture = m_pPlayer2;
		return true;
	}
	// Right to left cross
    else if(marks[2].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[6].texture == NULL)
	{
        marks[6].texture = m_pPlayer2;
		return true;
	}
    else if(marks[2].texture == m_pPlayer1 && marks[4].texture == NULL && marks[6].texture == m_pPlayer1)
	{
        marks[4].texture = m_pPlayer2;
		return true;
	}
    else if(marks[2].texture == NULL && marks[4].texture == m_pPlayer1 && marks[6].texture == m_pPlayer1)
	{
        marks[2].texture = m_pPlayer2;
		return true;
	}

	return false;
}

//=============================================================================
// Name: GameWon
// Desc: Checks if the player or AI has three marks in a row. Returns the
//       respective game state if one is found, else -1.
//=============================================================================
int CBoard::GameWon(bool player)
{
	if(player)
	{
		// Vertical lines
        if(marks[0].texture == m_pPlayer1 && marks[3].texture == m_pPlayer1 && marks[6].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
        else if(marks[1].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[7].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
        else if(marks[2].texture == m_pPlayer1 && marks[5].texture == m_pPlayer1 && marks[8].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
		// Horizontal lines
        else if(marks[0].texture == m_pPlayer1 && marks[1].texture == m_pPlayer1 && marks[2].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
        else if(marks[3].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[5].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
        else if(marks[6].texture == m_pPlayer1 && marks[7].texture == m_pPlayer1 && marks[8].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
		// Crossed lines
        else if(marks[0].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[8].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
        else if(marks[2].texture == m_pPlayer1 && marks[4].texture == m_pPlayer1 && marks[6].texture == m_pPlayer1)
			return GAMESTATE_PLAYERWON;
	}
	else
	{
		// Vertical lines
        if(marks[0].texture == m_pPlayer2 && marks[3].texture == m_pPlayer2 && marks[6].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
        else if(marks[1].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[7].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
        else if(marks[2].texture == m_pPlayer2 && marks[5].texture == m_pPlayer2 && marks[8].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
		// Horizontal lines
        else if(marks[0].texture == m_pPlayer2 && marks[1].texture == m_pPlayer2 && marks[2].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
        else if(marks[3].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[5].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
        else if(marks[6].texture == m_pPlayer2 && marks[7].texture == m_pPlayer2 && marks[8].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
		// Crossed lines
        else if(marks[0].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[8].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
        else if(marks[2].texture == m_pPlayer2 && marks[4].texture == m_pPlayer2 && marks[6].texture == m_pPlayer2)
			return GAMESTATE_AIWON;
	}

	return -1;
}
