#pragma once

#include "ofMain.h"
#include "ofEvents.h"


class KafkaMachineArduinoHardware{

public:

	bool setup( std::string portName );
	void update();
	void draw( int x , int y );
    bool isRuning();
    
    int getAppState();
    float getBrightness();
    float getZoom();
    float getRotation();
    float getEnergy01();
    float getEnergy02();
    float getEnergy03();
    
    bool justPresedButton();

	ofImage				imageKonb;
    ofImage				imageButon;
    ofImage				imageLedOn;
    ofImage				imageLedOff;
    
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	ofArduino           arduinoBoard;
    
    bool isSetedUpArduino;
    bool presedNow;
    
    int pinButtonRandom;
    int pinButtonClosedMachines;
    int pinButtonEnergys;
    
    int pinKnobBrightness;
    int pinKnobZoom;
    int pinKnobText;
    
    int pinKnobEnergy01;
    int pinKnobEnergy02;
    int pinKnobEnergy03;
    
    int pinLedRandom;
    int pinLedClosedMachines;
    int pinLedEnergys;
    
    float energy01;
    float energy02;
    float energy03;
    
    float brightness;
    float zoom;
    float rotation;
    
    bool isRandom;
    bool isClosedMachines;
    bool isEnergys;
    
    int deltaToCjangeValue;
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();

};

