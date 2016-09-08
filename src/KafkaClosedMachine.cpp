//
//  MusicianComposition.cpp
//  KafkaMachine
//
//  Created by Julio Lucio on 10/24/15.
//
//
#include "KafkaClosedMachine.h"
//--------------------------------------------------------------
KafkaClosedMachine::KafkaClosedMachine(){
}
//--------------------------------------------------------------
KafkaClosedMachine::~KafkaClosedMachine(){
    delete machine;
    delete machineController;
}
//--------------------------------------------------------------
void KafkaClosedMachine::setup( string machineName , string machineFileName  , int theNumVideos ){
    machine  = new KafkaStatesMachine( machineName );
    machine->loadFromTSV( machineFileName );
    machineController = new KafkaStatesMachineController( machine , machineFileName  , theNumVideos );
}
//--------------------------------------------------------------
void KafkaClosedMachine::update(){
    machineController->update();
}
//--------------------------------------------------------------
void KafkaClosedMachine::draw(){
    machineController->drawView();
}
//--------------------------------------------------------------
void KafkaClosedMachine::reset(){
    machine->reset();
}
