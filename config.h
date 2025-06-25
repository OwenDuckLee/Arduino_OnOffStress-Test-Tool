#ifndef CONFIG_h
#define CONFIG_h

#define RELAY_DCONOFF_PIN 5
#define RELAY_ACONOFF_PIN 6
#define RELAY_PWBTN_PIN 7
#define DISPLAY_DIO_PIN 8
#define DISPLAY_CLK_PIN 9

#define HWVERSION 3
#define FWVERSION 3
#define SFVERSION 2
#define PTCVERSION 2

#define DCMODE 13
#define ACMODE 14
#define CTRLMODE 15
#define AUTOMODE 16

#define PREPARING 17
#define READY 18
#define RUN 19

#define STANDBY 20
#define PING_TRIG 21
#define STATE_TRIG 22
#define HWVER_TRIG 23
#define FWVER_TRIG 24
#define SFVER_TRIG 25
#define PTCVER_TRIG 26
#define START_TRIG 27
#define START_TRIG_IMMEDIATE 28
#define STOP_TRIG 29
#define STOP_TRIG_IMMEDIATE 30
#define CLEAR_TRIG 31
#define RESET_TRIG 32
#define SET_TRIG 33

#define INSERT_FAIL 35
#define INSERT_SUCCESS 36

#define UNKNOWN_ERROR 1000 //represent unknown error
#define UNKNOWN_CMD 1001 //represent unknown cmd
#define CMD_PAR_ERROR 1002 //represent cmd parameter error
#define CMD_INTERNAL_ERROR 1003 //represent cmd interal error while executing command
#define EMPTY 0

#define DEFAULT_S0PWPRESS 100 
#define TIMEOUT 100
#define OFFSET_DCOFF 65 //set offset milliseconds for DCtoDC SSR

#endif