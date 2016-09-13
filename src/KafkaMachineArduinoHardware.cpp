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

    pinButtonRandom = 2;
    pinButtonClosedMachines = 3;
    pinButtonEnergys = 4;
    
    pinKnobBrightness = 3;
    pinKnobZoom = 4;
    pinKnobRotation = 5;
    
    pinKnobEnergy01 = 0;
    pinKnobEnergy02 = 1;
    pinKnobEnergy03 = 2;
    
    pinLedRandom = 11;
    pinLedClosedMachines = 12;
    pinLedEnergys = 13;
    
    deltaToChangeValue = 10;
    
    state = NO_PRESED;
    
    presedNow = false;
    
	if( !arduinoBoard.connect( portName , 57600) )
        return false;
    
	ofAddListener( arduinoBoard.EInitialized , this , &KafkaMachineArduinoHardware::setupArduino );

    return true;
}
//--------------------------------------------------------------
bool KafkaMachineArduinoHardware::isRuning(){
    return isSetedUpArduino;
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::update(){
    updateArduino();
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
float KafkaMachineArduinoHardware::getRotation(){
    return rotation;
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
int KafkaMachineArduinoHardware::hasJustPresedButton(){
    return (int)state;
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::setupArduino(const int & version) {
	ofRemoveListener(arduinoBoard.EInitialized, this, &KafkaMachineArduinoHardware::setupArduino);
    
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
    arduinoBoard.sendAnalogPinReporting( pinKnobRotation , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobEnergy01 , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobEnergy02 , ARD_ANALOG );
    arduinoBoard.sendAnalogPinReporting( pinKnobEnergy03 , ARD_ANALOG );
    
    ofAddListener( arduinoBoard.EDigitalPinChanged, this, &KafkaMachineArduinoHardware::digitalPinChanged );
    ofAddListener( arduinoBoard.EAnalogPinChanged, this, &KafkaMachineArduinoHardware::analogPinChanged );
    
    cout << "\nArduino ready !";
    isSetedUpArduino = true;
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::updateArduino(){
	arduinoBoard.update();
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::digitalPinChanged(const int & pinNum) {
    state = NO_PRESED;
    if( !arduinoBoard.getDigital( pinButtonRandom ) ){
    //if( arduinoBoard.getDigital( pinButtonRandom ) ){
        state = PRESED_RANDOM;
        presedNow = true;
        cout << "\nBotao 1 Pressed ";
    }

    
    else if( arduinoBoard.getDigital( pinButtonClosedMachines ) ){
        state = PRESED_CLOSED;
        presedNow = true;
        cout << "\nBotao 2 Pressed ";
    }

    else if( !arduinoBoard.getDigital( pinButtonEnergys ) ){
    //else if( arduinoBoard.getDigital( pinButtonEnergys ) ){
            //is negated because the buttonsbuttons are low activated
        state = PRESED_ENERGIES;
        presedNow = true;
        cout << "\nBotao 3 Pressed ";
    }

    return state;
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::analogPinChanged(const int & pinNum ) {
    int pinNumber = int(pinNum);
    int newValue = arduinoBoard.getAnalog(pinNum);
    
    if( pinNumber == pinKnobBrightness )
        if( abs( newValue - brightness ) > deltaToChangeValue  ){
            //cout << "\nbrightness = " << brightness;
            brightness = newValue;
        }
    
    if( pinNumber == pinKnobZoom )
        if( abs( newValue - zoom ) > deltaToChangeValue  ){
            //cout << "\nzoom = " << zoom;
            zoom = newValue;
        }
    
    if( pinNumber == pinKnobRotation )
        if( abs( newValue - rotation ) > deltaToChangeValue  ){
            //cout << "\nrotation = " << rotation;
            rotation = newValue;
        }
    
    if( pinNumber == pinKnobEnergy01 )
        if( abs( newValue - energy01 ) > deltaToChangeValue  ){
            //cout << "\nenergy01 = " << energy01;
            energy01 = newValue;
        }

    if( pinNumber == pinKnobEnergy02 )
        if( abs( newValue - energy02 ) > deltaToChangeValue  ){
            //cout << "\nenergy02 = " << energy02;
            energy02 = newValue;
        }

    if( pinNumber == pinKnobEnergy03 )
        if( abs( newValue - energy03 ) > deltaToChangeValue  ){
            //cout << "\nenergy03 = " << energy03;
            energy03 = newValue;
        }
}
//--------------------------------------------------------------
void KafkaMachineArduinoHardware::draw( int x , int y ){
    string debugString;
    ofFill();
    ofEnableAlphaBlending();
    ofSetColor( 20 , 200 , 0 , 100 );
    ofDrawRectangle( x - 10 , y - 20 , 200 , 230 );
    ofDisableAlphaBlending();
    ofNoFill();
    
    ofSetColor( 255,255,255  );

	if (!isSetedUpArduino){
		font.drawString("arduino not ready...\n", 515, 40);
	} else {
        string result;
        result += "Brigthness : " + ofToString( brightness );
        result += "\nZoom : " + ofToString( zoom );
        result += "\nRotation : " + ofToString( rotation );
        result += "\n\nEnergy01 : " + ofToString( energy01 );
        result += "\nEnergy02 : " + ofToString( energy02 );
        result += "\nEnergy03 : " + ofToString( energy03 );
        result += "\n\nRANDOM : " + ofToString( isRandomMachinesButton );
        result += "\nCLOSED : " + ofToString( isClosedMachinesButton );
        result += "\nENERGYS : " + ofToString( isEnergysMachineButton );
        
		smallFont.drawString( result , x , y );
	}
}