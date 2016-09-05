#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "KafkaFullPopulatedMachine.h"
#include "KafkaSemiPopulatedMachine.h"
#include "KafkaClosedMachine.h"
#include "KafkaMachineArduinoHardware.h"


class ofApp: public ofBaseApp{
public:
    
    typedef enum {
        APP_STATE_RANDOM,
        APP_STATE_CLOSED_MACHINES,
        APP_STATE_ENERGYS
    }appStates;
    
    
    //loops
    void setup();
    void update();
    void draw();
    
    //user interacion
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
    
    //bool loadTextForEffect( string fileName );
    
    bool setupHardware();
    
    //updating modes
    bool updateHardware();
    void updateGUI();
    void setAppState( appStates theState  );
    //void updateTextEffect();
    void setCurrentVideoState( int theIndexVideoSelected , float theStatrtPositionPercent , float theCurrentPositionPercent );
    void updateAleatorio();
    void updateRandom();
    void updateEnergys();
    void updateClosedMachine();
    void jumpToOtherClosedMachine();
    
    //timing
    long getcurrentVideoDurationMilli();
    
    //drawing
    void drawDebugTimes( int x , int y );
    void drawDebugTimeline( int x , int y , int w , int h);
    void drawHardware( int x , int y );
    string getTimeStringFromMilli (long Milli);
    void drawEnergy( ofVec3f position , int height , int radius);
    void drawVolumen( ofVec3f position , int height , int radius);
    
    //camera
    void drawInteractionArea();
    
    //video
    ofVideoPlayer   bed_fight;
    ofVideoPlayer   karl_suitcase;
    ofVideoPlayer   access;
    ofVideoPlayer   stoker_cabin;
    ofVideoPlayer   stoker_lanzarote;
    ofVideoPlayer   klaus_lost_suicase;
    ofVideoPlayer   ramses;
    ofVideoPlayer   lift_boy;
    ofVideoPlayer   lisbon_protest;
    ofVideoPlayer   america;
    ofVideoPlayer   interviews_ed;
    ofVideoPlayer   interviews_all;
    ofVideoPlayer   boat_entrance;
    ofVideoPlayer   boat;
    
    
    ofVideoPlayer*   currentVideo;
    ofVideoPlayer*   previousVideo;
    
    ofTexture textureVideo;
    ofPixels pixelsVideo;
    std::vector< ofVideoPlayer> videos;
    int             videoSelected;
    
    //control flags
    bool            hasFinishedPlaying;
    bool            isFirstTime;
    bool            isFirstTimeUpdateAleatorio;
    bool            isArduinoHardwarePresent;
    
    //timing
    long            cutLenghtMilli;
    
    long            cutStartPositionMilli;
    long            cutEndPositionMilli;
    
    long            currentVideoDurationMilli;
    long            currentVideoPositionMilli;
    
    float           currentVideoPositionNormalized;
    float           cutStartPositionPercent;
    
    float           currentVideoSpeed;
    
    float           delataEnd;
    
    ofTrueTypeFont  font;
    ofTrueTypeFont  fontNameClosedMachine;
    
//    //text effect
//    ofTrueTypeFont  fontTextEffect;
//    ifstream* fileIn;
//    string textForEffect;
//    string textDrawingEffect;
//    long textEfectUpdateRefresh;
//    long textEfectUpdateRefreshMin;
//    long lasttextEfectUpdateRefresh;
//    int numLetersInTextEffect;
//    int numLetersInTextEffectMin;
//    int firstEffectCharacter;
//    int textEffectDirection;
//    int textEffectSize;
//    ofVec3f textEffectPosition;
//    ofVec3f textEffectRotation;
    
    //camera
    ofEasyCam* camera;
    
    //machines for modes
    vector<KafkaClosedMachine*> machinesClosed;
    KafkaClosedMachine* currentClosedMacine;
    int currentClosedMacineIndex;
    long closedMachinesUpdateRefresh;
    long closedMachinesUpdateRefreshMin;
    long closedMachinesUpdateRefreshMax;
    long lastClosedMachinesUpdateTime;
    //KafkaFullPopulatedMachine* machineRandom;
    KafkaFullPopulatedMachine* machineEnergys;
    KafkaSemiPopulatedMachine* machineRandomSemipoulated;
    
    //energuy state
    vector<float> currentEnergys;
    
    //app state
    appStates appState;
    
    //arduino hardware
    KafkaMachineArduinoHardware hardware;
    long harwareUpdateRefresh;
    long lastHardwareUpdateRefresh;
    
    //lights
    ofVec3f positionPointLight01;
    ofVec3f positionPointLight02;
    ofVec3f positionPointLight03;
    
    ofLight pointLight01;
    ofLight pointLight02;
    ofLight pointLight03;
    
    //parameters
    float           currentVideoBrightness;
    float           currentVideoZoom;
    //float           currentVideoText;
    float           currentMachineRotation;
    float           currentMachineTranslation;
    float           currentEnergy01;
    float           currentEnergy02;
    float           currentEnergy03;
    
    float           currentVideoBrightnessMax;
    float           currentVideoBrightnessMin;
    float           currentVideoZoomMax;
    float           currentVideoZoomMin;
    //float           currentVideoTextMax;
    //float           currentVideoTextMin;
    float           currentVideoMachineRotationMax;
    float           currentVideoMachineRotationMin;
    float           currentMachineTranslationMax;
    float           currentMachineTranslationMin;
    
    float           currentEnergy01Max;
    float           currentEnergy01Min;
    float           currentEnergy02Max;
    float           currentEnergy02Min;
    float           currentEnergy03Max;
    float           currentEnergy03Min;
    
    //GUI  //-----------------------------------------------
    void setupGUI();
    void drawGUI();
    
    //gui Global
    ofxPanel guiGlobal;
    
    ofxFloatSlider sliderBrightness;
    ofxFloatSlider sliderZoom;
    ofxFloatSlider sliderText;
    
    void sliderBrightnessChanged(float &sliderBright );
    void sliderZoomChanged(float &sliderZoom );
    void sliderTextChanged(float &slidetText );
    
    //GUI Energys
    ofxPanel guiEnergys;
    
    ofxFloatSlider sliderEnergy01;
    ofxFloatSlider sliderEnergy02;
    ofxFloatSlider sliderEnergy03;
    
    void sliderEnergy01Changed(float &sliderEne01 );
    void sliderEnergy02Changed(float &sliderEne02 );
    void sliderEnergy03Changed(float &sliderEne03 );
};

