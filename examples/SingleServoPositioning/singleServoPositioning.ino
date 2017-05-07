
#include "SmoothMicroServoController.h"

SmoothMicroServoController smsc(A0, 11, 12);

void setup() {
    Serial.begin(115200);

    while(!Serial) {
        // wait for serial port to connect. Needed for native USB port only
        ;
    }
    smsc.setUpMode();
}

void loop() {
    smsc.update();
}
