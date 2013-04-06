/**
 * --9 axis--
 * The 9 axis code provides an interface for communicating with the module
 * It can read each piece individually or just read the current calculated
 * heading.
 *
 */

#include "i2c.h"
#include "9axis.h"
#include "math.h"
#include "globals.h"


int global_mag[3];
int global_acc[3];
int global_gyr[3];
int neinAxis_semaphore=0;
int f_UPDATE_9AXIS=0;
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
/* Interrupt Service Routine code goes here */
_T4IF = 0; //Clear Timer4 interrupt flag
//if(neinAxis_semaphore==UNLOCKED){
f_UPDATE_9AXIS=1;
//}

}

/**
 * Setsup the 9 axis with the appropriate intial settings
 * --todo remember these settings
 */
void init9axis(){
    i2c_Init();
    i2c_ResetBus();
    i2c_Start(acc_i2c_addr,I2C_WRITE);
    i2c_Write(0x20);
    i2c_Write(0x2F);
    i2c_ResetBus();

//    i2c_Start(acc_i2c_addr,I2C_WRITE);
//    i2c_Write(0x23);
//    i2c_Write(0xC0);
//    i2c_ResetBus();

    i2c_Start(mag_i2c_addr,I2C_WRITE);
    i2c_Write(0x00);
    i2c_Write(0x08);
    i2c_ResetBus();

    i2c_Start(mag_i2c_addr,I2C_WRITE);
    i2c_Write(0x01);
    i2c_Write(0x60);
    i2c_ResetBus();

    i2c_Start(mag_i2c_addr,I2C_WRITE);
    i2c_Write(0x02);
    i2c_Write(0x00);
    i2c_ResetBus();

    i2c_Start(gyr_i2c_addr,I2C_WRITE);
    i2c_Write(0x16);
    i2c_Write(0x18);
    i2c_ResetBus();

    //Setup Timer4 to be used as a trigger for information collection
//    TMR4 =0;
//    T4CON=0;
//    T4CONbits.TCKPS = 0b11; // 1:8 for now
//    _T4IP = 0x01; // Set Timer4 Interrupt Priority Level
//    IFS1bits.T4IF = 0; // Clear Timer4 Interrupt Flag
//    IEC1bits.T4IE = 1; // Enable Timer4 interrupt
//    T4CONbits.TON = 1; // turn on T4
}

/**
 * Reads the Magnetic value and stores its X,Y,Z into the
 * array
 * @param magArr
 */
void readMag(int magArr[3]){
        i2c_Start(mag_i2c_addr,I2C_WRITE);
        i2c_Write(mag_xH_addr);
        i2c_Restart(mag_i2c_addr,I2C_READ);
        unsigned int xH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(mag_i2c_addr,I2C_WRITE);
        i2c_Write(mag_xL_addr);
        i2c_Restart(mag_i2c_addr,I2C_READ);
        unsigned int xL = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(mag_i2c_addr,I2C_WRITE);
        i2c_Write(mag_yH_addr);
        i2c_Restart(mag_i2c_addr,I2C_READ);
        unsigned int yH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(mag_i2c_addr,I2C_WRITE);
        i2c_Write(mag_yL_addr);
        i2c_Restart(mag_i2c_addr,I2C_READ);
        unsigned int yL = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(mag_i2c_addr,I2C_WRITE);
        i2c_Write(mag_zH_addr);
        i2c_Restart(mag_i2c_addr,I2C_READ);
        unsigned int zH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(mag_i2c_addr,I2C_WRITE);
        i2c_Write(mag_zL_addr);
        i2c_Restart(mag_i2c_addr,I2C_READ);
        unsigned int zL = i2c_ReadFinal();
        i2c_ResetBus();

        magArr[0] = (xH<<8) | xL;
        magArr[1] = (yH<<8) | yL;
        magArr[2] = (zH<<8) | zL;
}//end read mag

/**
 * Reads the Acceleration value and stores its X,Y,Z into the
 * array
 * @param magArr
 */
void readAcc(int accArr[6]){
        i2c_Start(acc_i2c_addr,I2C_WRITE);
        i2c_Write(acc_xH_addr);
        i2c_Restart(acc_i2c_addr,I2C_READ);
        unsigned int xH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(acc_i2c_addr,I2C_WRITE);
        i2c_Write(acc_xL_addr);
        i2c_Restart(acc_i2c_addr,I2C_READ);
        unsigned int xL = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(acc_i2c_addr,I2C_WRITE);
        i2c_Write(acc_yH_addr);
        i2c_Restart(acc_i2c_addr,I2C_READ);
        unsigned int yH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(acc_i2c_addr,I2C_WRITE);
        i2c_Write(acc_yL_addr);
        i2c_Restart(acc_i2c_addr,I2C_READ);
        unsigned int yL = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(acc_i2c_addr,I2C_WRITE);
        i2c_Write(acc_zH_addr);
        i2c_Restart(acc_i2c_addr,I2C_READ);
        unsigned int zH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(acc_i2c_addr,I2C_WRITE);
        i2c_Write(acc_zL_addr);
        i2c_Restart(acc_i2c_addr,I2C_READ);
        unsigned int zL = i2c_ReadFinal();
        i2c_ResetBus();

        accArr[0] = (xH<<8) + xL;
        accArr[1] = (yH<<8) + yL;
        accArr[2] = (zH<<8) + zL;
}
/**
 * Reads the Gyroscope value and stores its X,Y,Z into the
 * array
 * @param magArr
 */
void readGyr(int gyrArr[6]){
        i2c_Start(gyr_i2c_addr,I2C_WRITE);
        i2c_Write(gyr_xH_addr);
        i2c_Restart(gyr_i2c_addr,I2C_READ);
        unsigned int xH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(gyr_i2c_addr,I2C_WRITE);
        i2c_Write(gyr_xL_addr);
        i2c_Restart(gyr_i2c_addr,I2C_READ);
        unsigned int xL = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(gyr_i2c_addr,I2C_WRITE);
        i2c_Write(gyr_yH_addr);
        i2c_Restart(gyr_i2c_addr,I2C_READ);
        unsigned int yH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(gyr_i2c_addr,I2C_WRITE);
        i2c_Write(gyr_yL_addr);
        i2c_Restart(gyr_i2c_addr,I2C_READ);
        unsigned int yL = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(gyr_i2c_addr,I2C_WRITE);
        i2c_Write(gyr_zH_addr);
        i2c_Restart(gyr_i2c_addr,I2C_READ);
        unsigned int zH = i2c_ReadFinal();
        i2c_ResetBus();

        i2c_Start(gyr_i2c_addr,I2C_WRITE);
        i2c_Write(gyr_zL_addr);
        i2c_Restart(gyr_i2c_addr,I2C_READ);
        unsigned int zL = i2c_ReadFinal();
        i2c_ResetBus();

        gyrArr[0] = (xH<<8) | xL;
        gyrArr[1] = (yH<<8) | yL;
        gyrArr[2] = (zH<<8) | zL;
}

/**
 * Calculates the heading based on the tilt compsenation algorithm
 *
 * @return a value between -pi and pi
 */
float getHeading( void ){
    neinAxis_semaphore = LOCKED;
    int acc[3];
    acc[xaxis] = global_acc[xaxis];
    acc[yaxis] = global_acc[yaxis];
    acc[zaxis] = global_acc[zaxis];
    int mag[3];
    mag[xaxis] = global_mag[xaxis];
    mag[yaxis] = global_mag[yaxis];
    mag[zaxis] = global_mag[zaxis];
    float acc_norm[3];
    float mag_norm[3];
    //readAcc(acc);
    //readMag(mag);
    acc_norm[xaxis]=maxAccG*((float) acc[xaxis]/maxAccGRaw);
    acc_norm[yaxis]=maxAccG*((float) acc[yaxis]/maxAccGRaw);
    acc_norm[zaxis]=maxAccG*((float) acc[zaxis]/maxAccGRaw);
    
    mag_norm[xaxis]=maxMag*((float) mag[xaxis]/maxMagRaw);
    mag_norm[yaxis]=maxMag*((float) mag[yaxis]/maxMagRaw);
    mag_norm[zaxis]=maxMag*((float) mag[zaxis]/maxMagRaw);

    float pitch = asin(acc_norm[xaxis]);
    float roll = asin(acc_norm[yaxis]);


    float cosRoll = cos(roll);
    float sinRoll = sin(roll);
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);


    float Xh = mag_norm[xaxis] * cosPitch + mag_norm[zaxis] * sinPitch;
    float Yh = mag_norm[xaxis] * sinRoll * sinPitch + mag_norm[yaxis] * cosRoll - mag_norm[zaxis] * sinRoll * cosPitch;
    neinAxis_semaphore = UNLOCKED;
    return atan2f(Yh, Xh)+pi;
}
//force a read and update the global values
float updateHeading( void ){
    neinAxis_semaphore = LOCKED;
    readMag(global_mag);
    readAcc(global_acc);
    readGyr(global_gyr);
    int acc[3];
    acc[xaxis] = global_acc[xaxis];
    acc[yaxis] = global_acc[yaxis];
    acc[zaxis] = global_acc[zaxis];
    int mag[3];
    mag[xaxis] = global_mag[xaxis];
    mag[yaxis] = global_mag[yaxis];
    mag[zaxis] = global_mag[zaxis];
    float acc_norm[3];
    float mag_norm[3];
    //readAcc(acc);
    //readMag(mag);
    acc_norm[xaxis]=maxAccG*((float) acc[xaxis]/maxAccGRaw);
    acc_norm[yaxis]=maxAccG*((float) acc[yaxis]/maxAccGRaw);
    acc_norm[zaxis]=maxAccG*((float) acc[zaxis]/maxAccGRaw);

    mag_norm[xaxis]=maxMag*((float) mag[xaxis]/maxMagRaw);
    mag_norm[yaxis]=maxMag*((float) mag[yaxis]/maxMagRaw);
    mag_norm[zaxis]=maxMag*((float) mag[zaxis]/maxMagRaw);

    float pitch = asin(acc_norm[xaxis]);
    float roll = asin(acc_norm[yaxis]);


    float cosRoll = cos(roll);
    float sinRoll = sin(roll);
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);


    float Xh = mag_norm[xaxis] * cosPitch + mag_norm[zaxis] * sinPitch;
    float Yh = mag_norm[xaxis] * sinRoll * sinPitch + mag_norm[yaxis] * cosRoll - mag_norm[zaxis] * sinRoll * cosPitch;
    neinAxis_semaphore = UNLOCKED;
    return atan2f(Yh, Xh)+pi;
}



