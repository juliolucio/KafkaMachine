//
//  KafkaStatesMachineState.cpp
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//
#include "ofMain.h"
#include "KafkaStatesMachineState.h"
//----------------------------------------------------------------------------------
KafkaStatesMachineState::KafkaStatesMachineState(){
}
//----------------------------------------------------------------------------------
KafkaStatesMachineState::KafkaStatesMachineState(  string theName ,int theVideoIndex , vector< float > params ){
    name = theName;
    videoIndex = theVideoIndex;
    if( params.size() == 3 ){
        energy = params[0];
        percentStart = params[1];
        percentEnd = params[2];
    }
}
//----------------------------------------------------------------------------------
string KafkaStatesMachineState::getName(){
    return name;
}
//----------------------------------------------------------------------------------
float KafkaStatesMachineState::getEnergy(){
    return energy;
}
//----------------------------------------------------------------------------------
int KafkaStatesMachineState::getVideoIndex(){
    return videoIndex;
}
//----------------------------------------------------------------------------------
float KafkaStatesMachineState::getStart(){
    return percentStart;
}
//----------------------------------------------------------------------------------
float KafkaStatesMachineState::getEnd(){
    return percentEnd;
}
//----------------------------------------------------------------------------------
bool KafkaStatesMachineState::load(std::ifstream* fileIn ){
    std::string junk;
    
    (*fileIn) >> junk;
    if( junk != "name=" ){
        cout << "* KafkaStatesMachine  State load: Bad tag name\n";
        return false;
    }
    (*fileIn) >> name;
    
    (*fileIn) >> junk;
    if( junk != "videoIndex=" ){
        cout << "* KafkaStatesMachine  State load: Bad tag videoIndex\n";
        return false;
    }
    (*fileIn) >> videoIndex;
    
    (*fileIn) >> junk;
    if( junk != "energy=" ){
        cout << "* KafkaStatesMachine  State load: Bad tag energy\n";
        return false;
    }
    (*fileIn) >> energy;
    
    (*fileIn) >> junk;
    if( junk != "percentStart=" ){
        cout << "* KafkaStatesMachine  State load: Bad tag percentStart\n";
        return false;
    }
    (*fileIn) >> percentStart;

    
    (*fileIn) >> junk;
    if( junk != "percentEnd=" ){
        cout << "* KafkaStatesMachine  State load: Bad tag percentEnd\n";
        return false;
    }
    (*fileIn) >> percentEnd;
    

    return true;
}
//----------------------------------------------------------------------------------
bool KafkaStatesMachineState::loadFromTSV(std::ifstream* fileIn , long frames ){
    (*fileIn) >> name;
    (*fileIn) >> videoIndex;
    (*fileIn) >> frameStart;
    (*fileIn) >> frameEnd;
    
    percentStart = float(frameStart + 1) / float(frames);
    percentEnd = float(frameEnd) / float(frames);
    
    (*fileIn) >> energy;
    return true;
}
//-----------------------------------------------------------
bool KafkaStatesMachineState::save(std::ofstream* fileOut ){
    (*fileOut) << "name= ";
    (*fileOut) << name;
    (*fileOut) << "\n";
    
    (*fileOut) << "videoIndex= ";
    (*fileOut) << name;
    (*fileOut) << "\n";
    
    (*fileOut) << "energy= ";
    (*fileOut) << energy;
    (*fileOut) << "\n";
    
    (*fileOut) << "percentStart= ";
    (*fileOut) << percentStart;
    (*fileOut) << "\n";
    
    (*fileOut) << "percentEnd= ";
    (*fileOut) << percentEnd;
    (*fileOut) << "\n";
    
    return  true;
}
