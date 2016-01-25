/*
	Cell.h
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

#ifndef CELL_H_
#define CELL_H_

#include <string>
#include <vector>
#include "Position.h"

using namespace std;

class Cell {
public:
	//creates a default cell with position 0,0 and type Cell
	Cell();
	//creates a cell with position pos and type Cell
	Cell(Position pos);
	//creates a cell with position pos and type passed as a string
	Cell(Position pos, string type);

	//returns the x position of the cell
	int GetPosX() const {
		return fPos.X();
	}
    
	//returns the y position of the cell
    int GetPosY() const {
        return fPos.Y();
    }
    
	//returns the position of the cell
	Position GetPos() const {
        return fPos;
    }
	
	//sets the position of the cell using integer inputs
    void SetPos(int y, int x){
        fPos.SetPos(y,x);
    }
	
	//sets the position of the cell using Position input
	void SetPos(Position pos){
        fPos = pos;
    }
    
	//sets the position of the cell using the position of another cell
    void SetPos(Cell current){
        fPos.SetPos(current.GetPos());
    }
	
	//returns the distance from startpoint of grid
    double GetDistFromStart(){
        return fDistFromStart;
    }
	
	//returns the type of the cell
    string GetType()const{
    return fType;
	}
    
	//sets the type of the cell with string input
    void SetType(string type)
    {
        fType = type;
    }
    
	//prints the position and type of the cell to the console
	virtual void Print() const;
    
	//sets the type as 'UnTravelled'
    void SetAsUnTravelled();
    
	//sets the type as 'Closed'
    void SetAsClosed();
    
	//sets the type as 'Open'
	void SetAsOpen();
	
	//sets the type as 'Obstacle'
    void SetAsObstacle();
    
	//sets the type as 'Start'
    void SetAsStart();
    
	//sets the type as 'End'
    void SetAsEnd();
    
	//sets the type as 'Path'
    void SetAsPath();
    
	//Sets the fParPos as position of the cell passed to the function
    void SetParent(Cell parent);
    
	//returns true if SetParent has been called on the cell
    bool HasParent();
	
	//returns position of the parent of the cell
	Position GetParPos() const;
    
	/*
		defines what happens if a cell is assigned to another cell using '='
		the position, type and parent position of the cell being assigned
		are set as the same as the cell rferred to by '='
	*/
    void operator = (const Cell & c)
    {
        SetPos(c.GetPos());
        SetType(c.GetType());
		Position parPos = c.GetParPos();
		Cell parent(parPos);
		SetParent(parent);
    }
    
	/*
		defines the conditions for checking if one cell is the same as another
		a cell is the same as another cell if they share the same position
	*/
    bool operator == (const Cell & c)
    {
        if(fPos == c.GetPos())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
	/*
		defines the conditions for checking if one cell is not the same as
		another
		a cell is notthe same as another cell if they do not share the same
		position
	*/
    bool operator != (const Cell & c)
    {
        if(fPos != c.GetPos())
        {
            return true;
        }
        else
        {
        return false;
        }
    }

    

private:
    

	bool fHasParent;
    string fType;
	Position fPos;
	Position fParPos;
    double fDistFromStart;

};

#endif /* CELL_H_ */

