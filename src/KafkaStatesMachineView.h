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
#include "KafkaStatesMachine.h"
#include "KafkaStatesMachineState.h"
#include "KafkaStatesMachineTransition.h"

typedef enum {
    MACHINE_VIEW_TYPE_BOXES,
    MACHINE_VIEW_TYPE_SPHERES,
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
    ofVec3f machineCenter;
    
    vector<ofVec3f> machineVideosPositions;
    map<int,ofBoxPrimitive*> videosBoxesPrimitives;
    vector<ofColor> videosColors;
    int primitivesSpacign;
    
    //states
    map<int,of3dPrimitive*> statePrimitive01;
    map<int,of3dPrimitive*> statePrimitive02;
    map<int,vector<ofMeshFace>> statePrimitiveTriangles;
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
    vector<ofVec3f> transitionsPositionsInitsCenters01;
    vector<ofVec3f> transitionsPositionsInitsCenters02;
    vector<ofVec3f> transitionsPositionsCentersEnds01;
    vector<ofVec3f> transitionsPositionsCentersEnds02;
    
    int stateCurrent;
    int statePrevious;
    int lastTransition;
    
    int numVideos;
    
    ofstream* fileOut;
    ifstream* fileIn;
    
    bool isItActive;
    
    void clear();
    int getStateIndex( string name );
    
    ofTrueTypeFont font;
    
    ofVideoPlayer* currentVideo;
    int activeVideoIndex;
    
    ofVec3f positionOrigin;
    ofVec3f videoPlanePosition;
    ofVec3f positionPointLight01;
    ofVec3f positionPointLight02;
//    ofVec3f positionPointLight03;

    ofMaterial material;
    ofTexture textureFromVideo;
    ofLight pointLight01;
    ofLight pointLight02;
//    ofLight pointLight03;

    
public:
    KafkaStatesMachineView( string theName , int numVideos );
    ~KafkaStatesMachineView();
    bool addState( string theName , int videoIndex , float theEnergy , float thePercentageStart , float thePercentageEnd  );
    bool addTransition( string nameState01 , string nameState02 , float theProbability );
    void update();
    void draw();
    bool load( string fileName );
    bool loadFromTSV( string fileName );
    bool loadFromMachine( KafkaStatesMachine* machineRefernece );
    void setActive( bool ifIsItActive );
    bool setCurrentState( string stateName );
    void updateViewDataVideo( int activeVideoIndex , ofVideoPlayer* currentVideo );
};

#endif 
