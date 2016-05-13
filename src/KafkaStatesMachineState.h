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
    float energy;
    float percentStart;
    float percentEnd;
    long frameStart;
    long frameEnd;

public:
    KafkaStatesMachineState( );
    KafkaStatesMachineState( string theName ,int theVideoIndex, vector< float > params );
    string getName();
    float getEnergy();
    int getVideoIndex();
    float getStart();
    float getEnd();
    
    
    bool load( std::ifstream* fileIn );
    bool loadFromTSV( std::ifstream* fileIn , long frames );
    bool save( std::ofstream* fileIn );
    
};
#endif
