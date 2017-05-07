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
    _Mode = 0;
    _SkipExecCounter = 0;
    _OutVal = 0.0;
    _MinSpeed = 30;
    _MaxSpeed = 60;
    
    pinMode(_CWp, OUTPUT);
    pinMode(_CCWp, OUTPUT);

    // set up Position Control variables
    _CurrentPos = analogRead(_AnalogInPin);
    _GoalPos = 512;
    _GoalSpeed = 20;
    _OldCurrentPos = _CurrentPos; 

    _DebugArrCntr = 0;
    for (int i=0; i < 3000; i++){
        _DebugArr[i] = 0;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - Update  - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::update()
{
    if (_Mode == 1) {
        _PositionControl(); 
    } else if (_Mode == 2) {
        _PositionControl(); 
        _LogPosData(); 
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - Position Control  - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::_PositionControl()
{
    _CurrentPos = analogRead(_AnalogInPin);
    // smoothing input 
    // compute current speed
    _CurrentSpeed = _CurrentPos - _OldCurrentPos;
    if((_GoalPos - _CurrentPos) > 0){
        // we are moving in the positive direction!
        if (abs(_GoalPos - _CurrentPos) > 300) {
            // we are still more then 200 steps away from goalposition 
            _OutVal +=  (_GoalPos - _CurrentPos) / 2000.0; 
            if (_OutVal > _MaxSpeed) {
                _OutVal = _MaxSpeed;
            } else if (_OutVal < -_MaxSpeed){
                _OutVal = -_MaxSpeed;
            } 
        } else {
            // we are less than 200 steps away from goalpos    
            _OutVal -=  (_GoalPos - _CurrentPos) / 2000.0; 
            if (_OutVal > _MaxSpeed) {
                _OutVal = _MaxSpeed;
            } else if (_OutVal < _MinSpeed){
                _OutVal = _MinSpeed;
            } 
        }
    } else {
        // we are moving in the negative direction!
        if (abs(_GoalPos - _CurrentPos) > 300) {
            // we are still more then 200 steps away from goalposition 
            _OutVal +=  (_GoalPos - _CurrentPos) / 2000.0; 
            if (_OutVal > _MaxSpeed) {
                _OutVal = _MaxSpeed;
            } else if (_OutVal < -_MaxSpeed){
                _OutVal = -_MaxSpeed;
            } 
        } else {
            // we are less than 200 steps away from goalpos    
            _OutVal -=  (_GoalPos - _CurrentPos) / 2000.0; 
            if (_OutVal > -_MinSpeed) {
                _OutVal = -_MinSpeed;
            } else if (_OutVal < -_MaxSpeed){
                _OutVal = -_MaxSpeed;
            } 
        }
    }

    Serial.println(_OutVal);

    if (abs(_GoalPos - _CurrentPos) < 5) {
        analogWrite(_CCWp, 255);
        analogWrite(_CWp, 255);
    } else if (_OutVal > 0){
        analogWrite(_CCWp, 0);
        analogWrite(_CWp, abs((int)_OutVal));
    } else {
        analogWrite(_CWp, 0);
        analogWrite(_CCWp, abs((int)_OutVal));
    }    

    _OldCurrentPos = _CurrentPos; 
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - Smoothing Input - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::_SmoothingInput()
{

}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - Compute Current Speed - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::_ComputeCurrentSpeed()
{

}
    
// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - -  Set Goal Pos - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::setGoalPos(short goalPos)
{
    _GoalPos = goalPos;
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - Set Up Mode - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::setUpMode()
{
    char inByte;
    Serial.println("Choose your mode: ");   // send an initial string
    Serial.println("1: Position Control");   
    Serial.println("2: Debug");   

    while (Serial.available() <= 0) {
        delay(300);
    }

    inByte = Serial.read();
    if (inByte == '1') {
        _Mode = 1;
        Serial.println("mode set to Position Control");    
    }
    if (inByte == '2') {
        _Mode = 2;
        Serial.println("mode set to Debug");    
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - -  Log Position Data  - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -
void SmoothMicroServoController::_LogPosData()
{
    if (_DebugArrCntr < 3000){
        _DebugArr[_DebugArrCntr++] = (short) analogRead(_AnalogInPin);   
    }
}

// - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - Send Position Data  - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - -

void SmoothMicroServoController::_SendPosData()
{
    for (int i=0; i < 3000; i++) {
        Serial.println(_DebugArr[i]);
    }
}


