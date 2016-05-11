//
//  KafkaStatesMachineView.cpp
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//

#include "KafkaStatesMachineView.h"
//-------------------------------------------------------------
//-------------------------------------------------------------
//public
//-------------------------------------------------------------
//-------------------------------------------------------------
KafkaStatesMachineView::KafkaStatesMachineView( string theName , int theNumVideos ){
    name = theName;
    numVideos =  theNumVideos;
    stateCurrent = -1;
    statePrevious = 0;
    lastTransition = 0;
    fileIn = 0;
    fileOut = 0;
    isItActive = true;
    
    deltaBezierX = 100;
    deltaBezierY = 100;
    
    machineGap = 100;
    
    machineSize         = ofVec3f( ofGetWidth() - 2 * machineGap , ofGetHeight() - 2 * machineGap ,  ofGetHeight() - 2 * machineGap );
    machineOrigen       = ofVec3f( machineGap , machineGap , 0 );
    machineVideoSize    = ofVec3f( ( machineSize.x - (( numVideos - 1 ) * machineGap )) / numVideos , machineSize.y , ( machineSize.x - (( numVideos - 1 ) * machineGap )) / numVideos );
    
    for( int videoIndex = 0 ; videoIndex < numVideos ; videoIndex ++ ){
        ofVec3f thisVideoPosition = machineOrigen  + ofVec3f( videoIndex * ( machineVideoSize.x + machineGap ) , 0 , 0 );;
        ofBoxPrimitive* tempBox = new ofBoxPrimitive();
        tempBox->setWidth( machineVideoSize.x );
        tempBox->setHeight( machineVideoSize.y );
        tempBox->setDepth( machineVideoSize.z );
        tempBox->setPosition( thisVideoPosition + ofVec3f( 0 , machineVideoSize.y / 2.0f , 0 ) );
        tempBox->setResolution( 1, 10, 1);
        videosBoxesPrimitives[ videoIndex ] = tempBox;
        machineVideosPositions.push_back( thisVideoPosition );
        videosColors.push_back( ofColor( ofRandom( 10 , 70 ) , 10, ofRandom( 100 , 200 ) ) );
    }
    
    type = MACHINE_VIEW_TYPE_BOXES;
    //font.load( "Contl___.ttf" , 16 );
    
    //ofDisableArbTex();
    //texture.loadImage("GUI/earth.png");
    //texture.rotate90(2);
    //texture.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
}
//-------------------------------------------------------------
KafkaStatesMachineView::~KafkaStatesMachineView(){
    clear();
}
//-------------------------------------------------------------
bool KafkaStatesMachineView::addState( string theName , int theVideoIndex , float theEnergy , float thePercentageStart , float thePercentageEnd  ){
    float percentLength = thePercentageEnd - thePercentageStart;
    float statePrimitiveHeight = percentLength *  machineVideoSize.y;
    
    ofVec3f positionObjectInit = machineVideosPositions[ theVideoIndex ] + ofVec3f( 0 , thePercentageStart *  machineVideoSize.y , 0);
    ofVec3f positionPrimitive = positionObjectInit + ofVec3f( 0 , ( percentLength / 2 ) *  machineVideoSize.y , 0);
    ofVec3f positionObjectEnd = positionPrimitive + ofVec3f( 0 , ( percentLength / 2 ) *  machineVideoSize.y , 0);
    
    map<string,float> tempMap;
    tempMap[ "energy" ] = theEnergy;
    
    statesNames             .push_back( theName );
    statesQuantizations     .push_back( tempMap );
    statesVideoIndexes      .push_back( theVideoIndex );
    statesPercentageStarts  .push_back( thePercentageStart );
    statesPercentageEnds    .push_back( thePercentageEnd );
    statesPositionsCenters  .push_back( positionPrimitive );
    statesPositionsInits    .push_back( positionObjectInit );
    statesPositionsEnds     .push_back( positionObjectEnd );
    
    of3dPrimitive* primitive;
    
    switch ( type ) {
        case MACHINE_VIEW_TYPE_BOXES:{
            ofBoxPrimitive* box = new ofBoxPrimitive();
            box->setWidth( machineVideoSize.x - 5 );
            box->setHeight( statePrimitiveHeight );
            box->setDepth( machineVideoSize.z - 5 );
            box->setPosition( positionPrimitive );
            box->setResolution( 3 );
            primitive = box;
        }
            break;
            
            
        case MACHINE_VIEW_TYPE_SPHERES:{
            ofIcoSpherePrimitive* sphere = new ofIcoSpherePrimitive();
            sphere->setRadius( statePrimitiveHeight  );
            sphere->setPosition( positionPrimitive );
            sphere->setResolution( 3 );
            primitive = sphere;
        }
            break;
            
            
        case MACHINE_VIEW_TYPE_PLANES:{
            ofPlanePrimitive* plane = new ofPlanePrimitive();
            plane->setWidth( machineVideoSize.x * percentLength );
            plane->setHeight( machineVideoSize.y );
            plane->setPosition( positionPrimitive );
            primitive = plane;
        }
            break;
            
        default:
            cout << " ERROR @@@ :bad machine view type\n";
            return;
    }
    states.insert(pair<int,of3dPrimitive*>(states.size(),primitive));
    
    if( stateCurrent == -1 )
        stateCurrent = 0;
    return true;
}
//-------------------------------------------------------------
bool KafkaStatesMachineView::addTransition( string nameState01 , string nameState02 , float theProbability ){
    ofVec3f initTransitionPosition = statesPositionsInits[ getStateIndex( nameState01 ) ];
    ofVec3f endTransitionPosition = statesPositionsEnds[ getStateIndex( nameState02 ) ];
    ofVec3f distVector = endTransitionPosition - initTransitionPosition;
    ofVec3f centerTransitionPosition =  initTransitionPosition + distVector * .5;
    //centerTransitionPosition -= ofVec3f( ( 1 + transitionStateNameInitial.size() ) * deltaBezierX , 0 , 0 );
    
    transitionStateNameInitial  .push_back( nameState01 );
    transitionStateNameFinal    .push_back( nameState02 );
    transitionStateProbabilities.push_back( theProbability );
    transitionsColors           .push_back( ofColor( 0 ,ofRandom( 100 , 250  ) , 0 ) );
    transitionsPositionsCenters .push_back( centerTransitionPosition );
    transitionsPositionsInits   .push_back( initTransitionPosition );
    transitionsPositionsEnds    .push_back( endTransitionPosition );
    return true;
}
//-------------------------------------------------------------
void KafkaStatesMachineView::draw(){
    if( !isItActive )
        return;
    
    ofVec3f positionOrigin = ofVec3f( - ofGetWidth() / 1.7 , - ofGetHeight() / 1.6 , 0 );
    
    map<int,of3dPrimitive*>::iterator it = states.begin();
    int stateIndex = 0;
    
    ofPushMatrix();
    ofTranslate( positionOrigin );
    
    while( it != states.end() ){
        of3dPrimitive* tempPrimitive = (*it).second;
        ofSetColor( 255 );
        tempPrimitive->draw();
        
        ofSetColor( 111,11,111);
        ofPoint textPosition = statesPositionsCenters[ stateIndex ];
        string textState = statesNames[ (*it).first ] ;
        ofDrawBitmapString( textState , textPosition );
        
        ofSetColor( 200 , 20 , 20  );
        textPosition = statesPositionsInits[ stateIndex ];
        textState = "ini = " + ofToString( statesPercentageStarts[ (*it).first ] );
        ofDrawBitmapString( textState , textPosition );
        
        ofSetColor( 20 , 20 , 200  );
        textPosition = statesPositionsEnds[ stateIndex ];
        textState =  + "end = " + ofToString( statesPercentageEnds[ (*it).first ] );
        ofDrawBitmapString( textState , textPosition );
        
        it++;
        stateIndex++;
    }
    
    int transitionIndex = 0;
    int numTransitions = transitionStateNameInitial.size();
    
    for( int t = 0 ; t < numTransitions ; t ++ ){
        int indexStateInitial   = getStateIndex( transitionStateNameInitial[transitionIndex] );
        int indexStateFinal     = getStateIndex( transitionStateNameFinal[transitionIndex] );
        
        ofSetColor( transitionsColors[transitionIndex ]  );
        ofDrawBezier( transitionsPositionsInits[ t ].x , transitionsPositionsInits[ t ].y , transitionsPositionsInits[ t ].z ,
                     transitionsPositionsCenters[ t ].x - deltaBezierX , transitionsPositionsCenters[ t ].y - deltaBezierY , transitionsPositionsCenters[ t ].z ,
                     transitionsPositionsCenters[ t ].x + deltaBezierX, transitionsPositionsCenters[ t ].y + deltaBezierY , transitionsPositionsCenters[ t ].z ,
                     transitionsPositionsEnds[ t ].x , transitionsPositionsEnds[ t ].y , transitionsPositionsEnds[ t ].z  );
        
        ofSetColor( 40 , 40 , 200  );
        ofDrawSphere(transitionsPositionsInits[ t ].x , transitionsPositionsInits[ t ].y , transitionsPositionsInits[ t ].z , machineVideoSize.x / 12);
        ofSetColor( transitionsColors[ t ] );
        ofDrawSphere(transitionsPositionsCenters[ t ].x , transitionsPositionsCenters[ t ].y , transitionsPositionsCenters[ t ].z , machineVideoSize.x / 8);
        ofSetColor( 200 , 40 , 40  );
        ofDrawSphere(transitionsPositionsEnds[ t ].x , transitionsPositionsEnds[ t ].y , transitionsPositionsEnds[ t ].z  , machineVideoSize.x / 12);
        
        ofDrawBitmapString( ofToString( transitionStateProbabilities[ transitionIndex ]), transitionsPositionsCenters[ t ] );
        transitionIndex++;
    }
    
    for( int videoIndex = 0 ; videoIndex < numVideos ; videoIndex ++ ){
        ofSetColor(videosColors[ videoIndex]);
        videosBoxesPrimitives[videoIndex]->drawWireframe();
        ofPoint textPosition = machineVideosPositions[videoIndex];
        string textState = "video = " + ofToString( videoIndex );
        textPosition.x -= machineVideoSize.x / 2;
        textPosition.y -= machineVideoSize.x / 2;
        ofDrawBitmapString( textState , textPosition );
    }
    ofPopMatrix();
    ofSetColor( 255 );
}
//----------------------------------------------------------------------------------
bool KafkaStatesMachineView::load( string fileName ){
    if( !fileIn )
        fileIn = new ifstream();
    
    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
    if ( !fileIn->is_open() ){
        cout << "MachineView File not found: ";
        cout << fileName << "\n";
        fileIn->close();
        return false;
    }
    
    std::string junk;
    int numStates;
    int numTransitions;
    
    (*fileIn) >> junk;
    if( junk != "numStates=" ){
        cout << "* KafkaStatesMachineView  load: Bad tag numStates\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numStates;
    
    (*fileIn) >> junk;
    if( junk != "numTransitions=" ){
        cout << "* KafkaStatesMachineView  load: Bad tag numTransitions\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numTransitions;
    
    clear();
    
    for( int s = 0 ; s < numStates ; s ++ ){
        std::string junk;
        int videoIndexCurrent;
        float energyCurrent;
        float percentageStartCurrent;
        float percentageEndCurrent;
        
        (*fileIn) >> junk;
        if( junk != "name=" ){
            cout << "* KafkaStates  State load: Bad tag name\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> name;
        
        
        (*fileIn) >> junk;
        if( junk != "videoIndex=" ){
            cout << "* KafkaStatesMachine  State load: Bad tag videoIndex\n";
            return false;
        }
        (*fileIn) >> videoIndexCurrent;
        
        (*fileIn) >> junk;
        if( junk != "energy=" ){
            cout << "* KafkaStatesMachine  State load: Bad tag energy\n";
            return false;
        }
        (*fileIn) >> energyCurrent;
        
        (*fileIn) >> junk;
        if( junk != "percentStart=" ){
            cout << "* KafkaStatesMachine  State load: Bad tag percentStart\n";
            return false;
        }
        (*fileIn) >> percentageStartCurrent;
        
        
        (*fileIn) >> junk;
        if( junk != "percentEnd=" ){
            cout << "* KafkaStatesMachine  State load: Bad tag percentEnd\n";
            return false;
        }
        (*fileIn) >> percentageEndCurrent;
        
        addState( name , videoIndexCurrent , energyCurrent , percentageStartCurrent , percentageEndCurrent );
    }
    
    for( int t = 0 ; t < numTransitions ; t ++ ){
        (*fileIn) >> junk;
        std::string stateNameInitial;
        if( junk != "stateNameInitial=" ){
            cout << "* KafkaStatesMachineView  load: Bad tag stateNameInitial\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameInitial;
        
        (*fileIn) >> junk;
        std::string stateNameFinal;
        if( junk != "stateNameFinal=" ){
            cout << "* KafkaStatesMachineView  load: Bad tag stateNameFinal\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameFinal;
        
        (*fileIn) >> junk;
        float probability;
        if( junk != "probability=" ){
            cout << "* KafkaStatesMachineView  load: Bad tag probability\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> probability;
        
        if( !addTransition( stateNameInitial , stateNameFinal, probability ) ){
            cout << "* KafkaStatesMachineView  load: couldnt add transition at load\n";
            fileIn->close();
            return false;
        }
    }
    fileIn->close();
    return true;
}//----------------------------------------------------------------------------------
bool KafkaStatesMachineView::loadFromTSV( string fileName ){
    if( !fileIn )
        fileIn = new ifstream();
    
    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
    if ( !fileIn->is_open() ){
        cout << "MachineView File not found: ";
        cout << fileName << "\n";
        fileIn->close();
        return false;
    }
    
    clear();
    std::string junk;
    int numStates;
    int numTransitions;
    
    (*fileIn) >> junk;
    if( junk != "States" ){
        cout << "* KafkaStatesMachineView  load: Bad tag States\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numStates;
    
    //crap tags
    (*fileIn) >> junk;
    if( junk != "Name" ){
        cout << "* KafkaStatesMachine  load: Bad tag Name\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "Video" ){
        cout << "* KafkaStatesMachine  load: Bad tag Video\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "Start" ){
        cout << "* KafkaStatesMachine  load: Bad tag Statrt\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "End" ){
        cout << "* KafkaStatesMachine  load: Bad tag End\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "Energy" ){
        cout << "* KafkaStatesMachine  load: Bad tag Energy\n";
        fileIn->close();
        return false;
    }
    
    for( int s = 0 ; s < numStates ; s ++ ){
        std::string junk;
        int videoIndexCurrent;
        float energyCurrent;
        float percentageStartCurrent;
        float percentageEndCurrent;
        
        (*fileIn) >> name;
        (*fileIn) >> videoIndexCurrent;
        (*fileIn) >> percentageStartCurrent;
        (*fileIn) >> percentageEndCurrent;
        (*fileIn) >> energyCurrent;
        addState( name , videoIndexCurrent , energyCurrent , percentageStartCurrent , percentageEndCurrent );
    }
    
    (*fileIn) >> junk;
    if( junk != "Transitions" ){
        cout << "* KafkaStatesMachineView  load: Bad tag numTransitions\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numTransitions;
    
    //crap tags
    (*fileIn) >> junk;
    if( junk != "StateName01" ){
        cout << "* KafkaStatesMachine  load: Bad tag StateName01\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "StateName02" ){
        cout << "* KafkaStatesMachine  load: Bad tag StateName02\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "Probability" ){
        cout << "* KafkaStatesMachine  load: Bad tag Probability\n";
        fileIn->close();
        return false;
    }

    
    for( int t = 0 ; t < numTransitions ; t ++ ){
        std::string stateNameInitial;
        std::string stateNameFinal;
        float probability;

        (*fileIn) >> stateNameInitial;
        (*fileIn) >> stateNameFinal;
        (*fileIn) >> probability;
        
        if( !addTransition( stateNameInitial , stateNameFinal, probability ) ){
            cout << "* KafkaStatesMachineView  load: couldnt add transition at load\n";
            fileIn->close();
            return false;
        }
    }
    fileIn->close();
    return true;
}
//-----------------------------------------------------------
void KafkaStatesMachineView::setActive( bool ifIsItActive ){
    isItActive = ifIsItActive;
}
//-----------------------------------------------------------
bool KafkaStatesMachineView::setCurrentState( string stateName ){
    int nexState = getStateIndex( stateName );
    if( nexState != -1 ){
        statePrevious = stateCurrent;
        stateCurrent = nexState;
        return true;
    }
    return false;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//private
//-------------------------------------------------------------
//-------------------------------------------------------------
void KafkaStatesMachineView::clear(){
    for( int s = 0 ; s < states.size() ; s ++ )
        delete states[s];
    states.clear();
}
//-------------------------------------------------------------
int KafkaStatesMachineView::getStateIndex( string name){
    for( int s = 0 ; s < statesNames.size() ; s ++ )
        if( name == statesNames[s] )
            return s;
    return -1;
}
