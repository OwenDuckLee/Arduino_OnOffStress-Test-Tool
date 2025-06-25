/**
*Copyright (c) 2024 OwenLee All rights reserved.
*
*This source code is licensed under the MIT license found in the
*License file in the root directory of this source tree.
***************************************************************************************************************************************************
*Author: Owen Lee
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

OnOffTool tool(RELAY_DCONOFF_PIN, RELAY_ACONOFF_PIN, RELAY_PWBTN_PIN);
// extern unsigned long timer0_millis;

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(TIMEOUT);
    tool.init();

    // timer0_millis = -2988;
}

void loop()
{
    if(tool.isAutoRunTrigger()){
        //do auto mode stuff   
            tool.runCmdTrigAutoMode();
    }else{
        //do control mode stuff
        while(Serial.available()){
            tool.cmdRead(Serial.readString());
            tool.runMainCmdParser();
            tool.runCmdTypeIdentifier();
            tool.runCmdTrig();
        }  
    }
}   
