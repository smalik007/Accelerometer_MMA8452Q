#include <message.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <i2c.h> 
#include <panic.h> 
#include <pio.h>
#include "accelerometer.h"

  int Ax, Ay, Az ;
 long cx, cy, cz;
 

void setup(void)
{
   
  
  printf("MMA8452Q Test Code! \n");
  initAcc();

}


int initAcc(void)
{
    uint8 c =0;
  
     printf("initial c: %d\n",c);
 
    c = readRegister(WHO_AM_I); /*who am i*/
    printf("after reading accelerometer c: %d\n",c); /*should print 2A = 42 */ 

    if(c!=0x2A)  /*Communication to acc not setup */
    {
        return 0;
    }
    
    else
       standby();        
        setupPL();
        active();
        
 return 1;
}




void standby(void)
{
	uint8 c = readRegister(CTRL_REG1);
	writeRegister(CTRL_REG1, c & ~(0x01)); /*Clear the active bit to go into standby*/
}


/*SET ACTIVE MODE
	Sets the MMA8452 to active mode. Needs to be in this mode to output data*/
void active(void)
{
	uint8 c = readRegister(CTRL_REG1);
    printf("control reg b4 writing :%d\n",c);
	writeRegister(CTRL_REG1, c | 0x01); /*Set the active bit to begin detection*/
    c = readRegister(CTRL_REG1);
    printf("control reg after writing :%d\n",c);
}

uint8 AccAvailable(void)
{
	return (readRegister(STATUS) & 0x08) >> 3;
}


void AccRead(void)
{
	uint8 rawData[6];  /* x/y/z accel register data stored here */

	readRegisters(OUT_X_MSB, rawData, 6);  /* Read the six raw data registers into data array */
	
	Ax = (rawData[0]<<8 | rawData[1]) >> 4;
	Ay = (rawData[2]<<8 | rawData[3]) >> 4;
	Az = (rawData[4]<<8 | rawData[5]) >> 4;
   
     
    
    
    cx = (long) Ax / (long)(1<<11) * (long)(2.0);
	cy = (long) Ay / (long)(1<<11) * (long)(2.0);
	cz = (long) Az / (long)(1<<11) * (long)(2.0);
    
    printf("\nX: %d\tY:%d\tZ:%d\t",Ax,Ay,Az);
    
   /* printf("X: %ld\tY:%.3ld\tZ:%.3ld\n",cx,cy,cz); */
    

    
   
	
}



uint8 readRegister(uint8 reg)
{
    
    uint8 i2c_data[1]; 
    uint8 i2c_rx_data[2]; 
    i2c_data[0] = reg; 
    
    PanicZero(I2cTransfer(address_R, i2c_data, 1, i2c_rx_data, 2)); 
    return ((i2c_rx_data[0])); /* 1st byte is data and 2nd byte is ack */
    
}


void writeRegister(uint8 reg, uint8 data)
{
    uint8 i2c_data[2];  
    i2c_data[0] = reg; 
    i2c_data[1] = data;
 
    /* 
    I2cTransfer(uint16 address, const uint8 *tx, uint16 tx_len, uint8 *rx, uint16 rx_len) 
    If tx_len is non-zero and rx_len is zero then the sequence reduces to: 
    - Start condition (S). 
    - Write slave address and direction byte (address | 0). 
    - Write tx_len data bytes from the buffer at tx. 
    - Stop condition (P). 
    */ 
    PanicZero(I2cTransfer(address_W, i2c_data, 2, NULL, 0)); 
    
}



/* READ MULTIPLE REGISTERS
	Read "len" bytes from the MMA8452Q, starting at register "reg". Bytes are stored
	in "buffer" on exit.
    */
void readRegisters(uint8 reg, uint8 *buff, uint8 len)
{
    uint8 n; 
    uint8 i2c_data[1]; 
    uint8 i2c_rx_data[7]; 
    i2c_data[0] = reg; 
    
    
    /* 
    I2cTransfer(uint16 address, const uint8 *tx, uint16 tx_len, uint8 *rx, uint16 rx_len) 
    If tx_len is non-zero and rx_len is non-zero then the sequence is: 
    - Start condition (S). 
    - Write slave address and direction byte (address | 0). 
    - Write tx_len data bytes from the buffer at tx. 
    - Repeated start condition (Sr). 
    - Write slave address and direction byte (address | 1). 
    - Read rx_len bytes into the buffer at rx, acknowledging all but the final byte. 
    - Stop condition (P). 
    */ 
     PanicZero(I2cTransfer(address_R, i2c_data, 1, i2c_rx_data, 7)); 
    
   
    for(n=0; n<len; n++)
    {
        buff[n]=i2c_rx_data[n];
    }
 
      
}



void setupPL(void)
{
	/* Must be in standby mode to make changes!!!*/
	
	/* 1. Enable P/L */
	writeRegister(PL_CFG, readRegister(PL_CFG) | 0x40); /* Set PL_EN (enable) */
	/* 2. Set the debounce rate */
	writeRegister(PL_COUNT, 0x50);  /* Debounce counter at 100ms (at 800 hz) */
}


uint8 readPL(void)
{
	uint8 plStat = readRegister(PL_STATUS);
	
	if (plStat & 0x40) /* Z-tilt lockout */
		return LOCKOUT;
	else /* Otherwise return LAPO status */
		return (plStat & 0x6) >> 1;
}

void printOrientation(void)
{
  
  uint8 pl = readPL();
  switch (pl)
  {
  case PORTRAIT_U:
    printf("Portrait Up");
    break;
  case PORTRAIT_D:
    printf("Portrait Down");
    break;
  case LANDSCAPE_R:
    printf("Landscape Right");
    break;
  case LANDSCAPE_L:
    printf("Landscape Left");
    break;
  case LOCKOUT:
    printf("Flat");
    break;
  }
}
   


