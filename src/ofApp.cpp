#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    ofBackground(125,125,125);
    ofSetVerticalSync(true);
    
     
    font.load( "Contl___.ttf" , 12 );
    
    hasFinishedPlaying = true;
    videoSelected;
    
    cutLenghtMilli = 0;
    
    cutStartPositionPercent = 0;
    cutStartPositionMilli = 0;
    
    cutEndPositionMilli = 0;
    
    cutMinimumLenghtMilli = 0;
    cutMaximusLenghtMilli = 0;
    
    currentVideoDurationMilli = 0;
    
    currentVideoPositionNormalized = 0;
    currentVideoPositionMilli = 0;
    
    cutMinimumLenghtMilli = sliderCutMinimumLenghtMilli;
    cutMaximusLenghtMilli = sliderCutMaximusLenghtMilli;
    
    currentVideoSpeed = 1;
    currentVideo.setSpeed( currentVideoSpeed );
    
    delataEnd = 0.9;
    
    ofSeedRandom();
    
    if( bed_fight.load("movies/Scene1_Bedfight_Edit-1.mov") )
        videos.push_back( bed_fight );
    
    if( karl_suitcase.load("movies/Scene2_Karl_Suitcase.mov" ) )
        videos.push_back( karl_suitcase );
    
    if( access.load("movies/Scene3_All_Access.mov") )
        videos.push_back( access );
    
    if(  stoker_cabin.load("movies/Scene4_Stoker_Cabin_Edit-3.mov") )
        videos.push_back( stoker_cabin );
    
    if( stoker_lanzarote.load("movies/Scene5_Stoker_Lanzarote.mov") )
        videos.push_back( stoker_lanzarote );
    
    if( klaus_lost_suicase.load("movies/Scene6_Klaus_Lost_Suitcase.mov") )
        videos.push_back( klaus_lost_suicase );
    
    if( ramses.load("movies/Scene7_Ramses_Edit-5.mov") )
        videos.push_back( ramses );
    
    if( lift_boy.load("movies/Scene8_Liftboy_Edit-1.mov") )
        videos.push_back( lift_boy );
    
    if( lisbon_protest.load("movies/Scene9 Lisbon_Protest.mov") )
        videos.push_back( lisbon_protest );
    
    if( america.load("movies/Scene10_America_Edit-4.mov") )
        videos.push_back( america );
    
    if( boat.load("movies/Scene14_Boat_Edit-2.mov") )
        videos.push_back( boat );
    
    for( int v = 0 ; v < videos.size() ; v ++ ){
        videos[ v ].stop();
        //videos[ v] .setVolume(0);
    }
    isFirstTime = true;

    edit.setup( "MACHINE_B" , "machines/machinesTest/MachineB.tsv" , videos.size() );
    
    videoSelected = edit.machine->getCurrentStateVideoIndex();
    currentVideo = videos[ videoSelected ];
    currentVideo.setSpeed( currentVideoSpeed );
    currentVideoDurationMilli = currentVideo.getDuration() * 1000;
    
    cutStartPositionPercent = edit.machine->getCurrentStateStart();
    cutStartPositionMilli = cutStartPositionPercent * currentVideoDurationMilli;
    
    cutEndPositionMilli = edit.machine->getCurrentStateEnd() * currentVideoDurationMilli;
    cutLenghtMilli = cutEndPositionMilli - cutStartPositionMilli;
    
    currentVideo.setPosition( cutStartPositionPercent );
    currentVideo.play();
    
    edit.machineController->updateViewDataVideo( videoSelected , &currentVideo );
    
    //Camera
    camera = new ofEasyCam();
    camera->setDistance(1500);
}
//--------------------------------------------------------------
void ofApp::setupGUI(){
    // we add this listener before setting up so the initial circle resolution is correct
    gui.setup();
    sliderCutMinimumLenghtMilli.addListener(this, &ofApp::sliderCutMinimumLenghtMilliChanged);
    sliderCutMaximusLenghtMilli.addListener(this, &::ofApp::sliderCutMaximumLenghtMillihanged);
    
    gui.add( sliderCutMinimumLenghtMilli.setup("Min cut llenght", 2000, 1000, 50000));
    gui.add( sliderCutMaximusLenghtMilli.setup("Max cut llenght", 8000, 1000, 50000));
}
//--------------------------------------------------------------
void ofApp::sliderCutMinimumLenghtMilliChanged(int &sliderCutMinimumLenghtMilli){
    cutMinimumLenghtMilli = sliderCutMinimumLenghtMilli;
}
//--------------------------------------------------------------
void ofApp::sliderCutMaximumLenghtMillihanged(int &sliderCutMaximumLenghtMilli ){
    cutMaximusLenghtMilli = sliderCutMaximumLenghtMilli;
}
//--------------------------------------------------------------
void ofApp::update(){
    if( hasFinishedPlaying ){
        updateALeatorio();
        updateClosedMachine();
    }
    else
        if( currentVideoPositionMilli >= cutEndPositionMilli )
            hasFinishedPlaying = true;
    currentVideo.update();
    currentVideoPositionNormalized = currentVideo.getPosition();
    currentVideoPositionMilli = currentVideoPositionNormalized * currentVideoDurationMilli;
    
}
//--------------------------------------------------------------
void ofApp::updateClosedMachine(){
    //stop previous video
    currentVideo.stop();
    
    if( ! edit.machine->step() )
        cout << "Machine is closed fuckerssss!!!!!!";
    
    //chose a random next video
    videoSelected = edit.machine->getCurrentStateVideoIndex();
    currentVideo = videos[ videoSelected ];
    currentVideo.setSpeed( currentVideoSpeed );
    currentVideoDurationMilli = currentVideo.getDuration() * 1000;
    
    cutStartPositionPercent = edit.machine->getCurrentStateStart();
    cutStartPositionMilli = cutStartPositionPercent * currentVideoDurationMilli;

    cutEndPositionMilli = edit.machine->getCurrentStateEnd() * currentVideoDurationMilli;
    cutLenghtMilli = cutEndPositionMilli - cutStartPositionMilli;
    
    currentVideo.setPosition( cutStartPositionPercent );
    currentVideo.play();
    
    edit.machineController->updateViewDataVideo( videoSelected , &currentVideo );
    
    hasFinishedPlaying = false;
}
//--------------------------------------------------------------
void ofApp::updateALeatorio(){
    //stop previous video
    currentVideo.stop();
    
    //chose a random next video
    videoSelected = ofRandom( 0 , videos.size() );
    currentVideo = videos[ videoSelected ];
    currentVideo.setSpeed( currentVideoSpeed );
    currentVideoDurationMilli = currentVideo.getDuration() * 1000;
    
    //calculating start position for the cut
    if( isFirstTime ){
        cutStartPositionPercent = 0;
        isFirstTime = false;
    }
    else
        cutStartPositionPercent = ofRandom( 0 , delataEnd );
    cutStartPositionMilli = cutStartPositionPercent * currentVideoDurationMilli;
    
    //calculating cut lenght
    cutLenghtMilli = ofRandom( cutMinimumLenghtMilli , cutMaximusLenghtMilli );
    
    //loock if the end is out of the video.
    if(cutStartPositionMilli + cutLenghtMilli >= delataEnd * currentVideoDurationMilli )
        cutLenghtMilli = currentVideoDurationMilli - cutStartPositionMilli;
    cutEndPositionMilli = cutStartPositionMilli + cutLenghtMilli;
    
    currentVideo.setPosition( cutStartPositionPercent );
    currentVideo.play();
    
    hasFinishedPlaying = false;
}
//--------------------------------------------------------------
void ofApp::draw(){
    currentVideo.draw( 0 , 0 , ofGetWidth() , ofGetHeight() );
    
    camera->begin();
    edit.draw();
    camera->end();
    
    drawDebugTimeline( 10 , ofGetHeight() - ofGetHeight() / 20  , ofGetWidth() - 20 , ofGetHeight() / 25 );
    drawDebugTimes( 20 , 180 );
    
    gui.draw();
}
//--------------------------------------------------------------
void ofApp::drawInteractionArea(){
    ofRectangle vp = ofGetCurrentViewport();
    float r = MIN(vp.width, vp.height) * 0.5f;
    float x = vp.width * 0.5f;
    float y = vp.height * 0.5f;
    
    ofPushStyle();
    ofSetLineWidth(3);
    ofSetColor(255, 255, 0);
    ofNoFill();
    glDepthMask(false);
    ofDrawCircle(x, y, r);
    glDepthMask(true);
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::drawDebugTimes( int x , int y ){
    string debugString;
    ofFill();
    ofEnableAlphaBlending();
    ofSetColor( 20 , 200 , 0 , 100 );
    ofDrawRectangle( x - 10 , y - 20 , 200 , 180 );
    
    ofDisableAlphaBlending();
    ofNoFill();
    ofSetColor( 255,255,255  );
    
    debugString += "Current  video ";
    debugString += ofToString( videoSelected );
    debugString += "\nTime: ";
    debugString += getTimeStringFromMilli( currentVideo.getPosition() * currentVideoDurationMilli );
    debugString += "\n-------------------------------------\n";
    
    debugString += "Started at:  ";
    debugString += getTimeStringFromMilli( cutStartPositionPercent * currentVideoDurationMilli );
    debugString += "\n";
    
    debugString += "Ends at:    ";
    debugString += getTimeStringFromMilli( cutStartPositionPercent * currentVideoDurationMilli + cutLenghtMilli );
    debugString += "\n-------------------------------\n";
    
    debugString += "Duration:   ";
    debugString += getTimeStringFromMilli(  cutLenghtMilli );
    debugString += "\n-------------------------------\n";
    
    debugString += "Speed: X";
    debugString += ofToString( currentVideoSpeed );
    
    font.drawString(debugString , x , y  );
}
//--------------------------------------------------------------
void ofApp::drawDebugTimeline( int x , int y , int w , int h ){
    ofFill();
    ofEnableAlphaBlending();
    ofSetColor( 0 , 190 , 0 , 100 );
    ofDrawRectangle( x , y , w , h );
    float cutLenghtPercent = float(cutLenghtMilli )/ float(currentVideoDurationMilli);
    ofSetColor( 200 , 30 , 90  , 100);
    ofDrawRectangle( x + w * cutStartPositionPercent  , y - h * .1  , w * cutLenghtPercent , h + h * .2 );
    ofSetColor( 50 , 100 , 200 , 255 );
    ofDrawLine( x + w * currentVideoPositionNormalized , y  , x + w * currentVideoPositionNormalized , y + h );
    ofDisableAlphaBlending();
    ofNoFill();
    ofSetColor( 255,255,255  );
}
//--------------------------------------------------------------
string ofApp::getTimeStringFromMilli (long milliseconds ){
    int deciseconds = 0;
    int seconds = 0;
    int minutes = 0;
    int hours = 0;
    int days = 0;
    deciseconds = milliseconds % 1000;
    if( deciseconds > 10 ){
        seconds = milliseconds / 1000;
        deciseconds %= 1000;
        if( seconds > 60 ){
            minutes += seconds / 60;
            seconds %= 60;
            if( minutes > 60 ){
                hours += minutes / 60;
                minutes %= 60;
            }
        }
    }
    
    string timeInString;
    if( hours < 10 )
        timeInString += "0";
    timeInString += ofToString(hours);
    timeInString += ":";
    
    if( minutes < 10 )
        timeInString += "0";
    timeInString += ofToString(minutes);
    timeInString += ":";
    
    if( seconds < 10 )
        timeInString += "0";
    timeInString += ofToString(seconds);
    
    timeInString += ":";
    timeInString += ofToString(deciseconds);
    return timeInString;
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if( key == 'q' || key == 'a' ){
        if( key == 'q' )
            currentVideoSpeed *=2;
        if( key == 'a' )
            currentVideoSpeed /=2;
        
        if( currentVideoSpeed > 16 )
            currentVideoSpeed = 16;
        if( currentVideoSpeed < 1 )
            currentVideoSpeed = 1;
        currentVideo.setSpeed( currentVideoSpeed );
    }
    
    if( key == ' ' )
        hasFinishedPlaying = true;
    
    if(key == 's')
        gui.saveToFile("settings.xml");
    
    if(key == 'l')
        gui.loadFromFile("settings.xml");
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
