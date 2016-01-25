/* 
Daniel Butters
10/1/16
Forest Fire Model
This program models forest fires in a grid, printing to console or gnuplot
Can use default settings or user can configure them through the console
*/


#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <stdlib.h> 
#include <time.h> 
#include <vector>
#include <stdio.h>
#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot


using namespace std;

//types of square can either be tree, empty or fire
const char types[3] = {'T','E','F'}; 

/*
initial conditions for creating the forest
height and width are dimesions for the grid
Prob Trees is the initial %probability for a tree to be planted in the grid
set default values of a 100*100 grid with 30% of spaces starting as trees
allow for user to change values later
*/
string  initCondNames[4] = {"Height","Width","Prob Trees"};
int initCond[3] = {120,120,0};

//p is probability for a tree to spontaneously grow in an empty space
double p = 0.01;

//f is probability for a tree to spontaneously catch fire (struck by 'lightning')
double f = p/100;

//number of steps to iterate forest for
int numSteps = 150;

//setting for if the program should print to console or not
bool consolePrint = 0;

//setting for if the program should print to gnuplot or not
bool gnuPrint = 1;

//setting for if the program should output 
//number of trees,empty,fires to console and save to file
bool resultsPrint = 1;

//a 2-D vector of characters represents the forest. These trees can be one of three types
vector< vector<char> > trees;

//a gnuplot to display the forest as a scatter plot
Gnuplot pic("forest");

//file for outputting results
ofstream outfile;

//a function to return a pseudo random number between 0 and 1
double randProb(){
	return (rand() % 100000)/100000.;
}

/*
impliments a periodic boundary system where if a tree is outside the edge of the 
forest, the function returns the tree on the opposite edge
this creates an effectively infinite forest as there is no 'edge' to the forest
*/
char borderCheck(int y, int x){
	int aheight = initCond[0];
	int awidth = initCond[1];
	if(y<0){
		y = aheight-1;
	}
	else if(y>aheight-1){
		y = 0;
	}
	else if(x<0){
		x = awidth-1;
	}
	else if(x>awidth-1){
		x = 0;
	}
	return trees[y][x];
}

//returns the trees surrounding a tree of position posy and posx
char * findTrees(int posy, int posx) {

	char * surround = new char(8);
	surround[0] = borderCheck(posy-1,posx-1);
	surround[1] = borderCheck(posy-1,posx);
	surround[2] = borderCheck(posy-1,posx+1);
	surround[3] = borderCheck(posy,posx-1);
	surround[4] = borderCheck(posy,posx+1);
	surround[5] = borderCheck(posy+1,posx+1);
	surround[6] = borderCheck(posy+1,posx);
	surround[7] = borderCheck(posy+1,posx-1);
	return(surround);
}

//prints the grid of trees to the console, colouring each type of tree a different colour
void printtoconsole() {
	int aheight = initCond[0];
	int awidth = initCond[1];
	cout<<endl;
	for(int i = 0; i<aheight; i++){
		for(int j = 0; j<awidth; j++){
			char output = trees[i][j];
			if(output == types[0]){
				cout << "\033[1;" << 32 << "m"<< setw(2) << output<< "\033[0m";
			}
			else if(output == types[1]){
				cout << "\033[1;" << 37 << "m"<< setw(2) << output<< "\033[0m";
			}
			else if(output == types[2]){
				cout << "\033[1;" << 31 << "m"<< setw(2) << output<< "\033[0m";
			}
		}
		cout<<endl;
	}
}

//plots the grid of trees with gnuplot using a scatter plot
void printtognuplot(int n){
	int aheight = initCond[0];
	int awidth = initCond[1];
	//go through forest, getting coordinates for each type of tree
	//only plot trees and fire, leaving empty spots unplotted to save processing power
	vector <int> treesx;
	vector <int> treesy;
	vector <int> firesx;
	vector <int> firesy;
	for(int i = 0; i<aheight;i++){
		for(int j = 0; j<awidth;j++){
			char type = trees[i][j];
			if(type == types[0]){
				treesx.push_back(j);
				treesy.push_back(i);			
			}
			else if(type == types[2]){
				firesx.push_back(j);
				firesy.push_back(i);			
			}
		}
	}
	//reset plot
	pic.reset_plot();

	//plot each spot as a coloured square
	pic.set_style("points pointtype 8");

	//plot trees and fires separately so they are different colours
	if(treesx.size()){
		pic.plot_xy(treesx,treesy,"");
	}
	pic.set_style("points pointtype 5");
	if(firesx.size()){
		pic.plot_xy(firesx,firesy,"");
	}


	//gnuplot has a limit for the number of temporary files created
	//of about 20 files, so clear temporary files after every 15 steps
	if(n%15==0){
		pic.remove_tmpfiles();
	}

	//pause for a second to allow the plot function to catch up
	sleep(1);
}

//creates a starting forest for the process using initial conditions set by user
void plantTrees(){
	int aheight = initCond[0];
	int awidth = initCond[1];
	double probtree = initCond[2]/100.;
	// initialize random seed with time to create a random distribution
  	srand (time(NULL));
	// Set up sizes of 2-D vector. (HEIGHT x WIDTH)
  	trees.resize(aheight);
  	for (int i = 0; i < aheight; ++i){
    		trees[i].resize(awidth);
	}
	for(int i = 0; i<aheight; i++){
		for(int j = 0; j<awidth; j++){
			double num = randProb();
			//a certain % of spaces start as trees or empty as defined by user
			if(num<probtree){
				trees[i][j] = types[0];
			}
			else{
				trees[i][j] = types[1];
			}
		}
	}
}

//receives input from user about initial settings or leaves default values
int getInitCond(){
	bool useStandardCond = 1;
	cout<<"Standard Conditions are: "<<endl;
	for(int i = 0; i<3;i++){
		cout<<initCondNames[i]<< " = " << initCond[i] << endl;
	}
	cout<<"Probability of tree growing = " << p << endl;
	cout<<"Probability of tree spontaneously catching fire = " << f << endl;
	cout<<"Use standard conditions? 1 for yes or 0 for no: "<<endl;;
	cin>>useStandardCond;
	//if user wants to define conditions, take input
	if(!useStandardCond){
		for(int i = 0; i<3;i++){
			cout<<"Please enter integer value for " << initCondNames[i]<< ": ";
			cin>>initCond[i];	
		}
		cout<<"Please enter integer value for probability of tree growing: ";
		cin>>p;;
		cout<<"Please enter integer value for probability of tree catching fire: ";
		cin>>f;
		cout<<"Please enter number of steps to iterate forest for: ";
		cin>>numSteps;
		cout<<"Please enter 0 or 1 for whether to print forest to console: ";
		cin>>consolePrint;
		cout<<"Please enter 0 or 1 for whether to print forest with gnuplot: ";
		cin>>gnuPrint;
		cout<<"Please enter 0 or 1 for whether to print results to file";
		cin>>resultsPrint;
	}
}



void calcProb(int n, double p, double f){
	int aheight = initCond[0];
	int awidth = initCond[1];
	//create new vector to edit so that the forest can all update at once
	vector< vector<char> > newtrees = trees;
	for(int i = 0; i<aheight; i++){
		for(int j = 0; j<awidth; j++){
			//find surrounding trees using periodic borders
			char * surroundtrees = findTrees(i,j);
			int nburn = 0;	
			//check if any surrounding trees are burning		
			for(int k = 0; k<8;k++){
				if(surroundtrees[k]==types[2]){
					nburn+=1;
				}
			}
			delete [] surroundtrees;
			//IF BURNING TURN EMPTY
			if(trees[i][j] == types[2]){
				newtrees[i][j] = types[1];
			}
			//IF TREE
			if(trees[i][j] == types[0]){
				//BURN WITH RANDOM CHANCE F
				if(randProb()<f){
					newtrees[i][j] = types[2];
				}
				//IF 1 OR MORE BURNING NEIGHBOUR, BURN
				if(nburn>0){
					newtrees[i][j] = types[2];
				}
				
			}
			//IF EMPTY, GROW TREE WITH CHANCE P
			if(trees[i][j] == types[1]){
				if(randProb()<p){
					newtrees[i][j] = types[0];
				}
			}
		}
	}
	//update forest in one step 
	trees = newtrees;	
}

//go through forest, counting the number of each type of node
vector <int> checkForest(){
	int aheight = initCond[0];
	int awidth = initCond[1];
	vector <int> Results;	
	Results.push_back(0);
	Results.push_back(0);
	Results.push_back(0);
	for(int i = 0; i<aheight; i++){
		for(int j = 0; j<awidth; j++){
			char type = trees[i][j];
			for(int k = 0; k<3;k++){
				if(type == types[k]){
					Results[k] = Results[k]+1;
				}
			}
		}
	}
	return Results;
}


int main() {
	//get settings from user or use defaults
	getInitCond();

	if(resultsPrint){
		//create filename, adding variables to give descriptive name
		string filename = "Forest-";
		stringstream dimstr1;
		dimstr1 << initCond[0];
		filename = filename + dimstr1.str();
		filename = filename + "-";
		stringstream dimstr2;
		dimstr2 << initCond[1];
		filename = filename + dimstr2.str();
		filename = filename + "-";
		stringstream probstr;
		probstr << p*1000;
		filename = filename + probstr.str();
		filename = filename + ".txt";
		cout<<filename.c_str()<<endl;
		outfile.open(filename.c_str());
	}

	//create forest of trees randomly planted
	plantTrees();
	
	//iterate algorithm for numSteps number of steps
	for(int n = 0; n<numSteps; n++){
		//iterate algorithm
		calcProb(n,p,f);
		
		if(consolePrint){
			printtoconsole();
		}

		if(gnuPrint){
			printtognuplot(n);
		}

		if(resultsPrint){
			vector <int> Results;
			Results = checkForest();
			cout<<"Step Number = " << n << endl
			<<"No. Trees = "<<Results[0]<< endl
			<<"No. Empty Spaces = "<<Results[1]<< endl
			<<"No. Fires = "<<Results[2]<<endl<<endl;
			outfile<<n<<","<<Results[0]<<','<<Results[1]<<','<<Results[2]<<endl;
		}
	}
	if(resultsPrint){
		outfile.close();
	}
}

 
