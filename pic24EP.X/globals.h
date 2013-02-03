/***
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

#define U1_RPIport 47 // This is the RPI number which reads the ultra pulse
#define U1_RBIport _RB15
#define U1_RBreg TRISBbits.TRISB15
#define U1_RPOreg RPOR0bits.RP20R

#define U2_RPIport 46 // This is the RPI number which reads the ultra pulse
#define U2_RBIport _RB14
#define U2_RBreg TRISBbits.TRISB14
#define U2_RPOreg RPOR2bits.RP38R // This is the reg that outputs

extern short global_u1_edge;
extern long global_u1_time;

extern short global_u2_edge;
extern long global_u2_time;
