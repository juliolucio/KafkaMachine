//
//  KafkaStatesMachineView.h
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//

#ifndef KafkaStatesMachineView_h
#define KafkaStatesMachineView_h

#include <stdio.h>
#include "ofMain.h"

class KafkaStatesMachineView{
private:
    map<int,ofIcoSpherePrimitive*> states;
    map<int,ofCylinderPrimitive*> transitions;
    
    vector<string> statesNames;
    vector<float> statesEnergy;
    vector<string> transitionStateNameInitial;
    vector<string> transitionStateNameFinal;
    vector<float> transitionStateProbabilities;

    int stateCurrent;
    int statePrevious;
    int lastTransition;
    
    int radiusSphere;
    int radiusCylinder;
    
    ofstream* fileOut;
    ifstream* fileIn;
    string name;
    bool isItActive;
    void clear();
    int getStateIndex( string name );
    vector<ofMeshFace> triangles;
    
    //ofImage texture;
    ofMaterial material;

    
public:
    KafkaStatesMachineView( string theName );
    ~KafkaStatesMachineView();
    bool addState( string theName , int videoIndex , float theEnergy , float thePercentageStart , float thePercentageEnd  );
    bool addTransition( string nameState01 , string nameState02 , float theProbability );
    void draw();
    bool load( string fileName );
    void setActive( bool ifIsItActive );
    bool setCurrentState( string stateName );
};

#endif 
