/**
 * Global Variables.
 * */
#include "globals.h"

/**
 * Helper function to convert integer values to string.
 * @param number The number the caller wants to convert.
 * @return The string value of the number parameter
 */
string IntToString(int number)
{
    stringstream stream;
    stream << number;
    return stream.str();
}