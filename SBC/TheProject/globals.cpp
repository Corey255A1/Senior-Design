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

void uCharCat(unsigned char* destBuff, unsigned char* sourceBuff, int destBuffSize)
{
    int destLength = strlen(reinterpret_cast <const char*>(destBuff));
    int sourceLength = strlen(reinterpret_cast <const char*>(sourceBuff));
    int destSize = sizeof(destBuff);
    if ((sourceLength + destLength) < destLength)
    {
        for (int i = 0; i < sourceLength; ++i)
        {
            destBuff[i + destLength] = sourceBuff[i];
        }
    }
}