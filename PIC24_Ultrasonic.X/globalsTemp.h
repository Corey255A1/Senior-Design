/* 
 * File:   globalsTemp.h
 * Author: Dan
 *
 * Created on March 12, 2013, 5:24 PM
 */
/**
 * -- Global Header --
 * Any global value used should be declared with in this
 * file so that they can be found quickly and easily
 * Declarations are also found with in this header file
 * Making it easy to know where stuff is defined.
 */

#include <p24EP32MC202.h>

#define RISE 1
#define FALL 0
#define INPUT 1
#define OUTPUT 0
#define OC1port 0b010000
#define HIGH 1
#define LOW 0
#define VINx100 330
#define BIT_10 1023
#define LOCKED 1
#define UNLOCKED 0
#define DISABLE 0
#define CLEAR 0
#define EN 1
#define DIS 0
#define pi 3.141592653589

extern short global_u1_edge;
extern long global_u1_time;

extern short global_u2_edge;
extern long global_u2_time;

extern int global_temp;

// THESE WILL NEED TO BE ADDED TO MAIN GLOBALS HEADER FILE FOR MAIN PROJECT
extern short global_front1_edge;
extern long global_front1_time;

extern short global_front2_edge;
extern long global_front2_time;
