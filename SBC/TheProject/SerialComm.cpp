/* 
 * File:   SerialComm.cpp
 * Author: robo-waiter
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
    logMsg = "";
}

/**
 * SerialComm class constructor.
 * @param orig
 */
SerialComm::SerialComm(const SerialComm& orig)
{
    logMsg = "";
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
    int rc;
    //const char *portToOpen = "/dev/ttyS0";
    
    //-------------------------------------------------------------------------
    //  Open the port (file) with the following configuration:
    //
    //          O_RDWR   - Reading and Writing
    //          O_NOCTTY - Port never becomes controlling terminal of process
    //          O_NDELAY - Use non-blocking I/O 
    //-------------------------------------------------------------------------
    logMsg = "Attempting to open serial communication.\n";
    WriteToLogFile(logMsg);
    commPort = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    
    //-------------------------------------------------------------------------
    //  Check to make sure the port was successfully opened.
    //-------------------------------------------------------------------------
    if (commPort == -1)
    {
        logMsg = "Error opening serial communication: ";
        logMsg.append(strerror(errno));
        logMsg.append("\n");
        WriteToLogFile(logMsg);
        return FAIL_OPEN_SERIAL;
    }
    else
    {
        logMsg = "Serial communication opened successfully.\n";
        WriteToLogFile(logMsg);
        fcntl(commPort, F_SETFL, 0);
    }
    
    //-------------------------------------------------------------------------
    //  Make the call to configure the communication port for compatibility 
    //  with the master PIC.
    //-------------------------------------------------------------------------
    rc = ConfigCommPort();
    
    if (rc != SUCCESS)
    {
        return rc;
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
        logMsg = "Error configuring comm channel: Port opened does not refer to a terminal connection\n";
        WriteToLogFile(logMsg);
        return FAIL_NO_TERM_REF;
    }
    
    //-------------------------------------------------------------------------
    //  Try and get the attributes for the terminal connection.
    //-------------------------------------------------------------------------
    if (tcgetattr(commPort, &config) < 0)
    {
        logMsg = "Error configuring comm channel: Failed to get terminal attributes.\n";
        WriteToLogFile(logMsg);
        return FAIL_GET_ATTRIB;
    }
    
    //-------------------------------------------------------------------------
    //  Configure the input flag for the termios structure as follows:
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
    //  Configure the output flag for the termios strucure as follows:
    //
    //          OCRNL   - Map carriage return to newline on output
    //          ONLCR   - Map newline to carriage return-newline on output
    //          ONLRET  - Don't output carriage return
    //          ONOCR   - Don't output carriage return at column 0
    //          ONOEOT  - 
    //          OFILL   - Send fill caracters for a delay, rather than using a
    //                    timed delay.
    //          OLCUC   - Map lowercase characters to uppercase on output.
    //          OPOST   - Enable implementation-defined output processing.
    // config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
    //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
    config.c_oflag = 0;
    
    
    //-------------------------------------------------------------------------
    //  Configure line processing flag for the termios structure as follows:
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
    //          VMIN    - Minimum number of characters for noncanonical read (1)
    //          VTIME   - Timeout in deciseconds for noncanonical read (0)
    //-------------------------------------------------------------------------
    config.c_cc[VMIN]  = 0;
    config.c_cc[VTIME] = 1;
    
    
    //-------------------------------------------------------------------------
    // Communication speed (simple version, using the predefined
    // constants)
    //-------------------------------------------------------------------------
    cfsetispeed(&config, B19200);
    cfsetospeed(&config, B19200);
    
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
void SerialComm::WritePort(unsigned char* writeBuff)
{
    int rc = write(commPort, writeBuff, strlen(reinterpret_cast <const char*>(writeBuff)));
}

/**
 * Reads the communication port for any available data.
 * @param readBuff Buffer to store incoming messages.
 * @return the number of bytes read from the port.
 */
int SerialComm::ReadPort(unsigned char* readBuff)
{
    int bytesRead;
    int loopCount = 0;
    unsigned char* buffptr = readBuff;
    while ((bytesRead = read(commPort, buffptr, 1)) > 0)
    {
        
        buffptr += bytesRead;
        unsigned char test = buffptr[-1];
        if (buffptr[-1] == '!')
        {
            buffptr[-1] = 0;
            break;
        }
        ++loopCount;
        if (loopCount >= 20)
        {
            break;
        }
        
    }
    
    return bytesRead;
}