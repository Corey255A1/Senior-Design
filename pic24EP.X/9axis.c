#include "i2c.h"
#include "9axis.h"
#include "math.h"

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
}

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

float getHeading( void ){
    int acc[3];
    int mag[3];
    float acc_norm[3];
    float mag_norm[3];
    readAcc(acc);
    readMag(mag);
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
    return atan2f(Yh, Xh);
}



