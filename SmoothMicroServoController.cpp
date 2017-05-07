/*
  SmoothMircoServoController.h - This library provides a way to position control
  headless micro servos. Headless meaning, that the micro servo's microcontroller was taken out
  and replaced by an arduino. 
  KT, 2017/05/04
*/

#include "Arduino.h"
#include "SmoothMicroServoController.h"
#include <math.h>

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - CONSTRUCTOR - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
SmoothMicroServoController::SmoothMicroServoController(int analogInPin, int moveCWpin, int moveCCWpin)
{
    _AnalogInPin = analogInPin;
    _CWp = moveCWpin;
    _CCWp = moveCCWpin;
    
    pinMode(_CWp, OUTPUT);
    pinMode(_CCWp, OUTPUT);

    _Mode = 0;
    _SwipeCntr = 0;
    _MaxSwipes = 2;
    _Cntr = 1;
    _MaxCount = 20;
    _SwitchToLow = 8;

    _Dir = false;

    _DebugArrCntr = 0;
    for (int i=0; i < 3000; i++){
        _DebugArr[i] = 0;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - MoveCW  - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::moveCW()
{
    if (analogRead(_AnalogInPin) < _MAX_COUNT_CW - 200 ) {
        digitalWrite(_CCWp, LOW); 
        digitalWrite(_CWp, _ModOut); 
    } else {
        digitalWrite(_CWp, HIGH); 
        digitalWrite(_CCWp, HIGH); 
        delay(200);
        _SwipeCntr++;
        _Dir = false;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - MoveCCW  - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::moveCCW()
{
    if (analogRead(_AnalogInPin) > _MAX_COUNT_CCW + 200 ) {
        digitalWrite(_CWp, LOW); 
        digitalWrite(_CCWp, _ModOut); 
    } else {
        digitalWrite(_CWp, HIGH); 
        digitalWrite(_CCWp, HIGH); 
        delay(200);
        _SwipeCntr++;
        _Dir = true;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - Debug_1 - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::debug_1()
{
    if (_SwipeCntr >= _MaxSwipes) {
        sendPosData();
        _SwipeCntr = 0;
        _Mode = 0;
        setUpMode();
        return;
    }

    if (_Dir) {
        moveCW(); 
    } else {
        moveCCW();
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - SetUpMode - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::setUpMode()
{
    char inByte;
    Serial.println("Choose your mode: ");   // send an initial string
    Serial.println("1: debug_1");   
    Serial.println("2: debug_2");   

    while (Serial.available() <= 0) {
        delay(300);
    }

    inByte = Serial.read();
    if (inByte == '1') {
        _Mode = 1;
        Serial.println("mode set to debug_1");    
    }
    if (inByte == '2') {
        _Mode = 2;
        Serial.println("mode set to debug_2");    
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - -  Log Position Data  - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::logPosData()
{
    _DebugArr[_DebugArrCntr++] = (short) analogRead(_AnalogInPin);   
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - Send Position Data  - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -

void SmoothMicroServoController::sendPosData()
{
    for (int i=0; i < 3000; i++) {
        Serial.println(_DebugArr[i]);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - Update  - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::update()
{
    if (_Cntr == _SwitchToLow) {
        _ModOut = 0;
    }

    // Count up to MaxCount. Restart at MaxCount + 1.
    if (_Cntr < _MaxCount) {
        _Cntr++; 
    } else {
        _ModOut = 1;
        _Cntr = 1;
    }

    // Log Position Data for debug
    logPosData(); 

    if (_Mode == 1) {
        debug_1(); 
    }
}

