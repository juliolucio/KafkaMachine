#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "KafkaEdit.h"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
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
    
    //updating modes
    void updateALeatorio01();
    long getcurrentVideoDurationMilli();
    
    //drawing
    void drawDebugTimes( int x , int y );
    void drawDebugTimeline( int x , int y , int w , int h);
    string getTimeStringFromMilli (long Milli);
    void drawEnergy( ofVec3f position , int height , int radius);
    void drawVolumen( ofVec3f position , int height , int radius);
    
    //camera
    void drawInteractionArea();
    
    //gui
    void setupGUI();
    void sliderCutMinimumLenghtMilliChanged(int &sliderCutMinimumLenghtMilli);
    void sliderCutMaximumLenghtMillihanged(int &sliderCutMaximumLenghtMilli );

    
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
    ofVideoPlayer   boat;
    
    ofVideoPlayer   currentVideo;
    
    
    std::vector< ofVideoPlayer> videos;
    
    bool            hasFinishedPlaying;
    bool            isFirstTime;
    bool            isFirstTimeUpdateAleatorio;
    
    int             videoRandomChoiceIndex;
    
    long            cutLenghtMilli;
    
    float           cutStartPositionPercent;
    long            cutStartPositionMilli;
    
    long            cutEndPositionMilli;
    
    long            cutMinimumLenghtMilli;
    long            cutMaximusLenghtMilli;
    
    long            currentVideoDurationMilli;
    
    float           currentVideoPositionNormalized;
    long            currentVideoPositionMilli;

    float           currentVideoSpeed;
    
    float           delataEnd;
    
    ofTrueTypeFont  font;
    
    
    
    //camera
    ofEasyCam* camera;
    
    //texturing and lighting
    //ofImage texture;
    ofMaterial material;
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofLight pointLightTime;

    //composition
    KafkaEdit edit;
    
    //gui
    ofxPanel gui;
    ofxIntSlider sliderCutMinimumLenghtMilli;
    ofxIntSlider sliderCutMaximusLenghtMilli;
    
    


};

