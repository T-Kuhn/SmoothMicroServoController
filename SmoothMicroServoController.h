/*
  SmoothMircoServoController.h - This library provides a way to position control
  headless micro servos. Headless meaning, that the micro servo's microcontroller was taken out
  and replaced by an arduino. 
  KT, 2017/05/04
*/

#ifndef SmoothMicroServoController_h
#define SmoothMicroServoController_h
#include "Arduino.h"

// - - - - - - - - - - - - - - - - - - -
// - SmoothMicroServoController CLASS  -
// - - - - - - - - - - - - - - - - - - -
class SmoothMicroServoController 
{
    public:
        SmoothMicroServoController(int analogInPin, int moveCWpin, int moveCCWpin);
        void setUpMode();
        void moveCW();
        void moveCCW();
        void update();
    private:
        void debug_1();
        void logPosData();
        void sendPosData();
        const int _MAX_COUNT_CW = 1023;
        const int _MAX_COUNT_CCW = 0;
        short _DebugArr[3000];
        int _DebugArrCntr;
        int _Mode;
        int _SwipeCntr;
        int _MaxSwipes;
        int _AnalogInPin;
        int _CWp;
        int _CCWp;
        int _Cntr;
        int _MaxCount;
        int _SwitchToLow;
        int _ModOut;
        int _Dir;
};

#endif














