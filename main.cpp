/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "platform/mbed_thread.h"
#include "htu21d.h"
#include <iostream>
#include <iomanip>

using namespace std;

#define SDA     PTC11 
#define SCL     PTC10

Serial pc(USBTX, USBRX);                    //local terminal
htu21d htu(SDA, SCL);                       //Temp Hum || sda, scl
 
float H21Temp = 0.0;                        //Temperture from HTU21D
float H21Hum  = 0.0;                        //Humidity from HTU21D
float H21Dew  = 0.0;                        //Dew Point from HTU21D

int main()
{
    pc.baud(115200);                        //local terminal baud
    //pc.printf("\r\n\r\nK64F_HTU21D basic operation\r\n");
    cout << "K64F_HTU21D basic operation" << endl;
 
    //initialize the HTU21D
    int htu21 = htu.softReset();
    if(htu21 == 0) {
        pc.printf(" - HTU21D broken...\r\n");
    } else {
        uint8_t HTU21DuserReg = htu.getUserReg();
        pc.printf("HTU21D UserReg: 0x%02x   SN: 0x%04x %08x %04x\r\n", 
                  HTU21DuserReg, htu.HTU21sn.HTU21D_sna, htu.HTU21sn.HTU21D_snb, 
                  htu.HTU21sn.HTU21D_snc);
    }
 
    while(true) {
        //get humidity, temperature and dew point from HTU21D
        if(htu21 == 1) {    //if HTU21D didn't initialize, don't access HTU21D anymore
            H21Hum = htu.getHum();
            if((double)H21Hum == 255.0) pc.printf("\r\n*** HTU21D Hum error!!\r\n");
            H21Temp = htu.getTemp();
            if((double)H21Temp == 255.0) pc.printf("\r\n*** HTU21D Temp error!!\r\n");
            H21Dew = htu.getDewPtFast();
        }
        //pc.printf("Temp: %7.2f C  Hum: %4.1f %%   DewPt: %7.2f C\r\n", H21Temp, H21Hum, H21Dew);
        cout << "Temp: "  << setprecision(4) << H21Temp << "C\t" 
             << "Hum: "   << setprecision(4) << H21Hum << "%\t" 
             << "DewPt: " << setprecision(4) << H21Dew << "C" 
             << endl;
        thread_sleep_for(1000);
        //wait(1.0); //Adiciono este texto por aquí...
   }

}