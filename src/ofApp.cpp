#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    ofSetWindowTitle("Kafka Machine App");
    
    ofBackground(140,140,160);
    ofSetVerticalSync(true);
    
    font.load( "Contl___.ttf" , 12 );
    fontNameClosedMachine.load( "Contl___.ttf" , 50 );
    
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
    
    //if( interviews_ed.load("movies/Scene11_Interviews_Edit.mov") )
    if( interviews_ed.load("movies/Scene5_Stoker_Lanzarote.mov") )
        videos.push_back( interviews_ed );
    
    //if( interviews_all.load("movies/Scene12_Interviews_All.mov")
    if( interviews_all.load("movies/Scene5_Stoker_Lanzarote.mov") )
        videos.push_back( interviews_all );
    
    //if( boat_entrance.load("movies/Scene13_Boat_Entrance.mov") )
    if( boat_entrance.load("movies/Scene5_Stoker_Lanzarote.mov") )
        videos.push_back( boat_entrance );
    
    if( boat.load("movies/Scene14_Boat_Edit-2.mov") )

        videos.push_back( boat );
    
    for( int v = 0 ; v < videos.size() ; v ++ ){
        videos[ v ].stop();
        //videos[ v] .setVolume(0);
    }
    
//    //text effect
//    textEffectSize = 120;
//    fontTextEffect.load( "Contl___.ttf" , textEffectSize );
//    
//    textEfectUpdateRefreshMin = 40;
//    numLetersInTextEffectMin = 3;
//    
//    if( !loadTextForEffect(("text/Text01.txt")) )
//        cout << "\nCouldnt fint test efect file :( ";
//    
//    for( int c = firstEffectCharacter ; c < firstEffectCharacter + numLetersInTextEffect ; c ++ )
//        textDrawingEffect.push_back( textForEffect[ c ] );
//    firstEffectCharacter++;
//    textEffectDirection = 1;
//    textEffectPosition = ofVec3f( 100 , 900 , 0 );
//    textEffectRotation = ofVec3f( 0 , 0 , 0 );
//    
//    firstEffectCharacter = 0;
//    lasttextEfectUpdateRefresh = 0;

    
    //flags
    isFirstTime = true;
    
    //machines closed loops
    KafkaClosedMachine* closedMachine01 = new KafkaClosedMachine();
    closedMachine01->setup( "PERCURSO FECHADO I" , "machines/Percurso Fechado I.tsv" , videos.size() );
    
    KafkaClosedMachine* closedMachine02 = new KafkaClosedMachine();
    closedMachine02->setup( "PERCURSO FECHADO II" , "machines/Percurso Fechado II.tsv" , videos.size() );
    
    machinesClosed.push_back( closedMachine01 );
    machinesClosed.push_back( closedMachine02 );

    currentClosedMacineIndex = 0;
    currentClosedMacine = machinesClosed[ currentClosedMacineIndex ];
    
    closedMachinesUpdateRefreshMin = 10000;
    closedMachinesUpdateRefreshMax = 20000;
    
    lastClosedMachinesUpdateTime = 0;
    
    closedMachinesUpdateRefresh = ofRandom( closedMachinesUpdateRefreshMin , closedMachinesUpdateRefreshMax );
    
    //machine Random
    
    //machine Random
    //machineRandom = new KafkaFullPopulatedMachine();
    //machineRandom->setup( "RANDOM + ENERGY" , videos.size() );
    
    machineRandomSemipoulated = new KafkaSemiPopulatedMachine();
    machineRandomSemipoulated->setup( "RANDOM" , videos.size() );
    
    //machine Energy
    machineEnergys = new KafkaFullPopulatedMachine();
    machineEnergys->setup( "ENERGY" , videos.size() );
    
    //Camera
    camera = new ofEasyCam();
    camera->setDistance(1500);
    
    setAppState( APP_STATE_RANDOM );
    
    //zoom effect
    textureVideo.allocate( currentVideo->getWidth() , currentVideo->getHeight() ,GL_RGB );
    
    //lights
    ofSetSmoothLighting(true);
    pointLight01.setDiffuseColor( ofFloatColor( .8 , .5 , .7 ) );
    pointLight01.setSpecularColor( ofFloatColor( .5 , .5 , .5 ) );
    
    pointLight02.setDiffuseColor( ofFloatColor( .6, .5, .8 ) );
    pointLight02.setSpecularColor(ofFloatColor(.5 , .5 , .5 ) );
    
    pointLight03.setDiffuseColor( ofFloatColor( .8 , .8, .9 ) );
    pointLight03.setSpecularColor( ofFloatColor( 1 , 1 , 1 ) );
    
    pointLight01.enable();
    pointLight02.enable();
    pointLight03.enable();

    //arduino hardware
    setupHardware();
    
    //parametaers INITIAL MAX and MIN values to set up by teh artist
    currentVideoBrightness = 1;
    currentVideoZoom = 1;
    //currentVideoText = .5;
    currentMachineRotation = 0;
    currentMachineTranslation = 0;
    
    currentEnergy01 = .5;
    currentEnergy02 = .5;
    currentEnergy03 = .5;
    
    currentVideoBrightnessMax = 8;
    currentVideoBrightnessMin = 1;
    
    currentVideoZoomMax = 10;
    currentVideoZoomMin = 1;
    
    //currentVideoTextMax = 10;
    //currentVideoTextMin = 1;
    
    currentVideoMachineRotationMax = 360;
    currentVideoMachineRotationMin = -360;
    
    currentMachineTranslationMax = 1500;
    currentMachineTranslationMin = -11000;
    
    currentEnergy01Max = 1;
    currentEnergy01Min = 0;
    
    currentEnergy02Max = 1;
    currentEnergy02Min = 0;
    
    currentEnergy03Max = 1;
    currentEnergy03Min = 0;
}
//--------------------------------------------------------------
bool ofApp::setupHardware(){
    isArduinoHardwarePresent = false;
    string portName = "/dev/cu.usbserial-A4001qyq";
    if( !hardware.setup( portName ) ){
        cout << "Couldnt find arduino at port : " << portName << "/n";
        return isArduinoHardwarePresent;
    }
    
    isArduinoHardwarePresent = true;
    harwareUpdateRefresh = 50;
    lastHardwareUpdateRefresh = 0;
    cout << "\nWaiting for arduino ";
    return isArduinoHardwarePresent;
}
//--------------------------------------------------------------
void ofApp::update(){
    long cutTimeMillis = ofGetElapsedTimeMillis();
    switch( appState ){
        case APP_STATE_RANDOM:
            if( hasFinishedPlaying )
                updateRandom();
            else
                if( currentVideoPositionMilli >= cutEndPositionMilli )
                    hasFinishedPlaying = true;
            //machineRandom->update();
            machineRandomSemipoulated->update();
            break;
            
        case APP_STATE_CLOSED_MACHINES:
            if( hasFinishedPlaying ){
                if( cutTimeMillis - lastClosedMachinesUpdateTime > closedMachinesUpdateRefresh ){
                    closedMachinesUpdateRefresh = ofRandom( closedMachinesUpdateRefreshMin , closedMachinesUpdateRefreshMax );
                    lastClosedMachinesUpdateTime = cutTimeMillis;
                    //jumpToOtherClosedMachine();
                }
                updateClosedMachine();
            }
            else
                if( currentVideoPositionMilli >= cutEndPositionMilli )
                    hasFinishedPlaying = true;
            currentClosedMacine->update();
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
    for( int p = 0 ; p < numPixels ; p +=3 ){
        pixelsVideo[p] *= currentVideoBrightness ;
        if( pixelsVideo[p] >= 254 )
            pixelsVideo[p] = 254;
    }
    textureVideo.loadData( pixelsVideo );
    
    //timers for ending cuts
    currentVideoPositionNormalized = currentVideo->getPosition();
    currentVideoPositionMilli = currentVideoPositionNormalized * currentVideoDurationMilli;
    
    //updating arduino hardware either GUI
    if( !updateHardware() )
        updateGUI();
    
    //updateTextEffect();
    
    //updateLights
    positionPointLight01.x = 0 + cos( .5 * ofGetElapsedTimef() ) * ofGetWidth();
    positionPointLight01.y = 0;
    positionPointLight01.z = 0 + sin( .5 * ofGetElapsedTimef() ) * ofGetWidth();
    
    positionPointLight02.x = 0 + cos( PI + .5 * ofGetElapsedTimef() ) * ofGetWidth();
    positionPointLight02.y = 0;
    positionPointLight02.z = 0 + sin( PI + .5 * ofGetElapsedTimef() ) * ofGetWidth();
    
    positionPointLight03.x = 0;
    positionPointLight03.y = 0 + cos( .2 * ofGetElapsedTimef() ) * ofGetWidth();
    positionPointLight03.z = 0 + sin( .2 * ofGetElapsedTimef() ) * ofGetWidth();
    
    pointLight01.setPosition( positionPointLight01 );
    pointLight02.setPosition( positionPointLight02 );
    pointLight03.setPosition( positionPointLight03 );

}
//--------------------------------------------------------------
bool ofApp::updateHardware(){
    if( !isArduinoHardwarePresent ){
        //cout << "Arduino couldn't Initialize\n";
        return false;
    }
    
    long cutTimeMillis = ofGetElapsedTimeMillis();
    hardware.update();
    if( !hardware.isRuning() ){
        cout << "Arduino couldnt Run this time\n";
        return false;
    }
    
    if( cutTimeMillis - lastHardwareUpdateRefresh > harwareUpdateRefresh ){
        
        currentVideoBrightness = ofMap( hardware.getBrightness() , 1023 , 0 , currentVideoBrightnessMin , currentVideoBrightnessMax );
        currentVideoZoom = ofMap( hardware.getZoom() , 1023 , 0 , currentVideoZoomMin , currentVideoZoomMax );
        currentMachineRotation = ofMap( hardware.getText() , 1023 , 0 , currentVideoMachineRotationMin , currentVideoMachineRotationMax );
        currentMachineTranslation = ofMap( hardware.getText() , 1023 , 0 , currentMachineTranslationMin , currentMachineTranslationMax );
        currentEnergy01 = ofMap( hardware.getEnergy01() , 1023 , 0 , currentEnergy01Min , currentEnergy01Max );
        currentEnergy02 = ofMap( hardware.getEnergy02() , 1023 , 0 , currentEnergy02Min , currentEnergy02Max );
        currentEnergy03 = ofMap( hardware.getEnergy03() , 1023 , 0 , currentEnergy03Min , currentEnergy03Max );
        //currentVideoText = ofMap( hardware.getText() , 1023 , 0 , currentVideoTextMin, currentVideoTextMax );
        
        int newAppState =  hardware.getAppState();
        if( newAppState != appState )
            setAppState( appStates(newAppState) );
        else if( hardware.justPresedButton() )
            jumpToOtherClosedMachine();
    
        lastHardwareUpdateRefresh = cutTimeMillis;
    }
    return true;
}
//--------------------------------------------------------------
void ofApp::updateGUI(){
    sliderBrightness.valueChanged( currentVideoBrightness );
    sliderZoom.valueChanged( currentVideoZoom );
    sliderText.valueChanged( currentMachineRotation );
    
    sliderEnergy01.valueChanged( currentEnergy01 );
    sliderEnergy02.valueChanged( currentEnergy02 );
    sliderEnergy03.valueChanged( currentEnergy03 );

}
//--------------------------------------------------------------
void ofApp::jumpToOtherClosedMachine(){
    int newMachineIndex = ofRandom( machinesClosed.size() );
    while( newMachineIndex == currentClosedMacineIndex )
        newMachineIndex = ofRandom( machinesClosed.size() );
    currentClosedMacineIndex = newMachineIndex;
    currentClosedMacine = machinesClosed[ newMachineIndex ];
    updateClosedMachine();

}
////--------------------------------------------------------------
//void ofApp::updateTextEffect(){
//    return;
//    long cutTimeMillis = ofGetElapsedTimeMillis();
//    textEfectUpdateRefresh = textEfectUpdateRefreshMin * ( currentVideoTextMax - currentVideoText );
//    numLetersInTextEffect = numLetersInTextEffectMin * currentVideoText;
//    if(  cutTimeMillis - lasttextEfectUpdateRefresh > textEfectUpdateRefresh ){
//        textDrawingEffect.clear();
//        if( textEffectDirection == 1 ){
//            firstEffectCharacter++;
//            for( int c = firstEffectCharacter ; c < firstEffectCharacter + numLetersInTextEffect ; c ++ )
//                textDrawingEffect.push_back( textForEffect[ c ] );
//            if( firstEffectCharacter + numLetersInTextEffect >= textForEffect.size() )
//                textEffectDirection = -1;
//        }
//        else{
//            firstEffectCharacter--;
//            for( int c = firstEffectCharacter ; c < firstEffectCharacter + numLetersInTextEffect ; c ++ )
//                textDrawingEffect.push_back( textForEffect[ c ] );
//            if( firstEffectCharacter <= 0 )
//                textEffectDirection = 1;
//        }
//        lasttextEfectUpdateRefresh = cutTimeMillis;
//    }
//}
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
    
    //machineRandom->machine->stepRandom();
    machineRandomSemipoulated->machine->stepRandom();
    
    //chose a random next video
    //int indexVideo = machineRandom->machine->getCurrentStateVideoIndex();
    //float startPercent = machineRandom->machine->getCurrentStateStart();
    //float endPercent = machineRandom->machine->getCurrentStateEnd();
    int indexVideo = machineRandomSemipoulated->machine->getCurrentStateVideoIndex();
    float startPercent = machineRandomSemipoulated->machine->getCurrentStateStart();
    float endPercent = machineRandomSemipoulated->machine->getCurrentStateEnd();
    //startPercent = machineEnergys->machine->getCurrentStateStart();
    if( isFirstTime ){
        startPercent = 0;
        isFirstTime = false;
    }
    setCurrentVideoState( indexVideo , startPercent , endPercent );
    //machineRandom->machineController->updateViewDataVideo( videoSelected , currentVideo );
    //machineRandom->machineController->update();
    machineRandomSemipoulated->machineController->updateViewDataVideo( videoSelected , currentVideo );
    machineRandomSemipoulated->machineController->update();
}
//--------------------------------------------------------------
void ofApp::updateClosedMachine(){
    if( appState != APP_STATE_CLOSED_MACHINES )
        return;
    
    if( !currentClosedMacine->machine->step() )
        cout << "Machine is closed fuckerssss!!!!!!";
    
    //chose a random next video
    int indexVideo = currentClosedMacine->machine->getCurrentStateVideoIndex();
    float startPercent = currentClosedMacine->machine->getCurrentStateStart();
    float endPercent = currentClosedMacine->machine->getCurrentStateEnd();
    if( isFirstTime ){
        startPercent = 0;
        isFirstTime = false;
    }
    setCurrentVideoState( indexVideo , startPercent , endPercent );
    currentClosedMacine->machineController->updateViewDataVideo( videoSelected , currentVideo );
    currentClosedMacine->machineController->update();
    
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
    
    if( currentVideoZoom != 1 ){
        ofPushMatrix();
        ofTranslate(( 1-currentVideoZoom )* ( ofGetWidth() / 2 )  , (1-currentVideoZoom) * ( ofGetHeight() / 2 ) );
        ofScale( currentVideoZoom , currentVideoZoom );
        textureVideo.draw(0,0, ofGetWidth() , ofGetHeight() );
        ofPopMatrix();
    }
    else
        textureVideo.draw( 0 , 0 , ofGetWidth() , ofGetHeight() );
    
    
    camera->begin();
    ofPushMatrix();
    ofTranslate( 0,0, currentMachineTranslation );
    ofRotate( currentMachineRotation , 0, 1, 0);

    switch( appState ){
        case APP_STATE_RANDOM:
            //machineRandom->draw();
            machineRandomSemipoulated->draw();
            break;
            
        case APP_STATE_CLOSED_MACHINES:
            currentClosedMacine->draw();
            
            break;
            
        case APP_STATE_ENERGYS:
            machineEnergys->draw();
            break;
    }
    ofPopMatrix();
    camera->end();
    
    ofSetColor(255);
    ofDisableLighting();
    //drawDebugTimeline( 10 , ofGetHeight() - ofGetHeight() / 20  , ofGetWidth() - 20 , ofGetHeight() / 25 );
    //drawDebugTimes( 20 , 420 );;
    
//    //text effect
//    ofPushMatrix();
//    ofTranslate( textEffectPosition );
//    fontTextEffect.drawString( textDrawingEffect , 0 , 0 );
//    ofPopMatrix();
    
    if( appState  == APP_STATE_CLOSED_MACHINES )
        fontNameClosedMachine.drawString( currentClosedMacine->machine->getName() ,  100  , ofGetHeight() - 200 );
    
    if( hardware.isRuning() )
        drawHardware( 20 , 50 );
        //hardware.draw(20 , 50);
    else
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
void ofApp::drawHardware( int x , int y ){
    ofFill();
    ofEnableAlphaBlending();
    ofSetColor( 20 , 200 , 0 , 100 );
    ofDrawRectangle( x - 10 , y - 20 , 160 , 180 );
    ofDisableAlphaBlending();
    ofNoFill();
    
    ofSetColor( 255,255,255  );
    
    if( !hardware.isRuning() ){
        font.drawString("Arduino not connected...\n", x, y);
    } else {
        string result;
        switch( appState ){
            case APP_STATE_RANDOM:
                result += "RANDOM MACHINE";
                break;
                
            case APP_STATE_CLOSED_MACHINES:
                result += "CLOSED MACHINES";
                break;
                
            case APP_STATE_ENERGYS:
                result += "ENERGY MACHINE";
                break;
        }

        result += "\n\nBrigthness : " + ofToString( currentVideoBrightness , 2 );
        result += "\nZoom : " + ofToString( currentVideoZoom , 2 );
        result += "\nTrans : " + ofToString( currentMachineTranslation , 2 );
        result += "\n\nEnergy01 : " + ofToString( currentEnergy01 , 2 );
        result += "\nEnergy02 : " + ofToString( currentEnergy02 , 2 );
        result += "\nEnergy03 : " + ofToString( currentEnergy03 , 2 );
        
        font.drawString( result , x , y );
    }
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
    
    if( key == 'j' )
        jumpToOtherClosedMachine();
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
////--------------------------------------------------------------
//bool ofApp::loadTextForEffect( string fileName ){
//    if( !fileIn )
//        fileIn = new ifstream();
//    
//    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
//    if ( !fileIn->is_open() ){
//        cout << "Machine File not found: ";
//        cout << fileName << "\n";
//        fileIn->close();
//        return false;
//    }
//    
//    textForEffect = "";
//    string read;
//    string readPrev;
//    (*fileIn) >> read;
//    while( ( read != readPrev && read != " " ) ) {
//        readPrev = read;
//        (*fileIn) >> read;
//        textForEffect += read;
//        textForEffect += "  ";
//    }
//    fileIn->close();
//    return true;
//}
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
    guiGlobal.add( sliderBrightness.setup("bright", currentVideoBrightness , currentVideoBrightnessMin , currentVideoBrightnessMax  ));
    guiGlobal.add( sliderZoom.setup("zoom", currentVideoZoom , currentVideoZoomMin , currentVideoZoomMax  ));
 //   guiGlobal.add( sliderText.setup("rot", currentVideoText , currentVideoTextMin , currentVideoTextMax  ));
    
    guiGlobal.add( sliderText.setup("rot", currentMachineRotation , currentVideoMachineRotationMin , currentVideoMachineRotationMax  ));
    
    //enery
    guiEnergys.add( sliderEnergy01.setup("energy01", currentEnergy01 , currentEnergy01Min , currentEnergy01Max ));
    guiEnergys.add( sliderEnergy02.setup("energy02", currentEnergy02 , currentEnergy02Min , currentEnergy02Max ));
    guiEnergys.add( sliderEnergy03.setup("energy03", currentEnergy03 , currentEnergy03Min , currentEnergy03Max ));
    
    currentVideoBrightness = sliderBrightness;;
    currentVideoZoom = sliderZoom;
    //currentVideoText = sliderText;
    currentMachineRotation = sliderText;
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
    currentMachineRotation = slidetText;
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
