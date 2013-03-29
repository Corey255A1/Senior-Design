/* 
 * File:   MessageBuilder.cpp
 * @author Design Team 12
 *
 * Created on March 26, 2013, 1:23 PM
 */

#include "MessageBuilder.h"
#include "string.h"

void ResetMessage(unsigned char* msgBuff)
{
    int msgByteCount = strlen(reinterpret_cast <const char*>(msgBuff));
    memset(msgBuff, 0, msgByteCount);
}

