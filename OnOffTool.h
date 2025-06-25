#ifndef ONOFFTOOL_H
#define ONOFFTOOL_H
#include <Arduino.h>
#include "config.h"
// #include "debugLog.h"

class OnOffTool {
    private:
        byte DCControlPin;
        byte ACControlPin;
        byte PWBTNControlPin;

        int toolState = PREPARING;
        int status = STANDBY;

        int powerMode = ACMODE;
        int toolMode = CTRLMODE;
        bool autoRunTrigger = false;

        int setValueInsert = INSERT_FAIL;
        int errorCode = EMPTY;
        int questionmarkDetectCount = 0;
        int punctDetectCount1 = 0; //count for &
        int punctDetectCount2 = 0; //count for :
        int punctDetectCount3 = 0; //count for ,
        int errorCount = 0;

        String mainCmd = "";  
        String cmdType = ""; 
        String startTimesSet, stopTimesSet; //var for store cmd start and stop info. ex: 00,00,00
        String testTimes, curTimes, s0inv, s5inv, acoffinv, dcoffinv, g3inv, pwpress, toolmode, powermode; //var for store each sub-cmd. ex: s0inv:00,00,00
        String testTimesSet, curTimesSet, s0invSet, s5invSet, acoffinvSet, dcoffinvSet, g3invSet, pwpressSet, toolmodeSet, powermodeSet; //var for store each sub-cmd info. ex: 00,00,00

        //var for store sub-cmd values numbers
        unsigned long startTimesValue = 0;
        unsigned long stopTimesValue = 0;
        int testTimesValue = 0;
        int curTimesValue = 0;
        unsigned long s0invValue;
        unsigned long s5invValue;
        unsigned long acoffinvValue;
        unsigned long dcoffinvValue;
        unsigned long g3invValue;
        unsigned long pwpressValue;

    private:
        void mainCmdParser();
        void resetMainCmdParser();
        void cmdTypeIdentifier();
        void resetCmdTypeIdentifier();
        void cmdTrig();
        void resetCmdTrig();

        void setTypeParser();
        String subCmdInfoParser(String _subCmd);
        unsigned long timeValueParser(String _timeInfo);
        unsigned int toolStateIdentifier(unsigned int _status);
        bool testTimeCheck();
        void stopTypeParser();
        void startTypeParser();
        void testCounterAct();
        void stateCountingAct();
        void clearCmdValue();
        void setToolMode();
        void (* resetFunc)(void) = 0;

        bool isAutoMode(); //based input cmd to identify and set toolmode
        bool isTestACMode(); //based input cmd to identify and set test power mode
        
    public:
        OnOffTool();
        OnOffTool(byte _DCControlPin, byte _ACControlPin, byte _PWBTNControlPin);
        void init();
        bool isAutoRunTrigger();
        void cmdRead(const String& _cmd);
        void runMainCmdParser();
        void runCmdTypeIdentifier();
        void runCmdTrig();
        void runCmdTrigAutoMode();
};

#endif