//
//  KafkaStatesMachineView.h
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//

#ifndef KafkaStatesMachineView_h
#define KafkaStatesMachineView_h

#include <stdio.h>
#include "ofMain.h"
typedef enum { MACHINE_VIEW_TYPE_SPHERES ,
    MACHINE_VIEW_TYPE_BOXES,
    MACHINE_VIEW_TYPE_PLANES} KafkeMachineViewType;

class KafkaStatesMachineView{
private:
    //Machine
    string name;
    KafkeMachineViewType type;
    ofBoxPrimitive videoBox;
    ofPlanePrimitive videoPlaneHorizontal;
    ofPlanePrimitive videoPlaneVertical;
    
    float machineGap;
    
    //videos
    ofVec3f machineOrigen;
    ofVec3f machineSize;
    ofVec3f machineVideoSize;
    
    vector<ofVec3f> machineVideosPositions;
    map<int,ofBoxPrimitive*> videosBoxesPrimitives;
    vector<ofColor> videosColors;
    
    //states
    map<int,of3dPrimitive*> states;
    vector<string> statesNames;
    
    vector<int> statesVideoIndexes;
    
    vector< map<string,float> > statesQuantizations;
    
    vector<float> statesPercentageStarts;
    vector<float> statesPercentageEnds;
    vector<float> statesPercentageSizes;
    
    vector<ofVec3f> statesPositionsCenters;
    vector<ofVec3f> statesPositionsInits;
    vector<ofVec3f> statesPositionsEnds;
    
    //transitions
    float deltaBezierX;
    float deltaBezierY;
    
    vector<string> transitionStateNameInitial;
    vector<string> transitionStateNameFinal;
    
    vector<float> transitionStateProbabilities;
    vector<ofColor> transitionsColors;
    vector<ofVec3f> transitionsPositionsCenters;
    vector<ofVec3f> transitionsPositionsInits;
    vector<ofVec3f> transitionsPositionsEnds;
    
    int stateCurrent;
    int statePrevious;
    int lastTransition;
    
    int numVideos;
    
    ofstream* fileOut;
    ifstream* fileIn;
    
    bool isItActive;
    void clear();
    int getStateIndex( string name );
    vector<ofMeshFace> triangles;
    
    ofTrueTypeFont font;
    
    ofVideoPlayer* currentVideo;
    int activeVideoIndex;
    
    //texturing and lighting
    ofMaterial material;
    ofTexture textureFromVideo;
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofLight pointLightTime;

    
    
public:
    KafkaStatesMachineView( string theName , int numVideos );
    ~KafkaStatesMachineView();
    bool addState( string theName , int videoIndex , float theEnergy , float thePercentageStart , float thePercentageEnd  );
    bool addTransition( string nameState01 , string nameState02 , float theProbability );
    void draw();
    bool load( string fileName );
    bool loadFromTSV( string fileName );
    void setActive( bool ifIsItActive );
    bool setCurrentState( string stateName );
    void updateViewDataVideo( int activeVideoIndex , ofVideoPlayer* currentVideo );
};

#endif 
