/*
 MapWindow.h
 18/01/15
 Daniel Butters
 Creates a window which recieves input from the user,
 sends the input to the Grid class, then displays the output
 to the screen
*/

#ifndef MAPWINDOW_H_
#define MAPWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

class Grid;

#include "Grid.h"
#include "Cell.h"
#include "Position.h"

class MapWindow{
	public:
	//creates a MapWindow instance using width, height and a pointer to a grid
	MapWindow(int width, int height, Grid* Map){
		fHeight = height;
		fWidth = width;
		
		//height for the header and key boxes
		fBoxHeight = 40;
		
		//width of the key
		fKeyWidth = 200;
		
		//stores the pointer to the grid of cells
		fMap = Map;
		
		/*
			calculates the width of the window based on the key width and the
			number of cells in each row of the grid
		*/
		int winWidth = (fWidth*10)+fKeyWidth;
		
		/*
			calculates the height of the window based on the header height
			and the number of cells in each column of the grid
		*/
		int winHeight = (fHeight*10)+fBoxHeight;
		
		//creates a fltk window with the dimensions set with title "PathFinder"
		window = new Fl_Window(winWidth,winHeight,"PathFinder");
		
		/*
		creates a fltk box, same width as grid
		this header box uses the label to convey instructions to the user
		*/
		header = new Fl_Box(0,0,(fWidth*10),fBoxHeight,
		"1. Press a button to create the start");
		
		//set the font of the header to bold
		header->labelfont(FL_BOLD);
		
		//store the pointer of the instance of this class as thisWindow
		thisWindow = this;
		
		//create the grid of buttons
		CreateButtons();
		
		//create the key
		CreateKey();
		
		//run the window
		RunWindow();
	}
	
	//returns the height of the grid
	int GetHeight(){
		return fHeight;
	}
	
	//returns the width of the grid
	int GetWidth(){
		return fWidth;
	}
	
	//creates the grid of buttons that represents the map
	void CreateButtons();
	
	//creates the key to define the colours for the user
	void CreateKey();
	
	//finish editing the window then show it to the user
	int RunWindow(){
		window->end();
		window->show();
		return Fl::run();
	}
	
	//returns a pointer to the window
	Fl_Window * GetWindow(){
		return window;
	}
	
	//returns a pointer to the header box
	Fl_Box * GetHeader(){
		return header;
	}
	
	/*
		returns a pointer to the done button, used to activate the plotroute 
		function and then exit the window
	*/
	Fl_Button * GetDoneButton(){
		return done;
	}
	
	//returns a pointer to the map containing the cells
	Grid * GetMap() {
		return fMap;
	}
	
	//returns a vector of pointers to the buttons representing the grid
	vector < Fl_Button*> GetButtons() {
		return buttonArray;
	}
	
	//sets the fMap pointer to the grid inputted
	void SetMap(Grid * Map) {
		fMap = Map;
	}
	
	private:
	
	Grid * fMap;
	
	Fl_Box * header;
	
	Fl_Button * done;
	
	Fl_Box * keyHeader;
	
	//vector of buttons that represent the colours of the grid
	vector < Fl_Button * > keyColours;
	
	//vector of boxes that label the colours of the grid
	vector < Fl_Box * > keyLabels;
	
	vector < Fl_Button * > buttonArray;
	
	Fl_Window * window;
	
	Position * LastPressed;
	
	int fBoxHeight;
	
	int fKeyWidth;
	
	int fWidth;
	
	int fHeight;
	
	MapWindow * thisWindow;
	
};

#endif /* MAPWINDOW_H_ */

