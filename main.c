#include <message.h> 
#include <stdlib.h> 
#include <stdio.h>
#include "accelerometer.h"

int main(void)
{   
    
    setup(); 

    while(1)
    {
          if(AccAvailable())
                { 
                  AccRead(); 
                  printOrientation();
                } 
    }
    
 
    
    return 0;
}
