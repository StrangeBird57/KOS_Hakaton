#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>
#include <bsp/bsp.h>
#include <sys/cdefs.h>
#include <rtl/countof.h>
#include <string.h>
#include <iostream>
#include "gpio.h"

#define GPIO_PIN_NUM    RTL_UINT32_C(28)
#define DELAY_S         2
#define HW_MODULE_NAME  "gpio0"
#define HW_MODULE_CFG   "raspberry_pi4b.default"

#define GPIO_PIN_NUM_IN1 12U
#define GPIO_PIN_NUM_IN2 13U
#define GPIO_PIN_NUM_IN3 20U
#define GPIO_PIN_NUM_IN4 21U
#define GPIO_PIN_NUM_ENA 6U
#define GPIO_PIN_NUM_ENB 26U

#define HIGH 1
#define LOW  0



int forward(GpioHandle* handle)
{
    GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);  
    return 0;
}

int backward(GpioHandle* handle)
{
    GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
    return 0;
}

int stop(GpioHandle* handle)
{
    GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, LOW);
    return 0;
}

int left(GpioHandle* handle)
{
    GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
return 0;
}

int right(GpioHandle* handle)
{
    GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
    return 0;
}

int gpiomain(int argc, char **argv)
{
    int returnValue = EXIT_FAILURE;
    Retcode rc = rcFail;
    GpioHandle handle = GPIO_INVALID_HANDLE;
   
    //rc = BspInit(NULL);
    if (rc != BSP_EOK)
    {
		return EXIT_FAILURE;
    }

	rc = BspSetConfig("gpio0", "raspberry_pi4b.default");
    if (rcOk != rc)
    {
	    return EXIT_FAILURE;
    }
   	
    rc = GpioInit();
    if (rcOk != rc)
    {
		return EXIT_FAILURE;
    }    

    if (rcOk == rc)
    {
        rc = GpioOpenPort(HW_MODULE_NAME, &handle);
        if (rcOk != rc)
        {
			return EXIT_FAILURE;
        }
        else if (GPIO_INVALID_HANDLE == handle)
        {
			return EXIT_FAILURE;
        }
    }

    if (rcOk == rc)
    {
        GpioSetMode(handle, GPIO_PIN_NUM_IN1, GPIO_DIR_OUT);
        GpioSetMode(handle, GPIO_PIN_NUM_IN2, GPIO_DIR_OUT);
        GpioSetMode(handle, GPIO_PIN_NUM_IN3, GPIO_DIR_OUT);
        GpioSetMode(handle, GPIO_PIN_NUM_IN4, GPIO_DIR_OUT);
        GpioSetMode(handle, GPIO_PIN_NUM_ENA, GPIO_DIR_OUT);
        GpioSetMode(handle, GPIO_PIN_NUM_ENB, GPIO_DIR_OUT);            
    
		if(argc > 0)
		{
			GpioHandle* my_handle = &handle;

			char* command = argv[0];
			unsigned int time_move =static_cast<unsigned int>(*argv[1]);

			if (strcmp(command,"forward")==0)
			{
				forward(my_handle);
				sleep(time_move);
				stop(my_handle);
			}
			if (strcmp(command,"backward")==0)
			{
				backward(my_handle);
				sleep(time_move);
				stop(my_handle);
			}
			if (strcmp(command,"right")==0)
			{
				right(my_handle);
				sleep(time_move);
				stop(my_handle);
			}

			if (strcmp(command,"left")==0)
			{
				left(my_handle);
				sleep(time_move);
				stop(my_handle);
			}

			if (strcmp(command,"stop")==0)
			{				
				stop(my_handle);
			}
		}     
       
        
    }
	  
    if (GPIO_INVALID_HANDLE != handle)
    {
        Retcode tempRc = GpioClosePort(handle);
        if (rcOk != tempRc)
        {
            fprintf(stderr,"GpioClosePort for %s port failed, error code: %d.\n",HW_MODULE_NAME, RC_GET_CODE(tempRc));
            rc = tempRc;
        }
    }

    if (rcOk == rc)
    {       
        returnValue = EXIT_SUCCESS;
    }
   
    return returnValue;
}
