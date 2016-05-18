//
//  KafkaFullPopulatedMachine.h
//  KafkaFullPopulatedMachine
//
//  Created by Julio Lucio on 10/April/2016.//
//
#include "KafkaFullPopulatedMachine.h"
//--------------------------------------------------------------
KafkaFullPopulatedMachine::KafkaFullPopulatedMachine(){
}
//--------------------------------------------------------------
KafkaFullPopulatedMachine::~KafkaFullPopulatedMachine(){
    delete machine;
    delete machineController;
}
//--------------------------------------------------------------
void KafkaFullPopulatedMachine::setup( string machineName , int theNumVideos ){
    machine  = new KafkaStatesMachine( machineName );
    machine->addStatesFormFile( "scenes/Scene1_Bedfight_Edit-1.tsv" , 0);
    machine->addStatesFormFile( "scenes/Scene2_Karl_Suitcase.tsv" , 1);
    machine->addStatesFormFile( "scenes/Scene3_All_Access.tsv" , 2);
    machine->addStatesFormFile( "scenes/Scene4_Stoker_Cabin_Edit-3.tsv" , 3);
    machine->addStatesFormFile( "scenes/Scene5_Stoker_Lanzarote.tsv" , 4);
    machine->addStatesFormFile( "scenes/Scene6_Klaus_Lost_Suitcase.tsv" , 5);
    machine->addStatesFormFile( "scenes/Scene7_Ramses_Edit-5.tsv" , 6);
    machine->addStatesFormFile( "scenes/Scene8_Liftboy_Edit-1.tsv" , 7);
    machine->addStatesFormFile( "scenes/Scene9 Lisbon_Protest.tsv" , 8);
    machine->addStatesFormFile( "scenes/Scene10_America_Edit-4.tsv" , 9);
    machine->addStatesFormFile( "scenes/Scene14_Boat_Edit-2.tsv" , 10);
    //machine->fullPopulateTransitions();
    
    machineController = new KafkaStatesMachineController( machine  , theNumVideos );
}
//--------------------------------------------------------------
void KafkaFullPopulatedMachine::update(){
    machineController->update();
}
//--------------------------------------------------------------
void KafkaFullPopulatedMachine::draw(){
    machineController->drawView();
}
