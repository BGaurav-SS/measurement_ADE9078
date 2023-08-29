#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <wiringPi.h>
#include "spiADE9078.h"

//Use wiringPi pin 6; physical pin 22 to send command to reset the IC.
#define RESET_PIN   6
//Use wiringPi pin 3; physical pin 15 as pin to read interrupt from IRQ1B pin.
#define IRQ1B_PIN   3

//  Configure the xIGAIN, xVGAIN, and xPGAIN registers via the SPI to calibrate the measurements.
//  x(I/V/P)GAIN = 0, xWATTOS = 0 means digital gain is 1.
//  xWATTOS = Phase X total active power offset correction for xWATT calculation.


struct initialGains {
  uint8_t avGain;
  uint8_t bvGain;
  uint8_t cvGain;

  uint8_t aiGain;
  uint8_t biGain;
  uint8_t ciGain;

  uint32_t apGain;
  uint32_t bpGain;
  uint32_t cpGain;

  uint8_t vConsel;
  uint8_t iConsel;
}; 


int initialize (void){
    //Reset ADE9078
    digitalWrite(RESET_PIN, LOW);
    delay(5);
    digitalWrite(RESET_PIN, HIGH);
    delay(5);

    printf("\nWaiting for RESET_DONE signal.\n");    
    //Wait until the RESET_DONE signal is generated.
    while (digitalRead(IRQ1B_PIN) != 0){}
    printf("RESET DONE.\n");
    return 0;
}



int main(int argc, char* argv[]){

    uint32_t data;

    if(argc <= 1){
        printf("Too few args, try %s /dev/spidev0.0\n",argv[0]);
        return -1;
    }

    wiringPiSetup();
    pinMode(RESET_PIN, OUTPUT);
    pinMode(IRQ1B_PIN, INPUT);


    if (initialize() != 0){
        printf("Error initializing the device.\n");
        return -1;
    }
    printf("Success: Device Initialization.\n\n");


    // open and configure SPI channel. (/dev/spidev0.0 for example)
    printf("Opening SPI port...\n");
    if(spi_open(argv[1]) < 0){
        printf("SPI_open failed\n");
        return -1;
    }
    printf("Success: SPI port opened.\n\n");
    delay(1000);

    // Turning the IRQ1B LED off.
    //The IRQ1B LED is lit when the device completes reset.
    if ((writeByte (ADDR_STATUS1, (1<<16))) < 0){
        return -1;
    }

    //Initialize the gain values.
    //All gains are initialized to be zero.
    initialGains gains = {0};

    //Configure the fundamental frequency to be 50Hz.
    //Set ACCMODE Bit-8 = 0
    if((writeByte (ADDR_ACCMODE, (0<<8)) < 0)){
        return -1;
    }

    //Write VLEVEL = 0x117514
    if((writeByte (ADDR_VLEVEL, 0x117514) < 0)){
        return -1;
    }

    //Write INTEN (B5) and ININTEN(B11) bit = 0 in CONFIG0 register.
    if((writeByte (ADDR_CONFIG0, (0<<5) | (0<<11))) < 0){
        return -1;
    }

    //Set ICONSEL bit and VCONSEL bit in ACCMODE register as required.
    //We wiil be running in 3-wire-single-phase
    if((writeByte (ADDR_ACCMODE, (0<<7) | (0<<6) | (0<<5) | (0<<4))) < 0){
        return -1;
    }


    //Write 1 to run register.
    
    //Write 1 to EP_CFG register.





    while (1){

        // printf ("Writing data\n\n");
        if((writeByte (ADDR_VLEVEL, 0x117514) < 0)){
            return -1;
        }
        // delay(10);


        printf ("Receiving data\n");
        if ((readByte(ADDR_VLEVEL, &data)) < 0){
            return -1 ;
        }
        printf("RECEIVED: %.2X\n\n",data);
        // delay(10);
    }
    return 0;
}