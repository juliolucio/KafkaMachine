#pragma once

#include "ofMain.h"
#include "ofEvents.h"


class KafkaMachineArduinoHardware{

public:

	bool setup( std::string portName );
	void update();
	void draw();
    bool isRuning();
    
    int getAppState();
    float  getBrightness();
    float getZoom();
    float getText();
    float getEnergy01();
    float getEnergy02();
    float getEnergy03();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofImage				imageKonb;
    ofImage				imageButon;
    ofImage				imageLedOn;
    ofImage				imageLedOff;
    
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	ofArduino           arduinoBoard;
    
    bool                isSetedUpArduino;
    
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
    float text;
    
    bool isRandom;
    bool isClosedMachines;
    bool isEnergys; 
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();

};

