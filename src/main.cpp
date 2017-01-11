/*
 * main.cpp
 *
 *  Created on: 5 Jul 2013
 *      Author: Fredrick Lockert
 *  Updated on: 11 Jan 2017
 */

#include "game.hpp"

int main()
{
	CGame game;

	int status = 0;

	status = game.Initialise();

	if(status == 0)
		status = game.Run();

	return status;
}
