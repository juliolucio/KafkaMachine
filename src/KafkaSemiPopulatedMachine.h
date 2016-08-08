//
//  KafkaSemiPopulatedMachine.h
//  KafkaSemiPopulatedMachine
//
///  Created by Julio Lucio on 10/April/2016.
//
//
#pragma once

#include "ofMain.h"
#include "KafkaStatesMachine.h"
#include "KafkaStatesMachineController.h"

class KafkaSemiPopulatedMachine{
    
public:
    KafkaSemiPopulatedMachine();
    ~KafkaSemiPopulatedMachine();
    
    void setup( string machineName , int theNumVideos );
    void update( );
    void draw();

    //machines
    KafkaStatesMachine* machine;
    KafkaStatesMachineController* machineController;
};

