#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    
    ofSetSphereResolution(24);
    
    //Textures
    texture.mirror(1,0);
    texture.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    //lights
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    pointLightTime.setDiffuseColor(ofFloatColor(.85, .85, .55) );
    pointLightTime.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    //material
    material.setShininess( 120 );
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    //Camera
    camera = new ofEasyCam();
    camera->setDistance(1200);
    
    font.load( "Contl___.ttf" , 12 );
    
    hasFinishedPlaying = true;
    videoRandomChoiceIndex;
    
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
    
    delataEnd = 0.999;
    
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
    
    for( int v = 0 ; v < videos.size() ; v ++ )
        videos[ v ].stop();
    
    isFirstTime = true;
    
    
    edit.setup( camera );
}
//--------------------------------------------------------------
void ofApp::setupGUI(){
    // we add this listener before setting up so the initial circle resolution is correct
    gui.setup();
    sliderCutMinimumLenghtMilli.addListener(this, &ofApp::sliderCutMinimumLenghtMilliChanged);
    sliderCutMaximusLenghtMilli.addListener(this, &::ofApp::sliderCutMaximumLenghtMillihanged);
    
    gui.add( sliderCutMinimumLenghtMilli.setup("Min cut llenght", 2000, 1000, 500000));
    gui.add( sliderCutMaximusLenghtMilli.setup("Max cut llenght", 8000, 1000, 500000));
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
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    
    //edit.update( directorEnergy , volume );
    
    if( hasFinishedPlaying )
        updateALeatorio01();
    else
        if( currentVideoPositionMilli >= cutEndPositionMilli )
            hasFinishedPlaying = true;
    currentVideo.update();
    currentVideoPositionNormalized = currentVideo.getPosition();
    currentVideoPositionMilli = currentVideoPositionNormalized * currentVideoDurationMilli;
    
}
//--------------------------------------------------------------
void ofApp::updateALeatorio01(){
    //stop previous video
    currentVideo.stop();
    
    //chose a random next video
    videoRandomChoiceIndex = ofRandom( 0 , videos.size() );
    currentVideo = videos[ videoRandomChoiceIndex ];
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
    ofSetHexColor(0xFFFFFF);
    currentVideo.draw( 0 , 0 , ofGetWidth() , ofGetHeight() );
    
    camera->begin();
    
    ofEnableDepthTest();
    
    ofEnableLighting();
    
    //material.begin();
    
    //pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    pointLightTime.enable();
    
    
    edit.draw();
    
    //    drawEnergy( ofVec3f( -700 , 300 , 0 ) , 400 , 20 );
    //    drawVolumen( ofVec3f( -800 , 300 , 0 ) , 400 , 10 );
    
    //material.end();
    
    ofDisableLighting();
    
    ofDisableDepthTest();
    
    ofFill();
    
    camera->end();
    
    drawDebugTimeline( 10 , 100 , ofGetWidth() - 20 , ofGetHeight() / 25 );
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
    debugString += ofToString( videoRandomChoiceIndex );
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
