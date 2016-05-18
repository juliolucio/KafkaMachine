//
//  KafkaStatesMachine.h
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//
#ifndef KafkaStatesMachine_h
#define KafkaStatesMachine_h

#include <stdio.h>
#include "ofMain.h"
class KafkaStatesMachineState;
class KafkaStatesMachineTransition;

class KafkaStatesMachine{
private:
    vector< KafkaStatesMachineState* > states;
    vector< KafkaStatesMachineTransition* > transitions;
    KafkaStatesMachineState* currentState;
    KafkaStatesMachineState* nextState;
    KafkaStatesMachineState*prevState;
    ofstream* fileOut;
    ifstream* fileIn;
    string name;
    bool isItActive;
    bool hasJustChangedState;
    //float volume;
    void clear();
    
public:
    KafkaStatesMachine( string theName );
    ~KafkaStatesMachine();
    bool addState( string theName , int theVideoIndex , vector<float> theParameters );
    bool addTransition( string nameState01 , string nameState02 , float theProbability );

    bool load( string fileName );
    bool loadFromTSV( string fileName );
    bool addStatesFormFile( string fileName , int videoIndex );
    //void fullPopulateTransitions();
    bool save( string fileName );
    void updateStates( float theEnergy);
    bool step();
    bool stepRandom();
    bool stepEnergys();
    void draw();
    string getName();
    string getCurrentStateName();
    int getCurrentStateVideoIndex();
    float getCurrentStateStart();
    float getCurrentStateEnd();
    float getCurrentStateEnergy();
    int getNumStates();
    int getNumTransition();
    KafkaStatesMachineState* getState( int stateIndex );
    KafkaStatesMachineTransition* getTransition( int transitionIndex );
    
    void setActive( bool ifIsItActive );
    bool isAtcive();
    bool justChangedState();
    void setCurrentState( KafkaStatesMachineState* newState );
    
    

};

#endif 
