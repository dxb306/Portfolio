/*
	Grid.cpp
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

#include <iostream>
#include <vector>
#include <iomanip>

#include "Position.h"
#include "Grid.h"
#include "Cell.h"

using namespace std;

//creates a grid with default width and height of 50*50
Grid::Grid() {
	//uses a default value for width and height
    fWidth = 50;
    fHeight = 50;
	//make the vector of rows fHeight rows long
    cellArray.resize(fHeight);
	for (int i = 0; i < fHeight; ++i){
		//make the rows fWidth cells long
        cellArray[i].resize(fWidth);
    }
	/*
		set the positions of each cell 
		initially set the type as untravelled so the grid has no obstacles
	*/
    for(int i = 0; i < fHeight; i++){
        for(int j = 0; j< fWidth; j++){
			Position pos(i,j);
            GetCell(pos)->SetPos(pos);
            GetCell(pos)->SetAsUnTravelled();
        }
    }
}

/*
	creates a grid with width and height provided by user
	set width and height using variables passed to function
*/
Grid::Grid(int width, int height)
: fWidth(width), fHeight(height){
	//make the vector of rows fHeight rows long
    cellArray.resize(fHeight);
	for (int i = 0; i < fHeight; ++i){
		//make the rows fWidth cells long
        cellArray[i].resize(fWidth);
    }
	/*
		set the positions of each cell 
		initially set the type as untravelled so the grid has no obstacles
	*/
    for(int i = 0; i < fHeight; i++){
        for(int j = 0; j< fWidth; j++){
			Position pos(i,j);
            GetCell(pos)->SetPos(pos);
            GetCell(pos)->SetAsUnTravelled();
        }
    }
}

//a print function for printing the grid to console
void Grid::Print()
{
	/*
	for each cell, get the type and print to console with width 2
	after every row start a new line so the console output forms a grid
	*/
    for(int i = 0; i<fHeight; i++){
        for(int j = 0; j<fWidth; j++){
			//get position of cell
			Position pos(i,j);
			//get type of cell
            string outputType = GetCell(pos)->GetType();
			//set each cell as a character for printing to the console
            if(outputType == "Start"){
                cout << setw(2) << "S";
            }
            else if(outputType == "UnTravelled"){
                cout << setw(2) << "U";
            }
            else if(outputType == "Closed"){
                cout << setw(2) << "C";
            }
            else if(outputType == "Open"){
                cout << setw(2) << "O";
            }
            else if(outputType == "Obstacle"){
                cout << setw(2) << "#";
            }
            else if(outputType == "End"){
                cout << setw(2) << "E";
            }
            else if(outputType == "Path"){
                cout << setw(2) << "P";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

/*
	a print function for printing to the MapWindow window given
	Passed a pointer to the MapWindow instance so that the buttons can change
	depending on the output from this algorithm
*/
void Grid::Print(MapWindow * win)
{
	/*
	Obstacle, Start and End types have been set so will not change
	This means that they will stay the same colour so dont need to be
	included in this function
	For Closed, Open and Path types set the button as a certain colour
	and change the box type to FL_FLAT_BOX, then redraw the button
	*/
    for(int i = 0; i<fHeight; i++){
        for(int j = 0; j<fWidth; j++){
			Position pos(i,j);
            string outputType = GetCell(pos)->GetType();
			//get the array of buttons representing the grid in the window
			vector < Fl_Button *> buttonArray = win->GetButtons();
			/*
				convert the 2-D position vector to the 1-D index for the
				1-D vector of buttons in the window
			*/
			int buttonIndex = j*win->GetHeight() + i;
			//set each type of button as a different colour
            if(outputType == "Closed"){
                buttonArray[buttonIndex]->color(FL_BLUE);
				buttonArray[buttonIndex]->box(FL_FLAT_BOX);
				buttonArray[buttonIndex]->redraw();
            }
            else if(outputType == "Open"){
                buttonArray[buttonIndex]->color(FL_DARK_BLUE);
				buttonArray[buttonIndex]->box(FL_FLAT_BOX);
				buttonArray[buttonIndex]->redraw();
            }
            else if(outputType == "Path"){
                buttonArray[buttonIndex]->color(FL_DARK_GREEN);
				buttonArray[buttonIndex]->box(FL_FLAT_BOX);
				buttonArray[buttonIndex]->redraw();
            }
        }
    }
}

/*
	function to print the position of a cell in a nice format
	uses print function declared in cell class
*/
void Grid::printCell(Position pos)
{
    (GetCell(pos))->Print();
}

/*
	takes console input to set
	a cell of a certain position as type 'Start'
*/
void Grid::SetStart()
{
    int posy;
    cout<<"Y Position of Start Point: ";
    cin>>posy;
    int posx;
    cout<<"X Position of Start Point: ";
    cin>>posx;
    cellArray[posy][posx].SetAsStart();
}

//sets the cell with the position passed to the function as the type 'Start'
void Grid::SetStart(Position pos)
{
    int posy = pos.Y();
	int posx = pos.X();
    cellArray[posy][posx].SetAsStart();
}

//returns the cell with the type 'Start' in cellArray
Cell Grid::GetStart()
{
    int posy = 0;
    int posx = 0;
	//checks type of all cells, storing the position of cell with type 'Start'
    for(int i = 0; i<fHeight; i++){
        for(int j = 0; j<fWidth; j++){
			Position pos(i,j);
            string outputType = GetCell(pos)->GetType();
            if(outputType == "Start"){
                posy = i;
                posx = j;
            }
        }
    }
    return cellArray[posy][posx];
}

/*
	takes console input to set
	a cell of a certain position as type 'End'
*/
void Grid::SetEnd()
{
    int posy;
    cout<<"Y Position of End Point: ";
    cin>>posy;
    int posx;
    cout<<"X Position of End Point: ";
    cin>>posx;
    cellArray[posy][posx].SetAsEnd();
}

//sets the cell with the position passed to the function as the type 'End'
void Grid::SetEnd(Position pos)
{
    int posy = pos.Y();
	int posx = pos.X();
    cellArray[posy][posx].SetAsEnd();
}

//returns the cell with the type 'End' in the cellArray of the grid
Cell Grid::GetEnd()
{
    int posy = 0;
    int posx = 0;
	//checks type of all cells, storing the position of cell with type 'End'
    for(int i = 0; i<fHeight; i++){
        for(int j = 0; j<fWidth; j++){
			Position pos(i,j);
            string outputType = GetCell(pos)->GetType();
            if(outputType == "End"){
            	posy = i;
                posx = j;
            }
        }
    }
    return cellArray[posy][posx];
}

//sets the cell with the position provided as the type 'Obstacle'
void Grid::SetObstacle(Position pos)
{
    int posy = pos.Y();
	int posx = pos.X();
    cellArray[posy][posx].SetAsObstacle();
}

/*
	Estimates the distance between a given cell and the end point in the grid
	Has no knowledge of obstacles in the way so uses a heuristic to give 
	an estimated answer
*/
double Grid::GetDistToGoal(Cell current)
{
    Cell Goal = GetEnd();
    double biggerDist;
    double smallerDist;
	//finds absolute verticle distance between current cell and the end cell
    double vertDist = abs(Goal.GetPosY()-current.GetPosY());
	//finds absolute horizontal distance between current cell and the end cell
    double horDist = abs(Goal.GetPosX()-current.GetPosX());
	//whichever distance is smaller is set as the smaller distance
    if(vertDist>horDist){
        biggerDist = vertDist;
        smallerDist = horDist;
    }
    else{
        biggerDist = horDist;
        smallerDist = vertDist;
    }
	/*
		Travelling diagonally costs 1.414 'distance'
		Travelling horizontally or vertically costs 1 'distance'
		
		Assume that you can travel diagonally for the smaller distance
		then travel horizontally or vertically for 
		(bigger distance - smaller distance)
		
		This is the distance of the path that would be plotted by the
		algorithm between the current cell and the goal if there were no 
		obstacles in the way
		
		This is a heuristic estimate, used by the algorithm to choose which
		cell to travel to next
	*/
    double dist = 1.414*smallerDist+(biggerDist-smallerDist);
    return dist;
}

/*
	Attempts to find a path between the start and end points in the grid
	Passed a pointer to the MapWindow instance so that the buttons can change
	depending on the output from this algorithm
*/
int Grid::PlotRoute(MapWindow * window)
{
	//takes input for the pointer to the window displaying the buttons
	win = window;
	
	//find start and end cells
    Cell Start = GetStart();
    Cell Goal = GetEnd();
	
	//cell variable for the previous cell chosen by the algorithm
	Cell LastCell;
	
	//variable for number of cells checked by the function
	int stepNumber = 0;
	
	//initialise open list, cells which could be checked by the function
    vector < Cell > OpenList;
	
	//add start to open list as this is the first cell to be checked
	OpenList.push_back(Start);
	
	//initialise closed list, cells which have been checked by the function
    vector < Cell > ClosedList;
	
	//iterate the function until the goal is found
    while(OpenList.size()){
        /*
			set any cells added to the open or closed list as open or closed
			dont change the type of the start or end cell
		*/ 
        for(int i = 0; i<OpenList.size();i++){
            if(OpenList[i].GetType() != "Start" &&
			OpenList[i].GetType() != "End"){
                GetCell(OpenList[i])->SetAsOpen();
            }
        }
        for(int i = 0; i<ClosedList.size();i++){
            if(ClosedList[i].GetType() != "Start" &&
			ClosedList[i].GetType() != "End"){
                GetCell(ClosedList[i])->SetAsClosed();
            }
        }
		
		//output step number to console
		stepNumber = stepNumber + 1;
		cout<<"Step Number = " << stepNumber<<endl;

        /*
			find cell in openlist with lowest combined distance from start 
			and estimated distance to goal. This is the cell most likely to
			get the path closer to the goal in the shortest distance
		*/
        Cell current;
        int currentIndex = 0;
		/*
			make the minimum score larger than the largest possible score 
			so that there is guaranteed to be at least one cell below
			the initial minimum score
		*/
        double minScore = fWidth*fHeight;
        for(int i = 0; i<OpenList.size(); i++){
			Cell openCell = OpenList[i];
			/*
				Distance from start is derived by setting the 'Start' cell
				distance as 0, then adding 1 or 1.414 (depending if diagonal
				or horizontal or verticle) for each cell which has 'Start'
				as it's parent, then continuing to add to this variable as 
				cells are assigned parent cells
			*/
            double distScore = openCell.GetDistFromStart() + 
							   GetDistToGoal(openCell);
			if(distScore<minScore){
                minScore = distScore;
                currentIndex = i;
            }
        }
		/*
			set current cell as the cell with lowest combined distance from 
			start and estimated distance to goal.
		*/
		int currentPosY = OpenList[currentIndex].GetPosY();
		int currentPosX = OpenList[currentIndex].GetPosX();
        current = cellArray[currentPosY][currentPosX];
        
		/*
			if current cell is the end cell,follow chain of parent cells back
			to start cell to form a vector of cells which is the route to 
			be displayed to the user
			Set each cell in the route as 'Path' type so that the colour is
			changed in the window so the user can see the path plotted between
			start and end cells.
		*/	  
		if(current == Goal){
			/*
			set the parent of the end cell as the penultimate cell to the
			current one
			*/
			Position lastCellPos = LastCell.GetPos();
			Goal.SetParent(cellArray[lastCellPos.Y()][lastCellPos.X()]);
			//declare vector of cells which form the path between start and end
			vector <Cell> pathCells;
			//follow chain of parents from end to start to form path
            pathCells = GetPath(GetEnd());
			/*
				for each cell in the path that is not start or end, change
				type to path so that the path can be displayed to the user
			*/
			for(int i = 0; i < pathCells.size(); i++){
				Position pathPos = pathCells[i].GetPos();
				string cellType = cellArray[pathPos.Y()][pathPos.X()].GetType();
				if(cellType != "End" && cellType!="Start"){
					cellArray[pathPos.Y()][pathPos.X()].SetAsPath();
				}
			}
			/*
				delete the OpenList variable so that the function will stop
				trying to find the end cell as it has already found it
			*/	  	  	  
			OpenList.clear();
			
			/*
				print the complete route to the MapWindow window so that the 
				user can view the path plotted
			*/
			Print(win);
			return 0;
        }
		
		//if the current cell is not the end cell
        else{ 
			//add checked node to closed list, delete from open list
			ClosedList.push_back(current);
        	OpenList.erase(OpenList.begin()+currentIndex);
			LastCell = current;
		}
       
		//get all neighbouring cells that are not obstacles
    	vector <Cell> neighbourList = GetNeighbours(current);
			
		//for each valid neighbouring cell
    	for(int i = 0; i<neighbourList.size(); i++){
		    Cell neighbour = neighbourList[i];
			
			//check if in openlist
        	bool inOpenList = false;
			int openIndex;
            for(int j = 0; j < OpenList.size(); j++){
            	if(neighbour == OpenList[j]){
                	inOpenList = true;
                }
            }
				
			//check if in closed list
            bool inClosedList = false;
			int closedIndex;
            for(int j = 0; j < ClosedList.size(); j++){
	        	if(neighbour == ClosedList[j]){
    	        	inClosedList = true;
        	    }
            }
			
			//if it is not in closed or open list, add to open list
			if(!inClosedList&&!inOpenList){
	        	OpenList.push_back(neighbour);
			}	 	
		}
    }
}



/*
	checks that a certain position is within the dimensions of the grid 
	so that a non-existent cell is not accessed as this would cause a crash
*/
bool Grid::CoordCheck(Position pos){
    bool yresult = false;
	bool xresult = false;
	bool result;
	int y = pos.Y();
	int x = pos.X();
    if(y >= 0 && y <= (fHeight-1)){
    	yresult = true;
	}
	if(x >= 0 && x <= (fWidth-1)){
    	xresult = true;
    }
	if(xresult&&yresult){
		result = true;
	}
	else{
		result = false;
	}
    return result;
}

//checks if a cell is of type 'Obstacle'
bool Grid::IsObstacle(Cell current){
    bool result;
    string type = current.GetType();
    if(type == "Obstacle"){
        result = true;
    }
    else{
        result = false;
    }
    return result;
}

//checks if a cell is of type 'Closed'
bool Grid::IsClosed(Cell current){
    bool result;
    string type = current.GetType();
    if(type == "Closed"){
        result = true;
    }
    else{
        result = false;
    }
    return result;
}

/*
	attempts to return the 8 neighbouring cells of a particular cell
	if a cell is of 'Obstacle' or 'Closed' type, or cell position is
	outside the grid, then returns less than 8 neighbours
*/
vector <Cell> Grid::GetNeighbours(Cell current){
    vector <Cell> neighbours;
    int posY = current.GetPosY();
    int posX = current.GetPosX();
	//stores the positions of the neighbouring 8 cells in two integer arrays
	int yArray[8] = {posY-1,posY-1,posY-1,posY,posY,posY+1,posY+1,posY+1};
	int xArray[8] = {posX-1,posX,posX+1,posX-1,posX+1,posX-1,posX,posX+1};
	//creates a vector of Positions of the neighbours and checks they are inside the grid
	vector <Position> neighPos;
	for(int i = 0; i<8; i++){
		Position pos(yArray[i],xArray[i]);
		if(CoordCheck(pos)==true){
	    	neighPos.push_back(pos);
		}
    }
	//if the neighbour is not an obstacle, add to neighbours vector
    for(int i = 0; i<neighPos.size();i++){
    	Cell newCell=cellArray[neighPos[i].Y()][neighPos[i].X()];
        if(!IsObstacle(newCell)){
			//closed cells already have 'parent cells' so check that cell is not closed
			if(!IsClosed(newCell)){
				cellArray[newCell.GetPosY()][newCell.GetPosX()].SetParent(current);	 
			}
			//add to neighbours vector to return to plotroute function
			neighbours.push_back(newCell);
		}    
    }
    return neighbours;
}

/*
	Once a path has been found between the start and end points, returns
	the list of cells that make up the path between the two points
	Only want to return the cells between start and end, not including start and end
*/
vector <Cell> Grid::GetPath(Cell end){
	//declare vector of cells that make up the path
    vector <Cell> path;
	//get starting position as penultimate cell before end and add it to path vector
	Position pathPos = end.GetParPos();
	Cell pathCell = cellArray[pathPos.Y()][pathPos.X()]; 
    path.push_back(pathCell);
	/*
		chains of parents very occasionally form loops so exit out of loop after
		the loop has iterated fHeight*fWidth times just in case
	*/ 
	int count = 0;
	/*
		Go along the chain of parents of cells, adding each parent cell to the path
		vector until the chain of parents reaches the starting cell
	*/
    while(pathCell!= GetStart()&&count<fHeight*fWidth){
		Position parPos = pathCell.GetParPos();
		Cell parent = cellArray[parPos.Y()][parPos.X()];		
        pathCell = parent;
        path.push_back(pathCell);
		count+=1;
    }
    return path;
}

