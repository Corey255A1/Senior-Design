#include "i2c.h"
#include "9axis.h"

void init9axis(){
    i2c_Init();
    i2c_ResetBus();
    i2c_Start(acc_i2c_addr,I2C_WRITE);
    i2c_Write(0x20);
    i2c_Write(0x37);
    i2c_ResetBus();

//    i2c_Start(acc_i2c_addr,I2C_WRITE);
//    i2c_Write(0x23);
//    i2c_Write(0x40);
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

void readMag(unsigned int magArr[3]){
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

        magArr[0] = xH<<8 | xL;
        magArr[1] = yH<<8 | yL;
        magArr[2] = zH<<8 | zL;
}//end read mag

void readAcc(unsigned int accArr[6]){
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

        accArr[0] = xH<<8 | xL;
        accArr[1] = yH<<8 | yL;
        accArr[2] = zH<<8 | zL;
}

void readGyr(unsigned int gyrArr[6]){
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

        gyrArr[0] = xH<<8 | xL;
        gyrArr[1] = yH<<8 | yL;
        gyrArr[2] = zH<<8 | zL;
}



