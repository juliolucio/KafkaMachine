//
//  KafkaStatesMachineState.h
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//

#ifndef KafkaStatesMachineState_h
#define KafkaStatesMachineState_h

#include <stdio.h>
class ofSoundPlayer;

class KafkaStatesMachineState{
private:
    string name;
    int  videoIndex;
    float energy01;
    float energy02;
    float energy03;
    float percentStart;
    float percentEnd;
    long frameStart;
    long frameEnd;
    
public:
    KafkaStatesMachineState( );
    KafkaStatesMachineState( string theName ,int theVideoIndex, vector< float > params );
    KafkaStatesMachineState( string theName ,int theVideoIndex , int theNumFrames, int  theFrameInit , int theFrameEnd );

    string getName();
    float getEnergy01();
    float getEnergy02();
    float getEnergy03();
    int getVideoIndex();
    float getStart();
    float getEnd();
    float getFrameStart();
    float getFrameEnd();
    bool load( std::ifstream* fileIn );
    bool loadFromTSV( std::ifstream* fileIn );
    bool save( std::ofstream* fileIn );
    
};
#endif
