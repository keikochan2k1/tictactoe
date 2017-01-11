/*
 * gamestates.hpp
 *
 *  Created on: 11 Jan 2017
 *      Author: Fredrick Lockert
 */

#ifndef GAMESTATES_HPP
#define GAMESTATES_HPP

enum eGameStates
{
    GAMESTATE_STARTUP = 0,
    GAMESTATE_INPROGRESS,
    GAMESTATE_PLAYERWON,
    GAMESTATE_AIWON,
    GAMESTATE_GAMEOVER
};

#endif // GAMESTATES_HPP
