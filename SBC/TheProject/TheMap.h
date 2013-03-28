/* 
 * File:   TheMap.h
 * Author: corey
 *
 * Created on March 17, 2013, 2:53 PM
 */

#include <strings.h>
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
private:
    void addObject(char,int,int,int,int);
    void updateLoc(COORDINATES_DBL);
    char map[MAPSIZE][MAPSIZE];
    COORDINATES loc;
    COORDINATES_DBL locD;
    double NORTH;

};

#endif	/* THEMAP_H */

