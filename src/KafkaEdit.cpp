//
//  MusicianComposition.cpp
//  KafkaMachine
//
//  Created by Julio Lucio on 10/24/15.
//
//
#include "KafkaEdit.h"
//--------------------------------------------------------------
KafkaEdit::KafkaEdit(){
}
//--------------------------------------------------------------
KafkaEdit::~KafkaEdit(){
    delete machine;
    delete machineController;
}
//--------------------------------------------------------------
void KafkaEdit::setup( string machineName , string machineFileName  , int theNumVideos ){
    machine  = new KafkaStatesMachine( machineName );
    machine->loadFromTSV( machineFileName );
    machineController = new KafkaStatesMachineController( machine , machineFileName  , theNumVideos );
}
//--------------------------------------------------------------
void KafkaEdit::update(){
    machineController->update();
}
//--------------------------------------------------------------
void KafkaEdit::draw(){
    machineController->drawView();
}

