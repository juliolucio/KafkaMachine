//
//  KafkaStatesMachineTransition.cpp
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//
#include "KafkaStatesMachineTransition.h"
#include "KafkaStatesMachineState.h"
//----------------------------------------------------------
KafkaStatesMachineTransition::KafkaStatesMachineTransition(  KafkaStatesMachineState* theStateInitial , KafkaStatesMachineState* theStateFinal , float theProbability ){
    stateInitial = theStateInitial;
    stateFinal = theStateFinal;
    probability = theProbability;
}
//----------------------------------------------------------
string KafkaStatesMachineTransition::getNameStateInitial(){
    return stateInitial->getName();
}
//----------------------------------------------------------
string KafkaStatesMachineTransition::getNameStateFinal(){
    return stateFinal->getName();
}
//----------------------------------------------------------
KafkaStatesMachineState* KafkaStatesMachineTransition::getStateFinal(){
    return stateFinal;
}
//----------------------------------------------------------
KafkaStatesMachineState* KafkaStatesMachineTransition::getStateInitial(){
    return stateInitial;
}
//----------------------------------------------------------
float KafkaStatesMachineTransition::getProbability(){
    return probability;
}
//-----------------------------------------------------------
bool KafkaStatesMachineTransition::save(std::ofstream* fileOut ){
    (*fileOut) << "stateNameInitial= ";
    (*fileOut) << stateInitial->getName();
    (*fileOut) << "\n";
    
    (*fileOut) << "stateNameFinal= ";
    (*fileOut) << stateFinal->getName();;
    (*fileOut) << "\n";
    
    (*fileOut) << "probability= ";
    (*fileOut) << probability;
    (*fileOut) << "\n";
    
    return  true;
}

