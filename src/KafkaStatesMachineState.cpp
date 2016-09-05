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
    if( params.size() == 6 ){
        float frames = params[0];;
        frameStart = params[1];;
        frameEnd = params[2];;
        energy01 = params[3];;
        energy03 = params[4];;
        energy02 = params[5];;
        percentStart = float(frameStart + 1) / float(frames);
        percentEnd = float(frameEnd) / float(frames);
    }
}
//----------------------------------------------------------------------------------
/*KafkaStatesMachineState::KafkaStatesMachineState(  string theName , int theVideoIndex , int theNumFrames, int  theFrameInit , int theFrameEnd ){
    name = theName;
    videoIndex = theVideoIndex;
    float frames = theNumFrames;
    frameStart = theFrameInit;
    frameEnd = theFrameInit;
    energy01 = 0;
    energy02 = 0;
    energy03 = 0;
    percentStart = float(frameStart + 1) / float(frames);
    percentEnd = float(frameEnd) / float(frames);
}*/
//----------------------------------------------------------------------------------
string KafkaStatesMachineState::getName(){
    return name;
}
//----------------------------------------------------------------------------------
float KafkaStatesMachineState::getEnergy01(){
    return energy01;
}
//----------------------------------------------------------------------------------
float KafkaStatesMachineState::getEnergy02(){
    return energy02;
}
//----------------------------------------------------------------------------------
float KafkaStatesMachineState::getEnergy03(){
    return energy03;
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
float KafkaStatesMachineState::getFrameStart(){
    return frameStart;
}
//----------------------------------------------------------------------------------
float KafkaStatesMachineState::getFrameEnd(){
    return frameEnd;
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
    (*fileIn) >> energy01;
    
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
bool KafkaStatesMachineState::loadFromTSV(std::ifstream* fileIn  ){
    long frames;
    (*fileIn) >> name;
    (*fileIn) >> videoIndex;
    (*fileIn) >> frameStart;
    (*fileIn) >> frameEnd;
    (*fileIn) >> frames;
    
    percentStart = float(frameStart + 1) / float(frames);
    percentEnd = float(frameEnd) / float(frames);
    
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
    (*fileOut) << energy01;
    (*fileOut) << "\n";
    
    (*fileOut) << "percentStart= ";
    (*fileOut) << percentStart;
    (*fileOut) << "\n";
    
    (*fileOut) << "percentEnd= ";
    (*fileOut) << percentEnd;
    (*fileOut) << "\n";
    
    return  true;
}
