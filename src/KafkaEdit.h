//
//  KafkaEdit.h
//  KafkaEdit
//
//  Created by Julio Lucio on 10/April/2016.
//
//
#pragma once

#include "ofMain.h"
#include "KafkaStatesMachine.h"
#include "KafkaStatesMachineController.h"

class KafkaEdit{
    
public:
    KafkaEdit();
    ~KafkaEdit();
    
    void setup( string machineName , string machineFileName , int theNumVideos );
    void update( );
    void draw();
    
    //machines
    KafkaStatesMachine* machine;
    KafkaStatesMachineController* machineController;
};

