#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>

    static void dbg(String str){
        #ifdef UTIL_DBG
        Serial.println(str);
        #endif
    }

    static void dbgnl(String str){
        #ifdef UTIL_DBG
        Serial.print(str);
        #endif
    }

    static void ndbg(String str){
        #ifdef UTIL_DBGN
        Serial.println(str);
        #endif
    }

    static void ndbgnl(String str){
        #ifdef UTIL_DBGN
        Serial.print(str);
        #endif
    }

    static void log(String str){
        Serial.println(str);
    
    }

    static void lognl(String str){
        Serial.print(str);
    }





#endif // UTIL_H





