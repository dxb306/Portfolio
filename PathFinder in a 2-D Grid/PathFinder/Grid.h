/*
	Grid.h
	18/01/16
	Daniel Butters
	Creates a grid of a certain height and width
	This grid is a vector of Cells
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
			
	Contains methods to create the grid and find a path
 	through the grid avoiding obstacles using my implimentation of
	the A* pathfinding algorithm
*/

#ifndef GRID_H_
#define GRID_H_

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <vector>

//include header files for needed classes
#include "Position.h"
#include "Cell.h"
#include "MapWindow.h"

using namespace std;

class Grid {
public:
	//creates a grid with default width and height of 50*50
	Grid();
	//creates a grid with width and height provided by user
	Grid(int width, int height);

	//returns width of grid
	int GetWidth() const {
		return fWidth;
	}
	
    //returns height of grid
    int GetHeight() const {
        return fHeight;
    }
    
	//returns a pointer to the cell stored in cellArray given any cell
    Cell * GetCell(Cell current){
        int posy = current.GetPosY();
        int posx = current.GetPosX();
        return &cellArray[posy][posx];
    }
    
	//returns a pointer to the cell stored in cellArray given the position
    Cell * GetCell(Position pos){
        return &cellArray[pos.Y()][pos.X()];
    }
	
	//a print function for printing to console
	void Print();
	
	/*
		a print function for printing to the MapWindow window given
		Passed a pointer to the MapWindow instance so that the buttons can change
		depending on the output from this algorithm
	*/
	void Print(MapWindow * win);
    
	/*
		function to print the position of a cell in a nice format
    	uses print function declared in cell class
	*/
	void printCell(Position pos);
    
	/*
		takes console input to set
		a cell of a certain position as type 'Start'
    */
	void SetStart();
	
	//sets the cell with the position provided as the type 'Start'
	void SetStart(Position pos);
    
	//returns the cell with the type 'Start' in the cellArray of the grid
    Cell GetStart();
    
	/*
		takes console input to set
		a cell of a certain position as type 'End'
    */
    void SetEnd();
	
	//sets the cell with the position provided as the type 'End'
	void SetEnd(Position pos);
	
	//returns the cell with the type 'End' in the cellArray of the grid
	Cell GetEnd();
	
	//sets the cell with the position provided as the type 'Obstacle'
	void SetObstacle(Position pos);
    
	/*
		Estimates the distance between a given cell and the end point in the grid
		Has no knowledge of obstacles in the way so uses a heuristic to give 
		an estimated answer
	*/
    double GetDistToGoal(Cell current);
    
	/*
		Attempts to find a path between the start and end points in the grid
		Passed a pointer to the MapWindow instance so that the buttons can change
		depending on the output from this algorithm
	*/
    int PlotRoute(MapWindow * win);
    
	/*
		checks that a certain position is within the dimensions of the grid 
		so that a non-existent cell is not accessed as this would cause a crash
	*/
    bool CoordCheck(Position pos);
    
	//checks if a cell is of type 'Obstacle'
    bool IsObstacle(Cell current);
	
	//checks if a cell is of type 'Closed'
	bool IsClosed(Cell current);
    
	/*
		attempts to return the 8 neighbouring cells of a particular cell
		if a cell is of 'Obstacle' or 'Closed' type, or cell position is
		outside the grid, then returns less than 8 neighbours
	*/
    vector <Cell> GetNeighbours(Cell current);
    
	/*
		Once a path has been found between the start and end points, returns
		the list of cells that make up the path between the two points
	*/
    vector <Cell> GetPath(Cell end);
    
	/*
		a 2-D vector of Cells that make up the grid
		When the grid is created, this array is resized to the size of the grid
		Each cell is accessed using cellArray[y][x]. 
	*/
    vector < vector <Cell> > cellArray;
	
	/*
		variable storing the pointer to the MapWindow instance
		this allows the print function to change the colours of the grid
		in the window so that the user can see the route plotted
	*/
	MapWindow * win;
	
private:
	//private variables for the height and the width of the grid
	int fWidth;
    int fHeight;

};

#endif /* GRID_H_ */

