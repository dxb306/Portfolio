/*
 MapWindow.cpp
 18/01/15
 Daniel Butters
 Impliments methods defined in MapWindow.h
*/


#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <iostream>

#include "Grid.h"
#include "MapWindow.h"

	/*
		Declare callback functions before they are assigned to the 
		buttons so that they do not have to be static functions
	*/
	void null_but_callback(Fl_Widget* but, void* p);

	void first_but_callback(Fl_Widget* but, void* p);
	
	void second_but_callback(Fl_Widget* head, void* p);
	
	void third_but_callback(Fl_Widget* head, void* p);
	
	void first_done_callback(Fl_Widget* head, void* p);
	
	void second_done_callback(Fl_Widget* head, void* p);
	
	/*
		creates the grid of buttons used to represent the  grid of cells
		this is a grid of buttons that are 10*10 pixels each
	*/
	void MapWindow::CreateButtons() {
    	for(int i = 0; i < fWidth; i++){
        	for(int j = 0; j< fHeight; j++){
				//calculate the position of each button based on the header
				int posx = i*10;
				int posy = (j*10)+fBoxHeight;
				//use 'new' to create a new instance of Fl_Button
				Fl_Button * but = new Fl_Button( posx, posy, 10, 10, "" ); 
				//set the buttons as grey, used for untravelled cells
				but->color(FL_BACKGROUND_COLOR);
				but->box(FL_THIN_DOWN_BOX);
				/*
					set the callback of every button in the grid
					passing a pointer to this instance of the MapWindow class
					This callback will set the start cell in the grid
				*/ 
				but->callback(first_but_callback, thisWindow);
				//add the pointer to the button to the buttonArray vector
				buttonArray.push_back(but);
        	}
    	}
	}
	
	//creates the key to define the colours of the buttons for the user
	void MapWindow::CreateKey() {
		//make a header for the key with the label 'KEY'
		keyHeader = new Fl_Box((fWidth*10),fHeight,fKeyWidth,fBoxHeight,"KEY");
		keyHeader->labelfont(FL_BOLD);
	
		//a vector of the colours used to display the grid to the user
		vector < Fl_Color > colours;
		colours.push_back(FL_MAGENTA);
		colours.push_back(FL_RED);
		colours.push_back(FL_BLACK);
		colours.push_back(FL_DARK_BLUE);
		colours.push_back(FL_BLUE);
		colours.push_back(FL_DARK_GREEN);
		colours.push_back(FL_BACKGROUND_COLOR);
		
		//a vector of labels to label the colours for the user
		vector < char* > labels;
		labels.push_back(" = Start");
		labels.push_back(" = End");
		labels.push_back(" = Obstacle");
		labels.push_back(" = Untested Cells");
		labels.push_back(" = Tested Cells");
		labels.push_back(" = Path Cells");
		labels.push_back(" = Unused Cells");
		
		/*
			creates a series of coloured button and labels to show the user 
			what each colour represents in the grid
		*/
		for(int i = 0; i< colours.size(); i++){
			//calculates the position of the coloured buttons
			int butposx = (fWidth*10)+20;
			int butposy = (fHeight*(i+2));
			//creates the coloured button for the key
			Fl_Button * colourKey = new Fl_Button(butposx,butposy,fBoxHeight,fBoxHeight,"");
			colourKey->color(colours[i]);
			//sets the boxtype, all except the last button are FL_FLAT_BOX
			if(i<colours.size()-1){
				colourKey->box(FL_FLAT_BOX);
			}
			//Untravelled cells use a different boxtype so the last category uses this boxtype
			else{
				colourKey->box(FL_THIN_DOWN_BOX);
			}
			//add the coloured button pointers to the keyColours vector
			keyColours.push_back(colourKey);
			
			//calculates the position of the key labels
			int labposx = (fWidth*10)+20+fBoxHeight;
			int labposy = (fHeight*(i+2));
			int labw = (fKeyWidth-40)-fBoxHeight;
		
			//creates the key label boxes
			Fl_Box * labelKey = new Fl_Box(labposx,labposy,labw,fBoxHeight,labels[i]);
			//change the label font to bold
			labelKey->labelfont(FL_BOLD);
			//add the label box pointers to the keyLabels vector
			keyLabels.push_back(labelKey);
		}
		
		//create a 'done' button. this will trigger the plotroute function and exit the window
		done = new Fl_Button((fWidth*10)+20,(fHeight*9),fKeyWidth-40,fBoxHeight,"Plot Route");
		done->color(FL_DARK_GREEN);
		/*
			set the callback function of the done button, passing a pointer to this 
			instance of the MapWindow class to the callback function
		*/
		done->callback(first_done_callback,thisWindow);	
		/*
			initially set this button as deactivated as the user shouldnt 
			press this button until the start, end and obstacle cells have been chosen
		*/	  	  
		done->deactivate();   
	}

	//a callback function that does nothing, used to deactivate buttons without making them grey
	void null_but_callback(Fl_Widget* but, void* p){

	}	 
	
	/*
		callback function for the done button, this callback 
		sets all of the callbacks for the buttons in the grid to null_but_callback
		triggers the PlotRoute function in the grid
		Sets the header label to tell the user how to exit the program
		Changes the callback and label of this button to exit the window
	*/
	void first_done_callback(Fl_Widget* but, void* p){
		//this button
   		Fl_Button* done = (Fl_Button*) but;
		
		//pointer to this instance of the MapWindow class
		MapWindow * thisWindow = (MapWindow*) p;
		
		//vector of pointers to the buttons representing the grid
		vector < Fl_Button *> buttonArray = thisWindow->GetButtons();
		
		//box used to give instructions to the user
		Fl_Box * header = thisWindow->GetHeader();
		
		//sets all of the callbacks for the buttons in the grid to null_but_callback
		for(int i = 0; i < buttonArray.size();i++){
			buttonArray[i]->callback(null_but_callback);
		}
		
		//pointer to the grid full of cells
		Grid * fMap = thisWindow->GetMap();
  		
		//triggers the PlotRoute function in the grid
		fMap->PlotRoute(thisWindow);
		
		//Sets the header label to tell the user how to exit the program
		header->label("Press the 'Exit' Button to exit the program");
		
		//Changes the callback and label of this button to exit the window
		done->label("Exit");
		done->color(FL_RED);
		done->redraw();
		done->callback(second_done_callback);
	}	  
	
	/*
		calls the'hide' function on the Fl_Window. This function causes the program to finish
		it is only possible to call this function once the plotroute function has been called	 
	*/
	void second_done_callback(Fl_Widget* but, void* p){
   		Fl_Button* done = (Fl_Button*) but;
		
		MapWindow * thisWindow = (MapWindow*) p;
		
		thisWindow->GetWindow()->hide();
	}	  
	
	/*
		this is the first callback function applied to all the buttons in the grid
		sets the start button in the grid
	*/
	void first_but_callback(Fl_Widget* but, void* p){
		
		MapWindow * thisWindow = (MapWindow*) p;
		
		Grid * fMap = thisWindow->GetMap();
		
		vector < Fl_Button *> buttonArray = thisWindow->GetButtons();
	
   		Fl_Button* button = (Fl_Button*) but;
		
		Fl_Box* header = thisWindow->GetHeader();
  
		Fl_Color buttonColour = button->color();
		if(buttonColour == FL_BACKGROUND_COLOR){
			//colours the button magenta to show that it is the start button
	   		button->color(FL_MAGENTA);
			//changes the boxtype of the button to be FL_FLAT_BOX
			button->box(FL_FLAT_BOX);
   			button->redraw();
			//changes the callback to be a null callback
			button->callback(null_but_callback);
			
			//changed the label of the header to give the second instruction to user
			header->label("2. Press a button to create the end");
   			header->redraw(); 
			
			/*
				calculates the position of the cell in the grid based on
			 	the buttons position in the window
			*/
			Position Start((button->y()-40)/10,(button->x())/10);
			//sets the start cell in the grid
			fMap->SetStart(Start);
			cout<<"Start Position: "<<endl;
			fMap->GetStart().Print();
		}
		//changes the callback of all other buttons in the grid to the second callback function
		for(int i = 0; i<buttonArray.size(); i++){
			Fl_Color buttonColour = buttonArray[i]->color();
			if(buttonColour != FL_MAGENTA){
				buttonArray[i]->callback(second_but_callback,thisWindow);
			}
		}
	}
	
	/*
		the second callback function applied to all the buttons in the grid
		sets the end button in the grid
	*/
	void second_but_callback(Fl_Widget* but, void* p){
		
		MapWindow * thisWindow = (MapWindow*) p;	
			
		Grid * fMap = thisWindow->GetMap();
	
   		Fl_Button* button = (Fl_Button*) but;
		
		vector < Fl_Button *> buttonArray = thisWindow->GetButtons();
		
		Fl_Box* header = thisWindow->GetHeader();
		//checks that the button is not the start button
		Fl_Color buttonColour = button->color();
		if(buttonColour == FL_BACKGROUND_COLOR){
			//changes colour and boxtype
   			button->color(FL_RED);
			button->box(FL_FLAT_BOX);
   			button->redraw();
			//changes callback so button callback cannot be called again
			button->callback(null_but_callback);
			
			//changes header label to provide third instruction to user
			header->label("3. Press buttons to create obstacles. Press 'Plot Route' Button when done.");
   			header->redraw(); 
			
			/*
				calculates the position of the cell in the grid based on
			 	the buttons position in the window
			*/
			Position End((button->y()-40)/10,(button->x())/10);
			//sets the end cell in the grid
			fMap->SetEnd(End);
			cout<<"End Position: "<<endl;
			fMap->GetEnd().Print();
		}
		/*
			changes the callback of all other buttons in the grid to the
			third callback function, avoiding the start and end buttons
		*/
		for(int i = 0; i<buttonArray.size(); i++){
			Fl_Color buttonColour = buttonArray[i]->color();
			if(buttonColour != FL_MAGENTA && buttonColour != FL_RED){
				buttonArray[i]->callback(third_but_callback,thisWindow);
			}
		}
	}
	
	/*
		the third callback function applied to all the buttons in the grid
		sets the obstacle buttons in the grid
	*/
	void third_but_callback(Fl_Widget* but, void* p){
	
		MapWindow * thisWindow = (MapWindow*) p;
	
		Grid * fMap = thisWindow->GetMap();
	
   		Fl_Button* button = (Fl_Button*) but;
		
		Fl_Button* done = thisWindow->GetDoneButton();
		
		//activates the 'done' button so that PlotRoute function can be called
		done->activate();
		
		vector < Fl_Button *> buttonArray = thisWindow->GetButtons();
		
		/*
			calculates the position of the cell in the grid based on
		 	the buttons position in the window
		*/
		Position Obs((button->y()-40)/10,(button->x())/10);
		/*
			gets the positions of the 3*3 grid of buttons surround the button
		 	that has been pressed, storing them in a vector
		*/
		vector < Position > ObsPos;
		ObsPos.push_back(Position(Obs.Y()-1,Obs.X()-1));
		ObsPos.push_back(Position(Obs.Y()-1,Obs.X()));
		ObsPos.push_back(Position(Obs.Y()-1,Obs.X()+1));
		ObsPos.push_back(Position(Obs.Y(),Obs.X()-1));
		ObsPos.push_back(Obs);
		ObsPos.push_back(Position(Obs.Y(),Obs.X()+1));
		ObsPos.push_back(Position(Obs.Y()+1,Obs.X()-1));
		ObsPos.push_back(Position(Obs.Y()+1,Obs.X()));
		ObsPos.push_back(Position(Obs.Y()+1,Obs.X()+1));
		
		/*
		sets the 3*3 grid surrounding the button to an obstacle, changing the colour
		of the button and the type of the cell that that button is representing
		*/
		for(int i = 0; i<ObsPos.size();i++){
			int ypos = ObsPos[i].Y();
			int xpos = ObsPos[i].X();
			if(xpos>=0 && xpos<thisWindow->GetWidth()){
				if(ypos>=0 && ypos<thisWindow->GetHeight()){
					/*
						converts from the 2-D position of the button to the 1-D index
						of the button in the buttonArray
					*/
					int buttonIndex = xpos*thisWindow->GetHeight() + ypos;
					Fl_Color buttonColour = buttonArray[buttonIndex]->color();
					//only changes buttons that are not start or end buttons to obstacles
					if(buttonColour == FL_BACKGROUND_COLOR){
						//sets colour and boxtype of buttons then redraws
						buttonArray[buttonIndex]->color(FL_BLACK);
						buttonArray[buttonIndex]->box(FL_FLAT_BOX);
						buttonArray[buttonIndex]->redraw();
						//sets callback of buttons so that they dont perform any function
						buttonArray[buttonIndex]->callback(null_but_callback);
						//sets the cells of these coordinates to the type 'Obstacle'
						fMap->SetObstacle(ObsPos[i]);
					}
				}
			}
		}
	}

