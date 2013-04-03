/* 
 * File:   TheMap.h
 * @author Design Team 12
 *
 * Created on March 17, 2013, 2:53 PM
 * 
 * Header for the TheMap class. The class has the ability to maintain and
 * update (in close to real time) the map of the surroundings for RoboWaiter.
 */

//-----------------------------------------------------------------------------
//  Include directories.
//-----------------------------------------------------------------------------
#include <strings.h>

//-----------------------------------------------------------------------------
//  Variable definitions.
//-----------------------------------------------------------------------------
#ifndef THEMAP_H
#define	THEMAP_H
#define MAPSIZE (245)
#define EMPTY ' '
#define OBJECT 'O'
#define WALL '*'
#define ROBOT 'X'
#define TRACE 'T'
#define FRIDGE 'R'
#define FLOORSENS 'F'
#define FLRSENSX 155
#define FLRSENSY 182
#define WALLBUFFER (5)
#define CLKWISE 0
#define CCLKWISE 1

//-----------------------------------------------------------------------------
//  Class variable and function declearations.
//-----------------------------------------------------------------------------
typedef struct{
    int x;
    int y;            
}COORDINATES;
typedef struct{
    double x;
    double y;            
}COORDINATES_DBL;
class TheMap {
public:
    TheMap();
    TheMap(const TheMap& orig);
    virtual ~TheMap();
    void orientNorth( double );
    void setLocation( COORDINATES );
    char move( double );
    void getCoords( COORDINATES );
    double getX();
    double getY();
    void display(void);
    void getMap(char[MAPSIZE][MAPSIZE]);
    double determineHeading( COORDINATES );
    int spinDirection(double destHeading);
    void setHeading(double newHeading);
    int getStepCount();
    int checkCompassHeading(double curHeading);
    
    COORDINATES destPt1;
    COORDINATES destPt2;
    COORDINATES destPt3;
private:
    void addObject(char,int,int,int,int);
    void updateLoc(COORDINATES_DBL);
    char map[MAPSIZE][MAPSIZE];
    COORDINATES loc;
    COORDINATES_DBL locD;
    double NORTH;
    double dCurHeading;
    int distToMove;

};

#endif	/* THEMAP_H */

