/*
	Cell.cpp
	18/01/16
	Daniel Butters
	Creates a Cell
	Each cell has a type and position
	Cells can have these types:
	Untravelled: cells that have not been checked by the PlotRoute function
				 effectively 'background' cells
	Start: The cell that has been designated as the start of the route
	End: The cell that has been designated as the end of the route
	Obstacle: Cells that are not traversable for the PlotRoute function
	Path: Cells that are part of the route found from the start to the end
		  returned as a vector once the algorithm has found the end cell
	Open: Cells in the 'Open' vector of the PlotRoute function
		  this means that these cells have not been checked yet but may be 
		  chosen as the next viable cell by the PlotRoute function
	Closed: Cells in the 'Closed' vector of the PlotRoute function
			this means that these cells have been checked by the PlotRoute
			function and may be part of the path that is ultimately plotted
*/

#include <iostream>
#include <sstream>

#include "Position.h"
#include "Cell.h"
#include "Grid.h"

using namespace std;

//creates a default cell with position 0,0 and type Cell
Cell::Cell() {
    fPos.SetPos(0,0);
    fType = "Cell";
}

//creates a cell with position pos and type Cell
Cell::Cell(Position pos)
: fPos(pos), fType("Cell"){}

//creates a cell with position pos and type passed as a string
Cell::Cell(Position pos, string type)
:fPos(pos),fType(type){}

//prints the position and type of the cell to the console
void Cell::Print()const
{
	cout << "PosX = " << fPos.X() << endl
    <<"PosY = " << fPos.Y()<< endl
	<<"Type = " << fType<<endl;
}

//sets the type as 'UnTravelled'
void Cell::SetAsUnTravelled()
{
    SetType("UnTravelled");
}

//sets the type as 'Closed'
void Cell::SetAsClosed()
{
    fType = "Closed";
}

//sets the type as 'Open'
void Cell::SetAsOpen()
{
    fType = "Open";
}

//sets the type as 'Obstacle'
void Cell::SetAsObstacle()
{
    fType = "Obstacle";
}

/*
	sets the type as 'Start'
	when the start cell is chosed, the distance from start is set as 0
	when cells assign start as their parent, they add 1 or 1.414 to their
	distance from start variable depending on if they are diagonal to the start
	or not
*/
void Cell::SetAsStart()
{
    fDistFromStart = 0;
    fType = "Start";
}

//sets the type as 'End'
void Cell::SetAsEnd()
{
    fType = "End";
}

//sets the type as 'Path'
void Cell::SetAsPath()
{
    fType = "Path";
}

/*
	sets the fParPos variable as the position of the parent
	sets fHasParent as 1
	sets fDistFromStart as the parents fDistFromStart + 1 or 1.414 depending
	on if they are diagonal to the parent or not
*/
void Cell::SetParent(Cell parent)
{
	/*
		checks if the cell is diagonal to the parent
		or vertical/horizontal to the parent
	*/
    int vertDist = abs(fPos.Y()-parent.GetPosY());
    int horDist = abs(fPos.X()-parent.GetPosX());
    int dist = vertDist + horDist;
    if(dist == 1){
        fDistFromStart = parent.fDistFromStart+1;
    }
    else{
        fDistFromStart = parent.fDistFromStart+1.414;
    }
    fHasParent = 1;
	fParPos = parent.GetPos();
}

//returns 1 if the cell has had SetParent called on it
bool Cell::HasParent()
{
    if(fHasParent){
        return 1;
    }
    else{
        return 0;
    }
}

//returns the position of the parent of this cell
Position Cell::GetParPos() const
{
    return fParPos;
}

