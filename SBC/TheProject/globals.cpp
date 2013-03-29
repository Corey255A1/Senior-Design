/**
 * Global Variables.
 * */
#include "globals.h"

/**
 * Helper function to convert integer values to string.
 * @param number The number the caller wants to convert.
 * @return The string value of the number parameter
 */
string IntToString(int nNumber)
{
    //-------------------------------------------------------------------------
    //  - Create a string stream
    //  - Feed in the integer number
    //  - Return the string value from the stream
    //-------------------------------------------------------------------------
    stringstream stream;
    stream << nNumber;
    return stream.str();
}

/**
 * Concatenates two unsigned character arrays together
 * @param destBuff - The buffer to be concatenated on to. Also will be the
 *                      final buffer where the result is stored.
 * @param sourceBuff - The buffer that will be appended.
 * @param destBuffSize - The size of the destination buffer.
 */
void UnCharCat(unsigned char* puszDestBuff, unsigned char* puszSourceBuff, int nDestBuffSize)
{
    //-------------------------------------------------------------------------
    //  Get the total used space in each buffer.
    //-------------------------------------------------------------------------
    int destLength = strlen(reinterpret_cast <const char*>(puszDestBuff));
    int sourceLength = strlen(reinterpret_cast <const char*>(puszSourceBuff));
    
    //-------------------------------------------------------------------------
    //  If the destination buffer is large enough to handle the
    //  concatenation...
    //-------------------------------------------------------------------------
    if ((sourceLength + destLength) < nDestBuffSize)
    {
        //---------------------------------------------------------------------
        //  Loop through source buffer and append each character one-by-one.
        //---------------------------------------------------------------------
        for (int i = 0; i < sourceLength; ++i)
        {
            puszDestBuff[i + destLength] = puszSourceBuff[i];
        }
    }
}
