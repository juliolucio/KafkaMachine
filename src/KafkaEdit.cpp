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
}
//--------------------------------------------------------------
void KafkaEdit::update(){
    machineController->update();
}
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
