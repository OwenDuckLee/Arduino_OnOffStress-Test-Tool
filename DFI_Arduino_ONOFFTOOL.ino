/**
*Copyright (c) 2024 DFI.FAE.TC.SW. All rights reserved.
*
*This source code is licensed under the MIT license found in the
*License file in the root directory of this source tree.
***************************************************************************************************************************************************
*Author: TSD AE Owen Lee
*Date: 2024/4/29
*HW  Version: 3
*FW  Version: 3
*SF Version: 2
*PTC  Version: 2
***************************************************************************************************************************************************
*Modifications:
*add new feature - DC power test mode
***************************************************************************************************************************************************
**/

#include "config.h"
#include "OnOffTool.h"
// #include "debugLog.h"

OnOffTool DFITool(RELAY_DCONOFF_PIN, RELAY_ACONOFF_PIN, RELAY_PWBTN_PIN);
// extern unsigned long timer0_millis;

void setup()
{
	Serial.begin(115200);
    Serial.setTimeout(TIMEOUT);
    DFITool.init();

    // timer0_millis = -2988;
}

void loop()
{
    if(DFITool.isAutoRunTrigger()){
        //do auto mode stuff   
            DFITool.runCmdTrigAutoMode();
    }else{
        //do control mode stuff
        while(Serial.available()){
            DFITool.cmdRead(Serial.readString());
            DFITool.runMainCmdParser();
            DFITool.runCmdTypeIdentifier();
            DFITool.runCmdTrig();
        }  
    }
}   