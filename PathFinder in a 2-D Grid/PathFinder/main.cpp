/*
 main.cpp
 18/01/16
 Daniel Butters
 Creates a window which recieves input from the user,
 sends the input to the Grid class, then displays the output
 to the screen
*/

#include "Grid.h"
#include "MapWindow.h"

using namespace std;

int main(int argc, const char * argv[]) {
	//Define the dimensions of the grid as constant integers
	const int gridWidth = 60;
	const int gridHeight = 50;
	/*
	Create an instance of the Grid class of dimensions gridWidth*gridHeight
	This creates the grid of cells of 'UnTravelled' type (i.e. a blank grid)
	*/
	Grid Map(gridWidth, gridHeight);  
	/*
	Create an instance of the MapWindow class of dimensions gridWidth*gridHeight
	This creates the window  with the grid of buttons, with a header and a key
	Pass a pointer to the Map variable so that the MapWindow can access the Grid
	class functions and transfer data to and from the Map instance.s
	*/   
	MapWindow win(gridWidth, gridHeight, &Map);
    return 0;
}


