#include "OnOffTool.h"
#include <TM1637.h>
// #include <SoftwareSerial.h>

TM1637 digitDisplay(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);
// SoftwareSerial debugSerial(2, 3); //RX , TX

OnOffTool :: OnOffTool(){}
OnOffTool :: OnOffTool (byte _DCControlPin, byte _ACControlPin, byte _PWBTNControlPin){
    DCControlPin = _DCControlPin;
    ACControlPin = _ACControlPin;
    PWBTNControlPin = _PWBTNControlPin;
}

void OnOffTool :: init(){
    pinMode(DCControlPin, OUTPUT);
    digitalWrite(DCControlPin, HIGH);

    pinMode(ACControlPin, OUTPUT);
    // digitalWrite(ACControlPin, HIGH);
    digitalWrite(ACControlPin, LOW);

    pinMode(PWBTNControlPin, OUTPUT);
    digitalWrite(PWBTNControlPin, LOW);

    digitDisplay.init();
    digitDisplay.set(BRIGHT_TYPICAL);

    digitDisplay.display(0, 0);
    digitDisplay.display(1, 0);
    digitDisplay.display(2, 0);
    digitDisplay.display(3, 0);
    
    toolState = READY;

    mainCmd.reserve(150);

    // debugSerial.begin(115200);
}

bool OnOffTool::isAutoMode(){
    if(toolMode == AUTOMODE){
        return true;
    }else{
        return false;
    }
}

bool OnOffTool::isAutoRunTrigger(){
    if(autoRunTrigger == true){
        return true;
    }else{
        return false;
    }
}

void OnOffTool :: cmdRead(const String& _cmd){
    mainCmd = _cmd;
}

void OnOffTool :: runMainCmdParser(){
    // DEBUGLOG(F("Debug message: runMainCmdParser function begin"));
    // DEBUGLOG(F("Debug message: received mainCmd = "));
    // DEBUGLOG(mainCmd);

    mainCmdParser();
    resetMainCmdParser();
    // DEBUGLOG(F("Debug message: runMainCmdParser function close"));
}

void OnOffTool :: mainCmdParser(){
    int cmdIndex;
    for(auto c : mainCmd){
        if(!isUpperCase(c) && !isSpace(c)){
            if(c == '?'){
                ++questionmarkDetectCount;
            }
        }
        else{
           ++errorCount;
        }     
    }

    if(questionmarkDetectCount == 1 && errorCount == 0){
        cmdIndex = mainCmd.indexOf('?');
        cmdType = mainCmd.substring(0, cmdIndex);
        // DEBUGLOG("Debug message: cmdType = " + cmdType);
    }
    else{
        errorCode = UNKNOWN_CMD;
        cmdType = "unknown";
        Serial.println(errorCode);
        // DEBUGLOG("Debug message: mainCmdParser function error");
    }
}

void OnOffTool :: resetMainCmdParser(){
    errorCode = EMPTY;
    errorCount = 0;
    questionmarkDetectCount = 0;
}

void OnOffTool :: runCmdTypeIdentifier(){
    // DEBUGLOG("Debug message: runCmdTypeIdentifier function begin");
    // DEBUGLOG("Debug message: cmdType = " + cmdType);

    cmdTypeIdentifier();
    resetCmdTypeIdentifier();
    // DEBUGLOG("Debug message: runCmdTypeIdentifier function close");    
}

void OnOffTool :: cmdTypeIdentifier(){
    if(cmdType == "set"){
            for(auto c : mainCmd){
                if(c !='?' && c!='&' && c!=':' && c !=',' && ispunct(c)){ 
                    ++errorCount;
                }

                if(c == '-'){
                    ++errorCount;
                } 
                
                switch(c){
                    case '&':
                        ++punctDetectCount1;
                        break;
                    case ':':
                        ++punctDetectCount2;
                        break;
                    case ',':
                        ++punctDetectCount3;
                        break;
                }
            }

            if(errorCount == 0 && punctDetectCount1 == 6 && punctDetectCount2 == 7 && punctDetectCount3 == 8){
                setValueInsert = INSERT_SUCCESS;
                status = SET_TRIG;
            }else if(errorCount == 0 && punctDetectCount1 == 9 && punctDetectCount2 == 10 && punctDetectCount3 == 11){
                setValueInsert = INSERT_SUCCESS;
                status = SET_TRIG;
            }else{
                setValueInsert = INSERT_FAIL;
                errorCode = UNKNOWN_CMD;
                Serial.print(F("set!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "reset"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = RESET_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("reset!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "clear"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = CLEAR_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("clear!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "stop"){ 
            for(auto c : mainCmd){
                if(c !='?' && c != ',' && ispunct(c)){ 
                    ++errorCount;
                } 

                if (c == ','){
                    ++punctDetectCount3;
                }  
            }

            if(errorCount == 0 && punctDetectCount3 == 2){
                status = STOP_TRIG;
            }else if(errorCount == 0 && punctDetectCount3 == 0){
                status = STOP_TRIG_IMMEDIATE;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("stop!"));
                Serial.println(errorCode);
            } 
        }
        else if(cmdType == "start"){
            for(auto c : mainCmd){
                if(c !='?' && c != ',' && ispunct(c)){ 
                    ++errorCount;
                } 

                if (c == ','){
                    ++punctDetectCount3;
                }  
            }

            if(errorCount == 0 && punctDetectCount3 == 2){
                status = START_TRIG;
            }else if(errorCount == 0 && punctDetectCount3 == 0){
                status = START_TRIG_IMMEDIATE;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("start!"));
                Serial.println(errorCode);
            }        
        }
        else if(cmdType == "ptcver"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = PTCVER_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("ptcver!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "sfver"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = SFVER_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("sfver!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "fwver"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = FWVER_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("fwver!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "hwver"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = HWVER_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("hwver!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "state"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = STATE_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("state!"));
                Serial.println(errorCode);
            }
        }
        else if(cmdType == "ping"){
            for(auto c : mainCmd){
                if(c != '?' && ispunct(c)){
                    ++errorCount;
                }
            }

            if(errorCount == 0){
                status = PING_TRIG;
            }
            else{
                errorCode = UNKNOWN_CMD;
                Serial.print(F("ping!"));
                Serial.println(errorCode);
            }
        }else if(cmdType == "unknown"){
            errorCode = UNKNOWN_CMD;
            // DEBUGLOG("Debug message: cmdTypeIdentifier function error");
        }
        else{
            errorCode = UNKNOWN_CMD;
            Serial.println(errorCode);
            // DEBUGLOG("Debug message: cmdTypeIdentifier function error");
        }

}

void OnOffTool :: resetCmdTypeIdentifier(){
    cmdType = "";
    errorCode = EMPTY;
    errorCount = 0;
    punctDetectCount1 = 0; 
    punctDetectCount2 = 0;
    punctDetectCount3 = 0;
}

void OnOffTool :: runCmdTrig(){
    // DEBUGLOG("Debug message: runCmdTrig function begin");
    // DEBUGLOG("Debug message: status = ");
    // DEBUGLOG(status);
    
    cmdTrig();
    resetCmdTrig();
    // DEBUGLOG("Debug message: runCmdTrig function close");
}

void OnOffTool:: runCmdTrigAutoMode(){
    if(curTimesValue <= testTimesValue){
        testCounterAct();
        stateCountingAct();
    }else{
        exit(0);
    }
}

void OnOffTool :: cmdTrig(){
    if(status != STANDBY){
        if(status == SET_TRIG){
            Serial.println(F("set")); 
            setTypeParser();
            setToolMode();
            toolStateIdentifier(status);
        }
        else if(status == RESET_TRIG){  
            Serial.println(F("reset"));
            resetFunc();
        }
        else if(status == CLEAR_TRIG){
            Serial.println(F("clear"));
            clearCmdValue();
            toolStateIdentifier(status);
        }
        else if(status == STOP_TRIG){
            stopTypeParser();
            unsigned long stopTime = millis();
            if(stopTimesValue > 0){
                Serial.println(F("stop"));

                while(millis() - stopTime < stopTimesValue){;}

                init();
                testCounterAct();
                toolStateIdentifier(status);
                status = STANDBY;
            }
            else{
                errorCode = CMD_PAR_ERROR;
                Serial.print(F("stop!"));
                Serial.println(errorCode);
            }
        }else if(status == STOP_TRIG_IMMEDIATE){
            Serial.println(F("stop"));
            init();
            testCounterAct();
            toolStateIdentifier(status);
            status = STANDBY;
        }
        else if(status == START_TRIG && setValueInsert == INSERT_SUCCESS){
            startTypeParser();
            unsigned long startTime = millis();
            if(startTimesValue > 0){
                Serial.println(F("start"));

                while(millis() - startTime < startTimesValue){;}

                if(isAutoMode()){
                    autoRunTrigger = true;
                }else{
                    toolStateIdentifier(status);
                    testCounterAct();
                    stateCountingAct();
                }
            }
            else{
                errorCode = CMD_PAR_ERROR;
                Serial.print(F("start!"));
                Serial.println(errorCode);
            }
        }else if(status == START_TRIG_IMMEDIATE && setValueInsert == INSERT_SUCCESS){
            Serial.println(F("start"));
            if(isAutoMode()){
                autoRunTrigger = true;
            }else{
                toolStateIdentifier(status);
                testCounterAct();
                stateCountingAct();
            }
        }
        else if(status == PTCVER_TRIG){
            if(PTCVERSION <= 0){
                errorCode = UNKNOWN_ERROR;
                Serial.print(F("ptcver!"));
                Serial.println(errorCode);
            }else{
                Serial.print(F("ptcver="));
                Serial.println(PTCVERSION);
            } 
        }
        else if(status == SFVER_TRIG){
            if(SFVERSION <= 0){
                errorCode = UNKNOWN_ERROR;
                Serial.print(F("sfver!"));
                Serial.println(errorCode);
            }else{
                Serial.print(F("sfver="));
                Serial.println(SFVERSION); 
            }
        }
        else if(status == FWVER_TRIG){
            if(FWVERSION <= 0){
                errorCode = UNKNOWN_ERROR;
                Serial.print(F("fwver!"));
                Serial.println(errorCode);
            }else{
                Serial.print(F("fwver="));    
                Serial.println(FWVERSION);            
            }
        }
        else if(status == HWVER_TRIG){
            if(HWVERSION <= 0){
                errorCode = UNKNOWN_ERROR;
                Serial.print(F("hwver!"));
                Serial.println(errorCode);
            }else{
                Serial.print(F("hwver=")); 
                Serial.println(HWVERSION);       
            }
        }
        else if(status == STATE_TRIG){
            if(toolState != NULL){
                switch(toolState){
                    case READY:
                        Serial.println(F("state=ready"));
                        break;
                    case RUN:
                        Serial.println(F("state=run"));
                        break;
                    default:
                        Serial.println(F("state=preparing"));
                        break;
                }
            }else{
                errorCode = UNKNOWN_ERROR;
                Serial.print(F("state!"));
                Serial.println(errorCode);
            }
        }
        else if(status == PING_TRIG){
            Serial.println(F("pong"));
        }
        else{
            Serial.println(CMD_INTERNAL_ERROR);
        }
    }
}

void OnOffTool :: resetCmdTrig(){
    status = STANDBY;
    errorCode = EMPTY;
    mainCmd = "";
}

void OnOffTool :: setTypeParser(){
    byte cmdIndex = mainCmd.indexOf('?');
    byte subCmdIndex1, subCmdIndex2, subCmdIndex3, subCmdIndex4, subCmdIndex5, subCmdIndex6, subCmdIndex7, subCmdIndex8, subCmdIndex9;
    subCmdIndex1= mainCmd.indexOf('&');
    testTimes = mainCmd.substring(cmdIndex+1, subCmdIndex1);
    subCmdIndex2 = mainCmd.indexOf('&', subCmdIndex1+1);
    curTimes = mainCmd.substring(subCmdIndex1+1, subCmdIndex2);
    subCmdIndex3 = mainCmd.indexOf('&', subCmdIndex2+1);
    s0inv = mainCmd.substring(subCmdIndex2+1, subCmdIndex3);
    subCmdIndex4 = mainCmd.indexOf('&', subCmdIndex3+1);
    s5inv = mainCmd.substring(subCmdIndex3+1, subCmdIndex4);
    subCmdIndex5 = mainCmd.indexOf('&', subCmdIndex4+1);
    acoffinv = mainCmd.substring(subCmdIndex4+1, subCmdIndex5);
    subCmdIndex6 = mainCmd.indexOf('&', subCmdIndex5+1);
    dcoffinv = mainCmd.substring(subCmdIndex5+1, subCmdIndex6);
    subCmdIndex7 = mainCmd.indexOf('&', subCmdIndex6+1);
    g3inv = mainCmd.substring(subCmdIndex6+1, subCmdIndex7);
    subCmdIndex8 = mainCmd.indexOf('&', subCmdIndex7+1);
    pwpress = mainCmd.substring(subCmdIndex7+1, subCmdIndex8);
    subCmdIndex9 = mainCmd.indexOf('&', subCmdIndex8+1);
    toolmode = mainCmd.substring(subCmdIndex8+1, subCmdIndex9); 
    powermode = mainCmd.substring(subCmdIndex9+1);

    testTimesSet = subCmdInfoParser(testTimes);
    curTimesSet = subCmdInfoParser(curTimes);
    s0invSet = subCmdInfoParser(s0inv);
    s5invSet = subCmdInfoParser(s5inv);
    acoffinvSet = subCmdInfoParser(acoffinv);
    dcoffinvSet = subCmdInfoParser(dcoffinv);
    g3invSet = subCmdInfoParser(g3inv);
    pwpressSet = subCmdInfoParser(pwpress);
    toolmodeSet = subCmdInfoParser(toolmode);
    powermodeSet = subCmdInfoParser(powermode);
    testTimesValue = testTimesSet.toInt();
    curTimesValue = curTimesSet.toInt();
    pwpressValue = pwpressSet.toInt();
    s0invValue = timeValueParser(s0invSet);
    s5invValue = timeValueParser(s5invSet);
    acoffinvValue = timeValueParser(acoffinvSet);
    dcoffinvValue = timeValueParser(dcoffinvSet);
    g3invValue = timeValueParser(g3invSet);
}

String OnOffTool :: subCmdInfoParser(String _subCmd){
    byte subCmdIndex;
    subCmdIndex = _subCmd.indexOf(':');
    String subCmdValue = _subCmd.substring(subCmdIndex + 1);
    return subCmdValue;
}

unsigned long OnOffTool :: timeValueParser(String _timeInfo){
        byte timeSetIndex1, timeSetIndex2, timeSetIndex3;
        timeSetIndex1 = _timeInfo.indexOf(',');
        unsigned long timeValueHr = _timeInfo.substring(0, timeSetIndex1).toInt();
        timeSetIndex2 = _timeInfo.indexOf(',', timeSetIndex1 + 1);
        unsigned long timeValueMin = _timeInfo.substring(timeSetIndex1 + 1, timeSetIndex2).toInt(); 
        timeSetIndex3 = _timeInfo.indexOf(',', timeSetIndex2 + 1);
        unsigned long timeValueSec = _timeInfo.substring(timeSetIndex2 + 1, timeSetIndex3).toInt();
        unsigned long timeValueMilisecond = _timeInfo.substring(timeSetIndex3 + 1).toInt();

        timeValueHr = timeValueHr * 3600000UL;
        timeValueMin = timeValueMin * 60000UL;
        timeValueSec = timeValueSec * 1000UL;

        unsigned long totalTimeValue = timeValueHr + timeValueMin + timeValueSec + timeValueMilisecond;
        return totalTimeValue;
}

void OnOffTool::setToolMode(){
    if(toolmodeSet == "auto"){
        toolMode = AUTOMODE;
    }else{
        toolMode = CTRLMODE;
    }

    if(powermodeSet == "dc"){
        powerMode = DCMODE;
    }else{
        powerMode = ACMODE;
    }
}

unsigned int OnOffTool :: toolStateIdentifier(unsigned int _status){
    switch(_status){
        case SET_TRIG:
            toolState = READY;
            break;
        case CLEAR_TRIG:
            toolState = READY;
            break;
        case STOP_TRIG: 
            toolState = READY;
            break;
        case STOP_TRIG_IMMEDIATE:
            toolState = READY;
            break;
        case START_TRIG:
            (testTimeCheck() ? toolState = RUN : toolState = READY);
            break;
        case START_TRIG_IMMEDIATE:
            (testTimeCheck() ? toolState = RUN : toolState = READY);
            break;
    }
    return toolState;
}

bool OnOffTool :: testTimeCheck(){
    if(curTimesValue < testTimesValue){
        return true;
    }else{
        return false;
    }
}

void OnOffTool :: stopTypeParser(){
    byte cmdIndex = mainCmd.indexOf('?');
    stopTimesSet = mainCmd.substring(cmdIndex+1);
    stopTimesValue = timeValueParser(stopTimesSet);
}

void OnOffTool :: startTypeParser(){
    byte cmdIndex = mainCmd.indexOf('?');
    startTimesSet = mainCmd.substring(cmdIndex+1);
    startTimesValue = timeValueParser(startTimesSet);
}

void OnOffTool :: testCounterAct(){
    int numDisplay, digit1, digit2, digit3, digit4;

    if(toolMode == AUTOMODE){
        numDisplay = curTimesValue++;
    }else{
        numDisplay = curTimesValue;
    }

    digit4 = numDisplay / 1000;
    numDisplay = numDisplay - (digit4 * 1000);
    digit3 = numDisplay / 100;
    numDisplay = numDisplay - (digit3 * 100);
    digit2 = numDisplay / 10;
    digit1 = numDisplay - (digit2 * 10);

    digitDisplay.display(0, digit4);
    digitDisplay.display(1, digit3);
    digitDisplay.display(2, digit2);
    digitDisplay.display(3, digit1);
}

void OnOffTool :: stateCountingAct(){
    // DEBUGLOG("Debug message: stateCountingAct function begin");
    if(s0invValue > 0){
        unsigned long s0StateTime = millis();
        unsigned long s0Pwpress = DEFAULT_S0PWPRESS;

        while(millis() - s0StateTime < s0invValue){;}
        
        if(s0Pwpress > 0){
            digitalWrite(PWBTNControlPin, HIGH);
            unsigned long pwpressStateTime = millis();

            while(millis() - pwpressStateTime < s0Pwpress){;}
            digitalWrite(PWBTNControlPin, LOW);
        }
    }

    if(isTestACMode()){
        if(s5invValue > 0 && acoffinvValue > 0){
            unsigned long s5StateTime = millis();
            while(millis() - s5StateTime < s5invValue){;}
            digitalWrite(ACControlPin, HIGH);
        }
        else if(s5invValue == 0 && acoffinvValue > 0){
            digitalWrite(ACControlPin, HIGH);
        }

        if(acoffinvValue > 0){
            unsigned long acoffStateTime = millis();
            while(millis() - acoffStateTime < acoffinvValue){;}
            digitalWrite(ACControlPin, LOW);
        }
    }else{
        if(s5invValue > 0 && dcoffinvValue > 0){
            unsigned long s5StateTime = millis();
            while(millis() - s5StateTime < s5invValue){;}
            digitalWrite(DCControlPin, LOW);
        }
        else if(s5invValue == 0 && dcoffinvValue > 0){
            digitalWrite(DCControlPin, LOW);
        }

        if(dcoffinvValue > 0){
            unsigned long dcoffinvValuePlusOffset = dcoffinvValue + OFFSET_DCOFF;
            unsigned long dcoffStateTime = millis();
            while(millis() - dcoffStateTime < dcoffinvValuePlusOffset){;}
            digitalWrite(DCControlPin, HIGH);
        }
    }

    if(g3invValue > 0){
        unsigned long g3StateTime = millis();

        while(millis() - g3StateTime < g3invValue){;}
            
        if(pwpressValue > 0){
            digitalWrite(PWBTNControlPin, HIGH);
            unsigned long pwpressStateTime = millis();

            while(millis() - pwpressStateTime < pwpressValue){;}
            digitalWrite(PWBTNControlPin, LOW);
        }
    }
    // DEBUGLOG("Debug message: stateCountingAct function close");
}

bool OnOffTool :: isTestACMode(){
     if(powerMode == ACMODE){
        return true;
     }else if(powerMode == DCMODE){
        return false;
     }
}

void OnOffTool :: clearCmdValue(){
    mainCmd = "";
    cmdType = "";
    startTimesSet = "";
    stopTimesSet = "";
    testTimes = "";
    curTimes = "";
    s0inv = "";
    s5inv = "";
    g3inv = "";
    acoffinv = "";
    dcoffinv = "";
    pwpress = "";
    testTimesSet = "";
    curTimesSet = "";
    s0invSet = "";
    s5invSet = "";
    acoffinvSet = "";
    dcoffinvSet = "";
    g3invSet = "";
    pwpressSet = "";
    startTimesValue = 0;
    stopTimesValue = 0;
    testTimesValue = 0;
    curTimesValue = 0;
    s0invValue = 0;
    s5invValue = 0;
    acoffinvValue = 0;
    dcoffinvValue = 0;
    g3invValue = 0;
    pwpressValue = 0;
    setValueInsert = INSERT_FAIL;
}
