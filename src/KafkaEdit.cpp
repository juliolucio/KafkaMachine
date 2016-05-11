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
}
//--------------------------------------------------------------
void KafkaEdit::setup( string machineName , string machineFileName  , int theNumVideos ){
    machine  = new KafkaStatesMachine( machineName );
    machine->loadFromTSV( machineFileName );
    machineController = new KafkaStatesMachineController( machine , machineFileName  , theNumVideos );
    machine->start();
}
//--------------------------------------------------------------
//void KafkaEdit::update( float theEnergy , float theVolume ){
//    energy = theEnergy;
//    if( machines[0]->justFinishidState() )
//        for( int m = 0 ; m < machines.size() ; m++)
//            if(machines[m] && machines[m]->isAtcive() )
//                machines[m]->updateStates(theEnergy);
//    
//    for( int mc = 0 ; mc < machinesControllers.size() ; mc++)
//        if(machinesControllers[mc])
//            machinesControllers[mc]->update();
//}
//--------------------------------------------------------------
void KafkaEdit::draw(){
    machineController->drawView();
}
//--------------------------------------------------------------
void KafkaEdit::keyPressed  (int key){
}
//--------------------------------------------------------------
void KafkaEdit::keyReleased(int key){
}
//--------------------------------------------------------------
void KafkaEdit::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void KafkaEdit::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void KafkaEdit::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void KafkaEdit::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void KafkaEdit::windowResized(int w, int h){
}
//--------------------------------------------------------------
void KafkaEdit::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void KafkaEdit::dragEvent(ofDragInfo dragInfo){
}
//--------------------------------------------------------------
float KafkaEdit::getPosition(){
    if( machine->isAtcive() )
        return machine->getCurrentStatePercentaje();
    return 0;
}
//--------------------------------------------------------------
bool KafkaEdit::isMachineActive( ){
    machine->isAtcive();
}
