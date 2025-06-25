#define __DEBUGLOG__
#ifdef __DEBUGLOG__
// You should only uncomment 1 version at a time!
// Version 1
#define DEBUGLOG(...) debugSerial.println(__VA_ARGS__)
// Version 2
//#define DEBUGLOG(...) Serial.println(__VA_ARGS__); \
                   Serial.print(" @ [SRC]:      "); \
                   Serial.println(__FILE__); \
                   Serial.print(" @ [LINE]:     "); \
                   Serial.println(__LINE__); \
                   Serial.print(" @ [FUNCTION]: "); \
                   Serial.println(__func__); 
#else
// else DEBUG is diabled
#define DEBUGLOG(...)
#endif