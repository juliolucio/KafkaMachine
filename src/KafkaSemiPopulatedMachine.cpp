//
//  KafkaSemiPopulatedMachine.h
//  KafkaSemiPopulatedMachine
//
//  Created by Julio Lucio on 8/August/2016.//
//
#include "KafkaSemiPopulatedMachine.h"
//--------------------------------------------------------------
KafkaSemiPopulatedMachine::KafkaSemiPopulatedMachine(){
}
//--------------------------------------------------------------
KafkaSemiPopulatedMachine::~KafkaSemiPopulatedMachine(){
    delete machine;
    delete machineController;
}
//--------------------------------------------------------------
void KafkaSemiPopulatedMachine::setup( string machineName , int theNumVideos ){
    machine  = new KafkaStatesMachine( machineName );
    machine->addStatesFormSingleFile( "scenes/AllScenes.tsv");
    machineController = new KafkaStatesMachineController( machine  , theNumVideos );
}
//--------------------------------------------------------------
void KafkaSemiPopulatedMachine::update(){
    machineController->update();
}
//--------------------------------------------------------------
void KafkaSemiPopulatedMachine::draw(){
    machineController->drawView();
}
