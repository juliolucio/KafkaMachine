//
//  KafkaStatesMachineController.cpp
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//
#include "KafkaStatesMachineController.h"
#include "KafkaStatesMachine.h"
#include "KafkaStatesMachineView.h"

//-------------------------------------------------------------
KafkaStatesMachineController::KafkaStatesMachineController( KafkaStatesMachine* theMachineReference , string fileName ){
    machineReference = theMachineReference;
    machineView = new KafkaStatesMachineView( theMachineReference->getName() );
    machineView->load(fileName);
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
}
//-------------------------------------------------------------
void KafkaStatesMachineController::drawView(){
    machineView->draw();
}
//-------------------------------------------------------------
void KafkaStatesMachineController::clear(){
    delete machineView;
}