//
//  KafkaStatesMachineController.h
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//

#ifndef KafkaStatesMachineController_h
#define KafkaStatesMachineController_h

#include <stdio.h>
#include "ofMain.h"

class KafkaStatesMachine;
class KafkaStatesMachineView;

class KafkaStatesMachineController{
private:
    KafkaStatesMachine* machineReference;
    KafkaStatesMachineView* machineView;
    ifstream* fileIn;
    void clear();
    
public:
    KafkaStatesMachineController( KafkaStatesMachine* theMachineReference , string fileName , int numVideo );
    ~KafkaStatesMachineController();
    void update();
    void drawView();
    void updateViewDataVideo( int activeVideoIndex , ofVideoPlayer* currentVideo );
};

#endif 
