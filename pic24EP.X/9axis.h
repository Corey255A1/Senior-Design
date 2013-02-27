/* 
 * File:   9axis.h
 * Author: student
 *
 * Created on February 21, 2013, 9:23 PM
 */

#define mag_i2c_addr 0x3C
#define gyr_i2c_addr 0xD0
#define acc_i2c_addr 0x30
#define mag_xH_addr 0x03
#define mag_xL_addr 0x04
#define mag_yH_addr 0x05
#define mag_yL_addr 0x06
#define mag_zH_addr 0x07
#define mag_zL_addr 0x08

#define acc_xH_addr 0x29
#define acc_xL_addr 0x28
#define acc_yH_addr 0x2B
#define acc_yL_addr 0x2A
#define acc_zH_addr 0x2D
#define acc_zL_addr 0x2C

#define gyr_xH_addr 0x1d
#define gyr_xL_addr 0x1e
#define gyr_yH_addr 0x1f
#define gyr_yL_addr 0x20
#define gyr_zH_addr 0x21
#define gyr_zL_addr 0x22

#define maxAccG 2
#define maxAccGRaw 32768
#define maxMag 2.5
#define maxMagRaw 32768

#define xaxis 0
#define yaxis 1
#define zaxis 2

void init9axis(void);
void readMag(int[]);
void readAcc(int[]);
void readGyr(int[]);
float getHeading(void);

