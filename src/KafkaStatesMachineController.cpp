//
//  KafkaStatesMachineController.cpp
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//
#include "KafkaStatesMachineController.h"
#include "KafkaStatesMachine.h"
#include "KafkaStatesMachineView.h"

//-------------------------------------------------------------
KafkaStatesMachineController::KafkaStatesMachineController( KafkaStatesMachine* theMachineReference , string fileName , int theNumVideos ){
    machineReference = theMachineReference;
    machineView = new KafkaStatesMachineView( theMachineReference->getName() , theNumVideos );
    machineView->loadFromTSV(fileName);
}
//-------------------------------------------------------------
KafkaStatesMachineController::~KafkaStatesMachineController(){
    clear();
}
//-------------------------------------------------------------
void KafkaStatesMachineController::update(){
    machineView->setActive(machineReference->isAtcive());
    if( machineReference->justChangedState())
        machineView->setCurrentState(machineReference->getCurrentStateName());
    machineView->update();
}
//-------------------------------------------------------------
void KafkaStatesMachineController::drawView(){
    machineView->draw();
}
//-------------------------------------------------------------
void KafkaStatesMachineController::clear(){
    delete machineView;
}
//-------------------------------------------------------------
void KafkaStatesMachineController::updateViewDataVideo( int activeVideoIndex , ofVideoPlayer* currentVideo ){
    machineView->updateViewDataVideo( activeVideoIndex , currentVideo );
}
