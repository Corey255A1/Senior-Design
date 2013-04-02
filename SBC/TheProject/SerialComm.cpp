/* 
 * File:   SerialComm.cpp
 * @author Design Team 12
 * 
 * Created on December 31, 2006, 7:33 PM
 */

#include "SerialComm.h"
#include <stdio.h>
#include <errno.h>
#include "globals.h"
#include "LogFile.h"


/**
 * SerialComm class constructor (empty).
 */
SerialComm::SerialComm()
{
    sLogMsg = "";
    packetType = JUNK;
}

/**
 * SerialComm class constructor.
 * @param orig
 */
SerialComm::SerialComm(const SerialComm& orig)
{
    sLogMsg = "";
}

/**
 * SerialComm class destructor
 */
SerialComm::~SerialComm()
{
    
}

/**
 * Opens the port to write.
 */
int SerialComm::connect()
{
    int nRC;
    //const char *portToOpen = "/dev/ttyS0";
    
    //-------------------------------------------------------------------------
    //  Open the port (file) with the following configuration:
    //
    //          O_RDWR   - Reading and Writing
    //          O_NOCTTY - Port never becomes controlling terminal of process
    //          O_NDELAY - Use non-blocking I/O 
    //-------------------------------------------------------------------------
    sLogMsg = "Attempting to open serial communication.\n";
    WriteToLogFile(sLogMsg);
    commPort = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    
    //-------------------------------------------------------------------------
    //  Check to make sure the port was successfully opened.
    //-------------------------------------------------------------------------
    if (commPort == -1)
    {
        sLogMsg = "Error opening serial communication: ";
        sLogMsg.append(strerror(errno));
        sLogMsg.append("\n");
        WriteToLogFile(sLogMsg);
        return FAIL_OPEN_SERIAL;
    }
    else
    {
        sLogMsg = "Serial communication opened successfully.\n";
        WriteToLogFile(sLogMsg);
        fcntl(commPort, F_SETFL, 0);
    }
    
    //-------------------------------------------------------------------------
    //  Make the call to configure the communication port for compatibility 
    //  with the master PIC.
    //-------------------------------------------------------------------------
    nRC = ConfigCommPort();
    
    if (nRC != SUCCESS)
    {
        return nRC;
    }
}

/**
 * Configure the communication port for compatible communication with the PIC.
 * 
 * @return SUCCESS or <fail value> 
 */
int SerialComm::ConfigCommPort()
{
    //-------------------------------------------------------------------------
    //  Test if the port we opened refers to a terminal or not
    //-------------------------------------------------------------------------
    if (!isatty(commPort))
    {
        sLogMsg = "Error configuring comm channel: Port opened does not refer to a terminal connection\n";
        WriteToLogFile(sLogMsg);
        return FAIL_NO_TERM_REF;
    }
    
    //-------------------------------------------------------------------------
    //  Try and get the attributes for the terminal connection.
    //-------------------------------------------------------------------------
    if (tcgetattr(commPort, &config) < 0)
    {
        sLogMsg = "Error configuring comm channel: Failed to get terminal attributes.\n";
        WriteToLogFile(sLogMsg);
        return FAIL_GET_ATTRIB;
    }
    
    //-------------------------------------------------------------------------
    //  Configure the input flag for the termios structure to exclude the
    //  following:
    //
    //          IGNBRK  - Ignore break condition on input.
    //          BRKINT  - Break is ignored
    //          ICRNL   - Translate carriage return to newline on input.
    //          INLCR   - Translate newline to carriage return in input
    //          PARMRK  - Prefix a character with a parity error or framing
    //                    error with \377 \0
    //          INPCK   - Enable input parity checking
    //          ISTRIP  - Strip off eighth bit
    //          IXON    - Enable XON/XOFF flow control on output
    //-------------------------------------------------------------------------    
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                        INLCR | PARMRK | INPCK | ISTRIP | IXON);
    
    //-------------------------------------------------------------------------
    //  Configure the output flag for the termios structure.
    //-------------------------------------------------------------------------
    config.c_oflag = 0;
    
    
    //-------------------------------------------------------------------------
    //  Configure line processing flag for the termios structure to disclude
    //  the following:
    //
    //          ECHO    - Echo input characters
    //          ECHONL  - Echo newline character
    //          ICANON  - Enable canonical mode.
    //          IEXTEN  - Enable implementation-defined input processing.
    //          ISIG    - When any of the characters INTR, QUIT, SUSP, or DSUSP
    //                    are received, generate the corresponding signal.
    //-------------------------------------------------------------------------
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    
    //-------------------------------------------------------------------------
    // Configure character processing for the termios structure as follows:
    //
    //          CSIZE   - Character size mask - CS8 (char is 8 bits)
    //          PARENB  - Enable parity generation on output and parity checking
    //                    on input.
    //-------------------------------------------------------------------------
    config.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
    config.c_cflag |= CS8;
    
    //-------------------------------------------------------------------------
    //  Define the terminal special characters for the termios structure:
    //
    //          VMIN    - Minimum number of characters for noncanonical read (0)
    //          VTIME   - Timeout in deciseconds for noncanonical read (1)
    //-------------------------------------------------------------------------
    config.c_cc[VMIN]  = 0;
    config.c_cc[VTIME] = 1;
    
    
    //-------------------------------------------------------------------------
    // Communication speed (simple version, using the predefined
    // constants)
    //-------------------------------------------------------------------------
    cfsetispeed(&config, B38400);
    cfsetospeed(&config, B38400);
    
    //-------------------------------------------------------------------------
    // Apply the configuration to the communication port.
    //-------------------------------------------------------------------------
    tcsetattr(commPort, TCSAFLUSH, &config);
    
    //-------------------------------------------------------------------------
    //  If control reaches here, then everything was successfully configured.
    //-------------------------------------------------------------------------
    return SUCCESS;
}

/**
 * Close the port being written to.
 */
void SerialComm::closeConnection(){
    
      close(commPort);
}

/**
 * Write the port with the data passed in by writeData.
 * @param writeData
 */
void SerialComm::WritePort(unsigned char* puszWriteBuff)
{
    int rc = write(commPort, puszWriteBuff, strlen(reinterpret_cast <const char*>(puszWriteBuff)));
}

/**
 * Reads the communication port for any available data.
 * @param readBuff Buffer to store incoming messages.
 * @return the number of bytes read from the port.
 */
int SerialComm::ReadPort(unsigned char* puszReadBuff)
{
    //-------------------------------------------------------------------------
    //  nBytesRead      - Integer variable to keep track of bytes we read
    //  puszBuffptr     - Pointer variable to progress through the buffer.
    //  cDataByteCount  - Number of Data Bytes Read
    //  cDataSize       - Number of data bytes expected in whole message
    //  cExitLoop       - Flag to exit the main function loop.
    //-------------------------------------------------------------------------
    int nBytesRead;
    unsigned char* puszBuffptr = puszReadBuff;
    char cDataByteCount = 0;
    char cDataSize = 0;
    char cExitLoop = FALSE;
    unsigned char ucCurChar;
    
    //-------------------------------------------------------------------------
    //  As long as there are messages to read, we will continued reading.
    //-------------------------------------------------------------------------
    while ((nBytesRead = read(commPort, &ucCurChar, 1)) > 0)
    {
        //---------------------------------------------------------------------
        // If we read the '!', then we know that's the start of the message,
        // and the end of any junk bytes. I.e, set the state to START.
        //---------------------------------------------------------------------
        if (ucCurChar == '!')
        {
            packetType = START;
        }
        
        //---------------------------------------------------------------------
        // Take different actions with the read bytes depending on what kind
        // of data we expect in the byte from the packet.
        //---------------------------------------------------------------------
        switch (packetType)
        {
            //-----------------------------------------------------------------
            //  We detected a '!' signifying the start of the packet reading,
            //  and we know the next byte should indicate the size (in number
            //  of bytes) of our data.
            //-----------------------------------------------------------------
            case START: 
                packetType = DATA_SIZE;
                break;
                
            //-----------------------------------------------------------------
            //  Here we take the byte and store it so we know how many bytes
            //  we expect to read next.
            //-----------------------------------------------------------------
            case DATA_SIZE:
                cDataSize = ucCurChar;
                packetType = DATA_MINE;
                break;
                
            //-----------------------------------------------------------------
            //  In this state, we are mining and storing all of the data bytes
            //-----------------------------------------------------------------
            case DATA_MINE:
                ++cDataByteCount;
                puszBuffptr[cDataByteCount - 1] = ucCurChar;
                
                //-------------------------------------------------------------
                //  If the number of data bytes read equals the data size byte
                //  from the packet, then we know we are done.
                //-------------------------------------------------------------
                if (cDataByteCount == cDataSize)
                {
                    cExitLoop = TRUE;
                    packetType = JUNK;
                }
                break;
                
            //-----------------------------------------------------------------
            //  Default state - here we assume all of the bytes are junk up
            //  until we recieve the character that signifies the start of the
            //  message.
            //-----------------------------------------------------------------
            case JUNK:
                break;
        }
        //---------------------------------------------------------------------
        //  Break the loop as to return from the function when told.
        //---------------------------------------------------------------------
        if (cExitLoop)
        {
            break;
        }
//        puszBuffptr += nBytesRead;
//        if (puszBuffptr[-1] == '!')
//        {
//            puszBuffptr[-1] = 0;
//            break;
//        }
        
    }
    
    return nBytesRead;
}