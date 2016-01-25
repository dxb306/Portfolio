/*
	Position.h
	18/01/16
	Daniel Butters
	A 2-D position vector used for the positions of cells
*/

#ifndef POSITION_H_
#define POSITION_H_

using namespace std;

class Position {
public:
	//creates an instance of Position with coordinates 0,0
	Position();
	//creates an instance of Position with coordinates posy,posx
    Position(int posy, int posx);
	
	//returns x component of the position
	int X() const {
		return fPosX;
	}
    
	//returns y component of the position
    int Y() const {
        return fPosY;
    }
	
	//sets the y component of the position
    void SetY(int y){
        fPosY = y;
    }
	
	//sets the x component of the position
	void SetX(int x){
        fPosX = x;
    }
	
	//sets the x and y components using integer input
    void SetPos(int y, int x){
        fPosY = y;
        fPosX = x;
    }
	
	//sets the x and y components using another position
	void SetPos(Position newPos){
        fPosY = newPos.Y();
        fPosX = newPos.X();
    }
    
	//prints the x and y component of the position
	virtual void Print() const;
    
	/*
		defines the '=' operator for position
		sets the x and y components as the same as the position inputted
	*/
    void operator = (const Position & p)
    {
        fPosY = p.Y();
		fPosX = p.X();
    }
    
	/*
		defines conditions for checking if two positions are the same
		if both x components and both y components are the same 
		then return true
	*/
    bool operator == (const Position & p)
    {
        if((fPosX == p.X())&&(fPosY == p.Y()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
	/*
		defines conditions for checking if two positions are not the same
		if either or both of the x components or the y components are 
		different then return true
	*/
    bool operator != (const Position & p)
    {
        if((fPosX != p.X())||(fPosY != p.Y()))
        {
            return true;
        }
        else
        {
        return false;
        }
    }

	/*
		defines how two positions are added together
		returns a position with the y components summed and the x components
		summed
	*/
    Position operator + (const Position & p)
	{
		Position sumPos(fPosY+p.Y(),fPosX+p.X());
		return sumPos;
	}
	
	/*
		defines how two positions are subtracted from each other
		returns a position with the y components subtracted and
		the x components subtracted
	*/
    Position operator - (const Position & p)
	{
		Position subPos(fPosY-p.Y(),fPosX-p.X());
		return subPos;
	}

private:
	int fPosX;
    int fPosY;

};

#endif /* POSITION_H_ */

