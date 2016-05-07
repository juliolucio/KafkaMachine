//
//  KafkaStatesMachineState.h
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
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

public:
    KafkaStatesMachineState( );
    KafkaStatesMachineState( string theName ,int theVideoIndex, vector< float > params );
    string getName();
    float getEnergy();
    int getVideoIndex();
    float getStart();
    float getEnd();
    
    
    bool load( std::ifstream* fileIn );
    bool save( std::ofstream* fileIn );
    
};
#endif
