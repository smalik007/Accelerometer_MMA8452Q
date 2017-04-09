#ifndef __ACCELEROMETER_H
#define __ACCELEROMETER_H


/* To use i2c set the PSKEY "I2C_SCL = 0006 and I2C_SDA = 0007 " using PSTool*/
/*Address in this code is for accelerometer MMA8452Q  , can be changed in this file for other*/



#define I2C_SCL                 (1<<6)
#define I2C_SDA                 (1<<7)


#define address_R (0x3B)  /*Device address in reading mode see datasheet pg.17 MMA8452Q  (for LIS2HH12 see datasheet and for rest below addresses)  */
#define address_W (0x3A)  /*Device address in writing mode */
#define WHO_AM_I (0x0D)
#define XYZ_DATA_CFG (0x0E)
#define CTRL_REG1 (0x2A)
#define OUT_X_MSB (0x01)
#define STATUS (0x00)
#define PL_STATUS (0x10) /* see IG_SRC1 at pg.39 in LIS2HH12 */
#define	PL_CFG (0x11)    /* see IG_CFG1 at pg.39 in LIS2HH12 */
#define	PL_COUNT (0x12)  /* see IG_DUR1 at pg.40 in LIS2HH12 */




/* Possible portrait/landscape settings */

#define PORTRAIT_U 0
#define PORTRAIT_D 1
#define LANDSCAPE_R 2
#define LANDSCAPE_L 3
#define LOCKOUT 0x40
  
void standby(void);
void setupPL(void);
uint8 readPL(void);
void printOrientation(void);

void setup(void);
int initAcc(void);
void active(void);



uint8 AccAvailable(void);
void AccRead(void);

uint8 readRegister(uint8 reg);
void writeRegister(uint8 reg, uint8 data);
void readRegisters(uint8 reg, uint8 *buff, uint8 len);

#endif


