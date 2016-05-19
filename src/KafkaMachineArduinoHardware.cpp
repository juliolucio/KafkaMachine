/*
 * This is a simple example use of ofArduino
 *
 * ofArduino currently only supports the standard Arduino boards
 * (UNO, Duemilanove, Diecimila, NG, and other boards based on the
 * ATMega168 or ATMega328 microcontrollers
 * The Arduio FIO and Arduino Mini should also work.
 * The Arduino MEGA and other variants based on microcontrollers
 * other than the ATMega168 and ATMega328 are not currently supported.
 * 
 * To use this example, open Arduino (preferably Arduino 1.0) and 
 * navigate to File -> Examples -> Firmata and open StandardFirmata.
 * Compile and upload StandardFirmata for your board, then close
 * the Arduino application and run this application.
 *
 * If you have a servo attached, press the left arrow key to rotate
 * the servo head counterclockwise and press the right arrow key to
 * rotate the servo head clockwise.
 *
 * Clicking the mouse over any part of the application will turn the
 * on-board LED on and off.
 *
 */

#include "KafkaMachineArduinoHardware.h"


//--------------------------------------------------------------
bool KafkaMachineArduinoHardware::setup( std::string portName ){
    isSetedUpArduino= false;

	imageKonb.load("knob.png");
    
	font.load("Contl___.ttf" , 20);
    smallFont.load("Contl___.ttf" , 14);

    pinButtonRandom = 3;
    pinButtonClosedMachines = 4;
    pinButtonEnergys = 5;
    
    pinKnobBrightness = 0;
    pinKnobZoom = 1;
    pinKnobText = 2;
    
    pinKnobEnergy01 = 3;
    pinKnobEnergy02 = 4;
    pinKnobEnergy03 = 5;
    
    pinLedRandom = 11;
    pinLedClosedMachines = 12;
    pinLedEnergys = 13;
    
	if( !arduinoBoard.connect( portName , 57600) )
        return false;
    
	ofAddListener(arduinoBoard.EInitialized, this, &KafkaMachineArduinoHardware::setupArduino);

    return true;
}

//--------------------------------------------------------------
void KafkaMachineArduinoHardware::update(){
    updateArduino();
}
//--------------------------------------------------------------
int KafkaMachineArduinoHardware::getAppState(){
    if( isRandom )
        return 0;
    else if( isClosedMachines )
        return 1;
    else if( isEnergys )
        return 2;
    else return -999999;//hack no!!!!
}
//--------------------------------------------------------------
float KafkaMachineArduinoHardware::getBrightness(){
    return brightness;
}
//--------------------------------------------------------------
float KafkaMachineArduinoHardware::getZoom(){
    return zoom;
}
//--------------------------------------------------------------
float KafkaMachineArduinoHardware::getText(){
    return text;
}
//--------------------------------------------------------------
float KafkaMachineArduinoHardware::getEnergy01(){
    return energy01;
}
//--------------------------------------------------------------
float KafkaMachineArduinoHardware::getEnergy02(){
    return energy02;
}
//--------------------------------------------------------------
float KafkaMachineArduinoHardware::getEnergy03(){
    return energy03;
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::setupArduino(const int & version) {
	ofRemoveListener(arduinoBoard.EInitialized, this, &KafkaMachineArduinoHardware::setupArduino);
    
    isSetedUpArduino = true;
    
    ofLogNotice() << arduinoBoard.getFirmwareName(); 
    ofLogNotice() << "firmata v" << arduinoBoard.getMajorFirmwareVersion() << "." << arduinoBoard.getMinorFirmwareVersion();
    
    arduinoBoard.sendDigitalPinMode( pinButtonRandom , ARD_INPUT );
    arduinoBoard.sendDigitalPinMode( pinButtonClosedMachines , ARD_INPUT );
    arduinoBoard.sendDigitalPinMode( pinButtonEnergys , ARD_INPUT );
    
    arduinoBoard.sendDigitalPinMode( pinLedRandom , ARD_OUTPUT );
    arduinoBoard.sendDigitalPinMode( pinLedClosedMachines , ARD_OUTPUT );
    arduinoBoard.sendDigitalPinMode( pinLedEnergys , ARD_OUTPUT );
 
    arduinoBoard.sendAnalogPinReporting( pinKnobBrightness , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobZoom , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobText , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobEnergy01 , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobEnergy02 , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobEnergy03 , ARD_ANALOG );
    
    ofAddListener( arduinoBoard.EDigitalPinChanged, this, &KafkaMachineArduinoHardware::digitalPinChanged );
    ofAddListener( arduinoBoard.EAnalogPinChanged, this, &KafkaMachineArduinoHardware::analogPinChanged );
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::updateArduino(){
	arduinoBoard.update();
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::digitalPinChanged(const int & pinNum) {
    if( arduinoBoard.getDigital( pinButtonRandom ) ){
        isRandom = true;
        isClosedMachines = false;
        isEnergys = false;
    }
    
    else if( arduinoBoard.getDigital( pinButtonClosedMachines ) ){
        isRandom = false;
        isClosedMachines = true;
        isEnergys = false;
    }
    
    else if( arduinoBoard.getDigital( pinButtonEnergys ) ){
        isRandom = false;
        isClosedMachines = false;
        isEnergys = true;
    }
    arduinoBoard.sendDigital( pinLedRandom , isRandom );
    arduinoBoard.sendDigital( pinLedClosedMachines , isClosedMachines );
    arduinoBoard.sendDigital( pinLedEnergys , isEnergys );
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::analogPinChanged(const int & pinNum ) {
    int pinNumber = int(pinNum);
    if( pinNumber == pinKnobBrightness )
        brightness = arduinoBoard.getAnalog(pinKnobBrightness);
    
    if( pinNumber == pinKnobZoom )
        zoom = arduinoBoard.getAnalog(pinKnobZoom);
    
    if( pinNumber == pinButtonEnergys )
        text = arduinoBoard.getAnalog(pinButtonEnergys);
    
    if( pinNumber == pinKnobEnergy01 )
        energy01 = arduinoBoard.getAnalog(pinKnobEnergy01);
    
    if( pinNumber == pinKnobEnergy02 )
        energy02 = arduinoBoard.getAnalog(pinKnobEnergy02);
    
    if( pinNumber == pinKnobEnergy03 )
        energy03 = arduinoBoard.getAnalog(pinKnobEnergy03);
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::draw(){
    ofSetColor(255, 255, 255);
	if (!isSetedUpArduino){
		font.drawString("arduino not ready...\n", 515, 40);
	} else {
        string result;
        result += "Brigthness : " + ofToString( brightness );
        result += "\nZoom : " + ofToString( zoom );
        result += "\nText : " + ofToString( text );
        result += "\n\nEnergy01 : " + ofToString( energy01 );
        result += "\nEnergy02 : " + ofToString( energy02 );
        result += "\nEnergy03 : " + ofToString( energy03 );
        result += "\n\nRANDOM : " + ofToString( isRandom );
        result += "\nCLOSED : " + ofToString( isClosedMachines );
        result += "\nENERGYS : " + ofToString( isEnergys );
        
        ofSetColor(0, 255, 0);
		smallFont.drawString( result , 500 , 200 );

        ofSetColor(255, 255, 255);
	}
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::keyPressed  (int key){
    switch (key) {
        case OF_KEY_RIGHT:
            // rotate servo head to 180 degrees
            arduinoBoard.sendServo(9, 180, false);
            arduinoBoard.sendDigital(18, ARD_HIGH);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        case OF_KEY_LEFT:
            // rotate servo head to 0 degrees
            arduinoBoard.sendServo(9, 0, false);
            arduinoBoard.sendDigital(18, ARD_LOW);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        default:
            break;
    }
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::keyReleased(int key){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::mouseMoved(int x, int y ){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::mouseDragged(int x, int y, int button){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::mousePressed(int x, int y, int button){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::mouseReleased(int x, int y, int button){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::mouseEntered(int x, int y){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::mouseExited(int x, int y){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::windowResized(int w, int h){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::gotMessage(ofMessage msg){

}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::dragEvent(ofDragInfo dragInfo){ 

}