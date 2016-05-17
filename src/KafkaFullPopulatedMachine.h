//
//  KafkaFullPopulatedMachine.h
//  KafkaFullPopulatedMachine
//
//  Created by Julio Lucio on 10/24/15.
//
//
#pragma once

#include "ofMain.h"
#include "KafkaStatesMachine.h"
#include "KafkaStatesMachineController.h"

class KafkaFullPopulatedMachine{
    
public:
    KafkaFullPopulatedMachine();
    ~KafkaFullPopulatedMachine();
    
    void setup( string machineName , int theNumVideos );
    void update( );
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    bool isMachineActive();
    
    //machines
    KafkaStatesMachine* machine;
    KafkaStatesMachineController* machineController;
    float energy;
    
    void createMachines();
    void createMachinesTest();
};

