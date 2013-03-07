#include <p24FJ128GA010.h>
#include "lcd.h"
#include "globals.h"
#include <string.h>

/**
 * initPMP() initializes the parallel master port
 * This is essential for use by the LCD
 */
void initPMP(void){
    PMCON=0x83BF;
    PMMODE = 0x3FF;
    PMAEN = 0x0001;
}

void initLCD(unsigned char cursor){
    msDelay(100);
    PMADDR=0;
    PMDIN1 = 0b00111100;//8bit mode
    PMADDR = 0x8000; //chip select enable
    msDelay(30);
    PMADDR = 0;
    PMDIN1 = 0b00001111;//cursor on
    PMADDR = 0x8000; //chip select enable
    _10usDelay(4); //macro for 40us
    PMADDR=0;

    PMDIN1 = 0b00000010; //return home
    PMADDR = 0x8000; //chip select enable
    msDelay(2); //macro for 2ms
    PMADDR=0;

    PMDIN1 = 0b00000110; //entry mode no shifting
    PMADDR = 0x8000; //chip select enable
    _10usDelay(4); //macro for 40us
    PMADDR = 0;
    PMDIN1 = 0b00001000 | cursor;
}

/**
 * This is for sending LCD specific control commands
 * @param cmd - the command to be sent
 */
void cmdLCD(char cmd){
    PMDIN1 = cmd;
    PMADDR = 0x8000; //chip select enable
    if(cmd & 1 || cmd & 2){
        msDelay(2);//macro for 2ms
    }
    else{
        msDelay(2); //macro for 40us
    }//end if
    PMADDR=0;
}//cmdlcd

/**
 * This prints a character to the LCD screen
 * The LCD is setup for auto advance mode
 * @param character
 */
void putLCD(char character){
    PMDIN1 = character;
    PMADDR = 0x8001; //enable and RS
    _10usDelay(4); //macro for 40us
}

/**
 * reads in a character array and prints it to the screen
 * @param string - the string to write to the screen
 */
void writeLCD(char string[]){
    int strlength = strlen(string);
    int i=0;
    for(i=0;i<strlength;i++){
        putLCD(string[i]);
    }
}
