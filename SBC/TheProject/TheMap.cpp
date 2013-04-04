/* 
 * File:   TheMap.cpp
 * @author Design Team 12
 * 
 * Created on March 17, 2013, 2:53 PM
 */

#include "TheMap.h"
#include "globals.h"
#include <stdio.h>
#include <math.h>
char map[MAPSIZE][MAPSIZE];
COORDINATES loc;
COORDINATES floorSens;
COORDINATES fridgeLoc;
int NORTH=0;

/**
 * Constructor for the TheMap class.
 */
TheMap::TheMap() 
{
    addObject(WALL,0,0,MAPSIZE,MAPSIZE);
    addObject(OBJECT,0,30,50,100);
    addObject(OBJECT,205,40,245,90);
    addObject(FRIDGE,220,160,245,205);
    addObject(FLOORSENS,155,182,156,183);
    
    destPt1.y = 95;
    destPt2.x = 190;
    destPt2.y = 95;
    destPt3.x = 190;
    destPt3.y = 202;
    
}

/**
 * Second constructor for the TheMap class.
 * @param orig
 */
TheMap::TheMap(const TheMap& orig) 
{
    
}

/**
 * Destructor for the TheMap class.
 */
TheMap::~TheMap() 
{
    
}

/**
 * Gets the current coordinates of the robot within the map.
 * @param crds - Coordinate structure to hold the current coordinates.
 */
void TheMap::getCoords(COORDINATES crds)
{
    crds.x=loc.x;
    crds.y=loc.y;
}

/**
 * Get the individual X position of the robot.
 * @return The X-Position of the robot.
 */
double TheMap::getX()
{
    return locD.x;
}

/**
 * Get the individual y position of the robot.
 * @return The Y-Position of the robot.
 */
double TheMap::getY()
{
    return locD.y;
}

/**
 * Set location of the robot
 * @param crds - Current coordinates of the robot.
 */
void TheMap::setLocation(COORDINATES crds)
{
    locD.x=crds.x;
    locD.y=crds.y;
    loc.x=crds.x;
    loc.y=crds.y;
    updateLoc(locD);          
}

/**
 * Sets up the north offset
 * @param heading
 */
void TheMap::orientNorth(double heading)
{
    NORTH=heading;
}

/**
 * Simply passes back a pointer to the map just incase
 * @param mp
 */
void TheMap::getMap(char mp[MAPSIZE][MAPSIZE])
{
    mp=map;
}

/** Takes in the heading directly from the compass of the slave pic
 * It then automatically accounts for the north offset and 
 * normalizes the map so that the robot and the map are on the same page
 */
char TheMap::move(double heading)
{
    double x;
    double y;
    x = cos(heading+NORTH);
    y = sin(heading+NORTH);
//    printf("1:Double(x,y)=%f,%f Int(x,y)=%d,%d\n",locD.x,locD.y,loc.x,loc.y);
    COORDINATES_DBL newC = {locD.x+x,locD.y+y};
    updateLoc(newC);
//    printf("2:Double(x,y)=%f,%f Int(x,y)=%d,%d\n",locD.x,locD.y,loc.x,loc.y);
    int nextX = (int)round(locD.x+x);
    int nextY = (int)round(locD.y+y);
//    printf("Next(x,y)=%d,%d\n",nextX,nextY);
    if(nextX>WALLBUFFER && nextX<MAPSIZE-WALLBUFFER &&
       nextY>WALLBUFFER && nextY<MAPSIZE-WALLBUFFER){
       return map[(int)round(locD.x+x)][(int)round(locD.y+y)];
    }else if(locD.x<=WALLBUFFER||locD.x>=MAPSIZE-WALLBUFFER||locD.y<=WALLBUFFER||locD.y>=MAPSIZE-WALLBUFFER){
        return WALL;
    }
}

/**
 * Takes in the top left and bottom right coordinates
 * and draws with in the map a box between those points.
 * The type is the type of object it will be represented
 * by in the array
 * @param type
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void TheMap::addObject(char type,int x1,int y1,int x2, int y2)
{
    int i,j;
    for(i=x1;i<x2;i++){
        map[i][y1]=type;
        map[i][y2-1]=type;
    }
    for(i=x1+1;i<x2-1;i++){
        for(j=y1+1;j<y2-1;j++)
            map[i][j]=EMPTY;
    }
    for(i=y1;i<y2;i++){
        map[x1][i]=type;
        map[x2-1][i]=type;
    }
}

/**
 * Takes in the new coordinates of the robot and 
 * moves the robot and leaves a TRACE character in it's place
 * @param crds
 */
void TheMap::updateLoc(COORDINATES_DBL crds)
{
    locD.x=crds.x;
    locD.y=crds.y;

    map[loc.x][loc.y]=TRACE;
    loc.x=round(crds.x);
    loc.y=round(crds.y);
    map[loc.x][loc.y]=ROBOT;
}
/**
 * This draws the map to the standard terminal
 * It just outputs all the characters in the map array
 * so therefore by it's nature it will display all the current
 * stats the map has recorded
 */
void TheMap::display()
{
    int x,y;
    printf("Double(x,y)=%f,%f Int(x,y)=%d,%d\n",locD.x,locD.y,loc.x,loc.y);
    printf("  ");
    for(x=0;x<MAPSIZE;x++){
            printf("%d ",x%10);
    }
    printf("\n");
    for(y=0;y<MAPSIZE;y++){
        printf("%d ",y%10);
        for(x=0;x<MAPSIZE;x++){
            printf("%c ",map[x][y]);
        }
        printf("\n");
    }
}


double TheMap::determineHeading(COORDINATES crds)
{
    int dX = loc.x - crds.x;
    int dY = loc.y - crds.y;
    double rad = atan((double)dY/(double)dX);
    if(dX>0 && dY<0){
        rad = rad+M_PI+NORTH;
    }else if(dX>0 && dY>0){
        rad = rad+M_PI+NORTH;
    }else if(dX<0 && dY>0){
        rad = rad+2*M_PI+NORTH;
    }
    if(rad>=2*M_PI){
        rad-=2*M_PI;
    }
    
    distToMove = sqrt((dX*dX) + (dY*dY));
    return rad;
}

int TheMap::getStepCount()
{
    return ((int) distToMove) * PULSES_TO_CM;
}

int TheMap::spinDirection(double destHeading)
{
    if ((dCurHeading >= 0) && (dCurHeading < (M_PI / 2)))
    {
        if ((destHeading >= (5*M_PI/4)) && (destHeading < (2*M_PI)))
        {
            return CCLKWISE;
        }
        else
        {
            return CLKWISE;
        }
    }
    else if ((dCurHeading >= (M_PI / 2)) && (dCurHeading < M_PI))
    {
        if (destHeading < dCurHeading)
        {
            return CCLKWISE;
        }
        else
        {
            return CLKWISE;
        }
    }
    else if ((dCurHeading >= M_PI) && (dCurHeading < (M_PI*3/2)))
    {
        if (destHeading < dCurHeading)
        {
            return CCLKWISE;
        }
        else
        {
            return CLKWISE;
        }
    }
    else
    {
        if ((destHeading <= (3*M_PI/4)) && (destHeading >= 0))
        {
            return CLKWISE;
        }
        else
        {
            return CCLKWISE;
        }
    }
}

void TheMap::setHeading(double newHeading)
{
    dCurHeading = newHeading;
}

int TheMap::checkCompassHeading(double curHeading)
{
    double lowThresh = curHeading - COMPASS_ERROR;
    double highThresh = curHeading + COMPASS_ERROR;

    //-------------------------------------------------------------------------
    //  Problem where the low threshold rolls back over
    //  to 2 pi
    //-------------------------------------------------------------------------
    if (lowThresh < 0)
    {
        lowThresh = (2*M_PI) + lowThresh;

        if (((curHeading >= lowThresh) && (curHeading <= (2*M_PI)))
            || ((curHeading <= highThresh) && (curHeading >= 0)))
        {
            return TRUE;
        }
    }

    //-------------------------------------------------------------------------
    //  Problem where the high threshold rolls over to
    //  0
    //-------------------------------------------------------------------------
    else if (highThresh > (2*M_PI))
    {
        highThresh = 0 + (highThresh - (2*M_PI)); 

        if (((curHeading >= lowThresh) && (curHeading <= (2*M_PI)))
            || ((curHeading >= 0) && (curHeading <= highThresh)))
        {
            return TRUE;
        }
    }
    
    //-------------------------------------------------------------------------
    //  ... Else we all good.
    //-------------------------------------------------------------------------
    else
    {
        if ((curHeading >= lowThresh) && (curHeading <= highThresh))
        {
            return TRUE;
        }
    }
    
    return FALSE;
}