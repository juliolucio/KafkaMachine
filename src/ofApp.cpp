#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    ofSetWindowTitle("Kafka Machine App");
    
    ofBackground(140,140,160);
    ofSetVerticalSync(true);
    
    font.load( "Contl___.ttf" , 12 );
    
    cutLenghtMilli = 0;
    
    cutStartPositionPercent = 0;
    cutStartPositionMilli = 0;
    
    cutEndPositionMilli = 0;
    
    currentVideoDurationMilli = 0;
    
    currentVideoPositionNormalized = 0;
    currentVideoPositionMilli = 0;
    
    currentVideoBrightness = sliderBrightness;
    
    currentVideoSpeed = 1;
    
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
    
    //machines closed loops
    KafkaClosedMachine* closedMachineTest = new KafkaClosedMachine();
    closedMachineTest->setup( "MACHINE_BOAT" , "machines/machinesTest/MachineA.tsv" , videos.size() );
    machinesClosed.push_back( closedMachineTest );
    
    //machine Random
    machineRandom = new KafkaFullPopulatedMachine();
    machineRandom->setup( "Machine_RANDOM" , videos.size() );
    
    //machine Energy
    machineEnergys = machineRandom;
    
    //Camera
    camera = new ofEasyCam();
    camera->setDistance(1500);
    
    setAppState( APP_STATE_ENERGYS );
    
    textureVideo.allocate( currentVideo->getWidth() , currentVideo->getHeight() ,GL_RGB );
    
    string portName = "/dev/cu.usbserial-A4001qyq";
    if( !hardware.setup( portName ) )
        cout << "couldnt find arduino at port : " << portName << "/n";
    lastHardwareUpdateRefresh = 500;
}
//--------------------------------------------------------------
void ofApp::update(){
    switch( appState ){
        case APP_STATE_RANDOM:
            if( hasFinishedPlaying )
                updateRandom();
            else
                if( currentVideoPositionMilli >= cutEndPositionMilli )
                    hasFinishedPlaying = true;
            machineRandom->update();
            break;
            
        case APP_STATE_CLOSED_MACHINES:
            if( hasFinishedPlaying )
                updateClosedMachine();
            else
                if( currentVideoPositionMilli >= cutEndPositionMilli )
                    hasFinishedPlaying = true;
            machinesClosed[0]->update();
            break;
            
        case APP_STATE_ENERGYS:
            if( hasFinishedPlaying )
                updateEnergys();
            else
                if( currentVideoPositionMilli >= cutEndPositionMilli )
                    hasFinishedPlaying = true;
            machineEnergys->update();
            break;
    }
    
    //updating brightness effect
    currentVideo->update();
    pixelsVideo = currentVideo->getPixels();
    int numPixels = currentVideo->getWidth() * currentVideo->getHeight() * 3;
    for( int p = 0 ; p < numPixels ; p ++ )
        pixelsVideo[p] *= currentVideoBrightness ;
    textureVideo.loadData( pixelsVideo );
    
    //timers for ending cuts
    currentVideoPositionNormalized = currentVideo->getPosition();
    currentVideoPositionMilli = currentVideoPositionNormalized * currentVideoDurationMilli;
    
    if( !updateHardware() )
        cout << "couldnt update arduino /n";
}
//--------------------------------------------------------------
bool ofApp::updateHardware(){
    if( !hardware.isRuning() )
        return false;
    hardware.update();
    cutTimeMillis = ofGetElapsedTimeMillis();
    if(  cutTimeMillis - lastHardwareUpdateRefresh > harwareUpdateRefresh ){
        currentVideoBrightness = ofMap( hardware.getBrightness() , 0 , 1023 , 0 , 20 );
        currentVideoZoom = ofMap( hardware.getZoom() , 0 , 1023 , 0 , 1 );
        currentVideoText = ofMap( hardware.getText() , 0 , 1023 , 0 , 1 );
        currentEnergy01 = ofMap( hardware.getEnergy01() , 0 , 1023 , 0 , 1 );
        currentEnergy02 = ofMap( hardware.getEnergy02() , 0 , 1023 , 0 , 1 );
        currentEnergy03 = ofMap( hardware.getEnergy03() , 0 , 1023 , 0 , 1 );
        
        sliderBrightness.valueChanged( currentVideoBrightness );
        sliderZoom.valueChanged( currentVideoZoom );
        sliderText.valueChanged( currentVideoText );
        
        sliderEnergy01.valueChanged( currentEnergy01 );
        sliderEnergy02.valueChanged( currentEnergy02 );
        sliderEnergy03.valueChanged( currentEnergy03 );
        
        int newAppState =  hardware.getAppState();
        if( newAppState != appState )
            setAppState( appStates(newAppState) );
        lastHardwareUpdateRefresh = cutTimeMillis;
    }
    return true;
}
//--------------------------------------------------------------
void ofApp::setAppState( appStates theState  ){
    appState = theState;
    switch( appState ){
        case APP_STATE_RANDOM:
            updateRandom();
            break;
            
        case APP_STATE_CLOSED_MACHINES:
            updateClosedMachine();
            break;
            
        case APP_STATE_ENERGYS:
            updateEnergys();
            break;
    }
}
////--------------------------------------------------------------
void ofApp::setCurrentVideoState( int theIndexVideoSelected , float theStatrtPercent , float theEndPercent ){
    previousVideo = currentVideo;
    
    videoSelected = theIndexVideoSelected;
    currentVideo = &videos[ theIndexVideoSelected ];
    currentVideo->setSpeed( currentVideoSpeed );
    currentVideoDurationMilli = currentVideo->getDuration() * 1000.0f;
    
    cutStartPositionPercent = theStatrtPercent;
    cutStartPositionMilli = cutStartPositionPercent * currentVideoDurationMilli;
    
    cutEndPositionMilli = theEndPercent * currentVideoDurationMilli;
    
    cutLenghtMilli = cutEndPositionMilli - cutStartPositionMilli;
    
    currentVideo->setPosition( cutStartPositionPercent );
    currentVideo->play();
    if( previousVideo != currentVideo && previousVideo )
        previousVideo->stop();
    hasFinishedPlaying = false;
}
//--------------------------------------------------------------
void ofApp::updateRandom(){
    if( appState != APP_STATE_RANDOM )
        return;
    
    machineRandom->machine->stepRandom();
    
    //chose a random next video
    int indexVideo = machineRandom->machine->getCurrentStateVideoIndex();
    float startPercent = machineRandom->machine->getCurrentStateStart();
    float endPercent = machineRandom->machine->getCurrentStateEnd();
    startPercent = machineEnergys->machine->getCurrentStateStart();
    if( isFirstTime ){
        startPercent = 0;
        isFirstTime = false;
    }
    setCurrentVideoState( indexVideo , startPercent , endPercent );
    machineRandom->machineController->updateViewDataVideo( videoSelected , currentVideo );
    machineRandom->machineController->update();
}
//--------------------------------------------------------------
void ofApp::updateClosedMachine(){
    if( appState != APP_STATE_CLOSED_MACHINES )
        return;
    
    if( !machinesClosed[0]->machine->step() )
        cout << "Machine is closed fuckerssss!!!!!!";
    
    //chose a random next video
    int indexVideo = machinesClosed[0]->machine->getCurrentStateVideoIndex();
    float startPercent = machinesClosed[0]->machine->getCurrentStateStart();
    float endPercent = machinesClosed[0]->machine->getCurrentStateEnd();
    if( isFirstTime ){
        startPercent = 0;
        isFirstTime = false;
    }
    setCurrentVideoState( indexVideo , startPercent , endPercent );
    machinesClosed[0]->machineController->updateViewDataVideo( videoSelected , currentVideo );
    machinesClosed[0]->machineController->update();
    
}
//--------------------------------------------------------------
void ofApp::updateEnergys(){
    if( appState != APP_STATE_ENERGYS )
        return;
    
    currentEnergys.clear();
    currentEnergys.push_back(currentEnergy01);
    currentEnergys.push_back(currentEnergy02);
    currentEnergys.push_back(currentEnergy03);
    
    machineEnergys->machine->stepEnergys( currentEnergys );
    
    //chose a random next video
    int indexVideo = machineEnergys->machine->getCurrentStateVideoIndex();
    float startPercent = machineEnergys->machine->getCurrentStateStart();
    float endPercent = machineEnergys->machine->getCurrentStateEnd();
    if( isFirstTime ){
        startPercent = 0;
        isFirstTime = false;
    }
    setCurrentVideoState( indexVideo , startPercent , endPercent );
    machineEnergys->machineController->updateViewDataVideo( videoSelected , currentVideo );
    machineEnergys->machineController->update();
}
//--------------------------------------------------------------
void ofApp::draw(){
    //currentVideo->draw( 0 , 0 , ofGetWidth() , ofGetHeight() );
    ofSetColor(255);
    textureVideo.draw( 0 , 0 , ofGetWidth() , ofGetHeight() );
    
    camera->begin();
    switch( appState ){
        case APP_STATE_RANDOM:
            machineRandom->draw();
            break;
            
        case APP_STATE_CLOSED_MACHINES:
            machinesClosed[0]->draw();
            break;
            
        case APP_STATE_ENERGYS:
            machineEnergys->draw();
            break;
    }
    camera->end();
    
    ofSetColor(255);
    ofDisableLighting();
    drawDebugTimeline( 10 , ofGetHeight() - ofGetHeight() / 20  , ofGetWidth() - 20 , ofGetHeight() / 25 );
    drawDebugTimes( 20 , 180 );
    
    hardware.draw();
    
    drawGUI();
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
    debugString += getTimeStringFromMilli( currentVideo->getPosition() * currentVideoDurationMilli );
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
        currentVideo->setSpeed( currentVideoSpeed );
    }
    
    if( key == ' ' )
        hasFinishedPlaying = true;
    
    if(key == 's'){
        guiGlobal.saveToFile("settingsGlobal.xml");
        guiEnergys.saveToFile("settingsEnergys.xml");
    }
    
    if(key == 'l'){
        guiGlobal.loadFromFile("settingsGlobal.xml");
        guiEnergys.loadFromFile("settingsEnergys.xml");
    }
    
    if(key == '1'){
        setAppState( APP_STATE_RANDOM );
    }
    
    if(key == '2'){
        setAppState( APP_STATE_CLOSED_MACHINES );
    }
    
    if(key == '3'){
        setAppState( APP_STATE_ENERGYS );
    }
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

//--------------------------------------------------------------
//GUI
//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::setupGUI(){
    //listeners
    //globals
    sliderBrightness.addListener( this , &ofApp::sliderBrightnessChanged );
    sliderZoom.addListener( this , &ofApp::sliderZoomChanged );
    sliderText.addListener( this , &ofApp::sliderTextChanged );
    
    //enery
    sliderEnergy01.addListener( this , &ofApp::sliderEnergy01Changed );
    sliderEnergy02.addListener( this , &ofApp::sliderEnergy02Changed );
    sliderEnergy03.addListener( this , &ofApp::sliderEnergy03Changed );
    
    //panels
    guiGlobal.setup();
    guiGlobal.setPosition(ofPoint( 10 , 10 ) );
    guiEnergys.setup();
    guiEnergys.setPosition(ofPoint( 230 , 10  ) );
    
    //globals
    guiGlobal.add( sliderBrightness.setup("bright", 1, 1, 20  ));
    guiGlobal.add( sliderZoom.setup("zoom", 1, 1, 5  ));
    guiGlobal.add( sliderText.setup("text", 1, 1, 2  ));
    
    //enery
    guiEnergys.add( sliderEnergy01.setup("amor", 1 , 0.001 , 1.000  ));
    guiEnergys.add( sliderEnergy02.setup("tristeza", 1 , 0.001 , 1.000  ));
    guiEnergys.add( sliderEnergy03.setup("beleza", 1 , 0.001 , 1.000  ));
    
    currentVideoBrightness = sliderBrightness;;
    currentVideoZoom = sliderZoom;
    currentVideoText = sliderText;
    currentEnergy01 = sliderEnergy01;
    currentEnergy02 = sliderEnergy02;
    currentEnergy03 = sliderEnergy03;
}
//--------------------------------------------------------------
void ofApp::drawGUI(){
    guiGlobal.draw();
    guiEnergys.draw();
}
//--------------------------------------------------------------
//GUI Global
void ofApp::sliderBrightnessChanged(float &sliderBright ){
    currentVideoBrightness = sliderBright;
}
//--------------------------------------------------------------
void ofApp::sliderZoomChanged(float &sliderZoom ){
    currentVideoZoom = sliderZoom;
}
//--------------------------------------------------------------
void ofApp::sliderTextChanged(float &slidetText ){
    currentVideoText = slidetText;
}
//--------------------------------------------------------------
//GUI Energy
void ofApp::sliderEnergy01Changed(float &sliderEne01 ){
    currentEnergy01 = sliderEne01;
}
//--------------------------------------------------------------
void ofApp::sliderEnergy02Changed(float &sliderEne02 ){
    currentEnergy02 = sliderEne02;
}
//--------------------------------------------------------------
void ofApp::sliderEnergy03Changed(float &sliderEne03 ){
    currentEnergy03 = sliderEne03;
}

////--------------------------------------------------------------
//void ofApp::updateAleatorio(){
//    previousVideo = currentVideo;
//
//    //chose a random next video
//    videoSelected = ofRandom( 0 , videos.size() );
//    currentVideo = &videos[ videoSelected ];
//    currentVideo->setSpeed( currentVideoSpeed );
//    currentVideoDurationMilli = currentVideo->getDuration() * 1000;
//
//    //calculating start position for the cut
//    if( isFirstTime ){
//        cutStartPositionPercent = 0;
//        isFirstTime = false;
//    }
//    else
//        cutStartPositionPercent = ofRandom( 0 , delataEnd );
//    cutStartPositionMilli = cutStartPositionPercent * currentVideoDurationMilli;
//
//    //calculating cut lenght
//    cutLenghtMilli = ofRandom( 0 , cutMaximusLenghtMilli );
//
//    //loock if the end is out of the video.
//    if(cutStartPositionMilli + cutLenghtMilli >= delataEnd * currentVideoDurationMilli )
//        cutLenghtMilli = currentVideoDurationMilli - cutStartPositionMilli;
//    cutEndPositionMilli = cutStartPositionMilli + cutLenghtMilli;
//
//    currentVideo->setPosition( cutStartPositionPercent );
//    currentVideo->play();
//    if( previousVideo != currentVideo )
//        previousVideo->stop();
//
//    hasFinishedPlaying = false;
//}
//

//--------------------------------------------------------------
//void ofApp::update(){
//    if( hasFinishedPlaying ){
//        //updateAleatorio();
//        updateRandom();
//        //updateClosedMachine();
//    }
//    else
//        if( currentVideoPositionMilli >= cutEndPositionMilli )
//            hasFinishedPlaying = true;
//
//    edit.update();
//    machineRandom.update();
//    currentVideo->update();
//
//    pixelsVideo = currentVideo->getPixels();
//    int numPixels = currentVideo->getWidth() * currentVideo->getHeight() * 3;
//    for( int p = 0 ; p < numPixels ; p ++ )
//        pixelsVideo[p] *= currentVideoBrightness ;
//
//    textureVideo.loadData( pixelsVideo );
//
//    currentVideoPositionNormalized = currentVideo->getPosition();
//    currentVideoPositionMilli = currentVideoPositionNormalized * currentVideoDurationMilli;
//}
