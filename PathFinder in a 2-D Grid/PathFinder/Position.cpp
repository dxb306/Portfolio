/*
 Position.cpp
 18/12/15
 Daniel Butters
*/

#include <iostream>
#include <sstream>

#include "Position.h"
#include "Cell.h"
#include "Grid.h"

using namespace std;

//creates an instance of Position with coordinates 0,0
Position::Position() {
    fPosX = 0;
    fPosY = 0;
}

//creates an instance of Position with coordinates posy,posx
Position::Position(int posy, int posx)
: fPosY(posy), fPosX(posx){}

//prints the x and y component of the position
void Position::Print()const
{
	cout << "PosX = " << fPosX << endl
    <<"PosY = " << fPosY<< endl;
}


