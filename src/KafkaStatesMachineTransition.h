//
//  KafkaStatesMachineTransition.h
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//
#ifndef KafkaStatesMachineTransition_h
#define KafkaStatesMachineTransition_h

#include <stdio.h>
#include "ofMain.h"
class KafkaStatesMachineState;
class KafkaStatesMachineTransition{
private:
    KafkaStatesMachineState* stateInitial;
    KafkaStatesMachineState* stateFinal;
    float probability;
public:
    KafkaStatesMachineTransition(  KafkaStatesMachineState* theStateInitial , KafkaStatesMachineState* theStateFinal , float theProbability );
    string getNameStateInitial();
    string getNameStateFinal();
    KafkaStatesMachineState* getStateFinal();
    KafkaStatesMachineState* getStateInitial();
    float getProbability();
    bool save( std::ofstream* fileOut);
};
#endif
