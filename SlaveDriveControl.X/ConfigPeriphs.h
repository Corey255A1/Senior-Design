/* 
 * File:   ConfigPeriphs.h
 * Author: Nicholas
 *
 * Created on March 13, 2013, 2:40 PM
 *
 * @author Nicholas Fragiskatos (Senior Design Team #12 - Robo Waiter)
 */

#include <p24EP32MC202.h>
#include "globals.h"

//-----------------------------------------------------------------------------
//  Function Defines
//-----------------------------------------------------------------------------
void configOutputCompare(void);
void configDevicePins(void);
void configInputCaptures();

//-----------------------------------------------------------------------------
//  Variable Declaration
//-----------------------------------------------------------------------------
#define CLK_PER    100