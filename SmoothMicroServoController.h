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
        void update();
        void setGoalPos(short goalPos);  
    private:
        void _PositionControl();
        void _SmoothingInput();
        void _ComputeCurrentSpeed();
        void _LogPosData();
        void _SendPosData();
        const int _MAX_COUNT_CW = 1023;
        const int _MAX_COUNT_CCW = 0;
        int _Mode;
        int _AnalogInPin;
        int _CWp;
        int _CCWp;
        int _Dir;
        int _SkipExecCounter;
        short _CurrentPos;
        short _MinSpeed;
        short _MaxSpeed;
        short _OldCurrentPos;
        short _GoalPos;
        short _GoalSpeed;
        double _CurrentSpeed;
        double _OutVal;
        // for debugging
        short _DebugArr[3000];
        int _DebugArrCntr;
};

#endif














