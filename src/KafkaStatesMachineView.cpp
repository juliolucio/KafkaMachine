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
    primitivesSpacign = 5;
    machineGap = 5;
    
    machineSize         = ofVec3f( ofGetWidth() - 2 * machineGap , ofGetHeight() - 2 * machineGap ,  ofGetHeight() - 2 * machineGap );
    machineOrigen       = ofVec3f( machineGap , machineGap , 0 );
    machineVideoSize    = ofVec3f( ( machineSize.x - (( numVideos - 1 ) * machineGap )) / numVideos , machineSize.y , ( machineSize.x - (( numVideos - 1 ) * machineGap )) / numVideos );
    machineCenter = ofVec3f( ofGetWidth() / 2 , ofGetHeight()/2, 0 );
    
    deltaBezierX = machineVideoSize.x;
    deltaBezierY = machineVideoSize.x;
    
    for( int videoIndex = 0 ; videoIndex < numVideos ; videoIndex ++ ){
        ofVec3f thisVideoPosition = machineOrigen  + ofVec3f( videoIndex * ( machineVideoSize.x + machineGap ) , 0 , 0 );;
        ofBoxPrimitive* tempBox = new ofBoxPrimitive();
        tempBox->setWidth( machineVideoSize.x );
        tempBox->setHeight( machineVideoSize.y );
        tempBox->setDepth( machineVideoSize.z );
        tempBox->setPosition( thisVideoPosition + ofVec3f( 0 , machineVideoSize.y / 2.0f , 0 ) );
        tempBox->setResolution( 1, 1, 1);
        videosBoxesPrimitives[ videoIndex ] = tempBox;
        machineVideosPositions.push_back( thisVideoPosition );
        videosColors.push_back( ofColor( ofRandom( 10 , 200 ) , ofRandom( 10 , 200 ), ofRandom( 100 , 200 ) ) );
    }
    
    type = MACHINE_VIEW_TYPE_SPHERES;
    //font.load( "Contl___.ttf" , 16 );
    
    //ofSetSphereResolution(3);
    
    //lights
//    ofSetSmoothLighting(true);
//    pointLight01.setDiffuseColor( ofFloatColor( .8 , .5 , .7 ) );
//    pointLight01.setSpecularColor( ofFloatColor( .5 , .5 , .5 ) );
//
//    pointLight02.setDiffuseColor( ofFloatColor( .6, .5, .8 ) );
//    pointLight02.setSpecularColor(ofFloatColor(.5 , .5 , .5 ) );
//    
//    pointLight03.setDiffuseColor( ofFloatColor( .8 , .8, .9 ) );
//    pointLight03.setSpecularColor( ofFloatColor( 1 , 1 , 1 ) );
    
//    pointLight01.enable();
//    pointLight02.enable();
//    pointLight03.enable();

    //material
    material.setShininess( 120 );
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    videoPlaneHorizontal.rotate( 90 , 1 , 0 , 0 );
}
//-------------------------------------------------------------
KafkaStatesMachineView::~KafkaStatesMachineView(){
    clear();
    delete fileIn;
}
//-------------------------------------------------------------
bool KafkaStatesMachineView::addState( string theName , int theVideoIndex ,  float thePercentageStart , float thePercentageEnd  ){
    float percentLength = thePercentageEnd - thePercentageStart;
    float statePrimitiveHeight = percentLength *  machineVideoSize.y;
    
    ofVec3f positionObjectInit = machineVideosPositions[ theVideoIndex ] + ofVec3f( 0 , thePercentageStart *  machineVideoSize.y , 0);
    ofVec3f positionPrimitive = positionObjectInit + ofVec3f( 0 , ( percentLength / 2 ) *  machineVideoSize.y , 0);
    ofVec3f positionObjectEnd = positionPrimitive + ofVec3f( 0 , ( percentLength / 2 ) *  machineVideoSize.y , 0);
    
    map<string,float> tempMap;
    
    statesNames             .push_back( theName );
    statesQuantizations     .push_back( tempMap );
    statesVideoIndexes      .push_back( theVideoIndex );
    statesPercentageStarts  .push_back( thePercentageStart );
    statesPercentageEnds    .push_back( thePercentageEnd );
    statesPositionsCenters  .push_back( positionPrimitive );
    statesPositionsInits    .push_back( positionObjectInit );
    statesPositionsEnds     .push_back( positionObjectEnd );
    
    of3dPrimitive* primitive01;
    of3dPrimitive* primitive02;
    vector<ofMeshFace> triangles;
    
    switch ( type ) {
        case MACHINE_VIEW_TYPE_BOXES:{
            ofBoxPrimitive* box = new ofBoxPrimitive();
            box->setWidth( machineVideoSize.x - primitivesSpacign );
            box->setHeight( statePrimitiveHeight - primitivesSpacign );
            box->setDepth( machineVideoSize.z - primitivesSpacign );
            box->setPosition( positionPrimitive );
            box->setResolution( 1,1,1 );
            box->setMode( OF_PRIMITIVE_TRIANGLES );
            ofBoxPrimitive* box2 = new ofBoxPrimitive();
            box2->setWidth( machineVideoSize.x  );
            box2->setHeight( statePrimitiveHeight - primitivesSpacign );
            box2->setDepth( machineVideoSize.z );
            box2->setPosition( positionPrimitive );
            box2->setResolution( 1,1,1 );
            
            primitive01 = box;
            primitive02 = box;
        }
            break;
            
            
        case MACHINE_VIEW_TYPE_SPHERES:{
            float scale = ( statePrimitiveHeight - primitivesSpacign ) / ( machineVideoSize.x - primitivesSpacign );
            ofIcoSpherePrimitive* sphere = new ofIcoSpherePrimitive();
            sphere->setRadius(  ( machineVideoSize.x - primitivesSpacign ) / 2  );
            sphere->setPosition( positionPrimitive - primitivesSpacign );
            sphere->setResolution( 2 );
            sphere->setMode( OF_PRIMITIVE_TRIANGLES );
            sphere->setScale( 1 , scale , 1 );
            
            ofIcoSpherePrimitive* sphere2 = new ofIcoSpherePrimitive();
            sphere2->setRadius( ( machineVideoSize.x - primitivesSpacign ) / 2 );
            sphere2->setPosition( positionPrimitive - primitivesSpacign );
            sphere2->setResolution( 2 );
            sphere2->setScale( 1 , scale , 1 );
            primitive01 = sphere;
            primitive02 = sphere2;
        }
            break;
            

            
            
        case MACHINE_VIEW_TYPE_PLANES:{
            ofPlanePrimitive* plane = new ofPlanePrimitive();
            plane->setWidth( machineVideoSize.x - primitivesSpacign );
            plane->setHeight( statePrimitiveHeight - primitivesSpacign );
            plane->setPosition( positionPrimitive );
            plane->setMode( OF_PRIMITIVE_TRIANGLES );
            ofPlanePrimitive* plane2 = new ofPlanePrimitive();
            plane2->setWidth( machineVideoSize.x );
            plane2->setHeight( statePrimitiveHeight );
            plane2->setPosition( positionPrimitive );
            primitive01 = plane;
            primitive02 = plane2;
        }
            break;
            
        default:
            cout << " ERROR @@@ :bad machine view type\n";
            return;
    }
    triangles = primitive01->getMesh().getUniqueFaces();
    statePrimitiveTriangles.insert(pair<int,vector<ofMeshFace>>(statePrimitive01.size(),triangles));
    statePrimitive01.insert(pair<int,of3dPrimitive*>(statePrimitive01.size(),primitive01));
    statePrimitive02.insert(pair<int,of3dPrimitive*>(statePrimitive02.size(),primitive02));
    
    if( stateCurrent == -1 )
        stateCurrent = 0;
    return true;
}
//-------------------------------------------------------------
bool KafkaStatesMachineView::addTransition( string nameState01 , string nameState02 , float theProbability ){
    ofVec3f initTransitionPosition = statesPositionsEnds[ getStateIndex( nameState01 ) ];
    ofVec3f endTransitionPosition = statesPositionsInits[ getStateIndex( nameState02 ) ];
    ofVec3f distVector = endTransitionPosition - initTransitionPosition;
    
    int initVideoIndex = statesVideoIndexes[ getStateIndex( nameState01 )];
    int endVideoIndex = statesVideoIndexes[ getStateIndex( nameState02 )];
    
    ofVec3f centerTransitionPosition;
    ofVec3f transitionPositionInitsCenter01;
    ofVec3f transitionPositionInitsCenter02;
    ofVec3f transitionPositionCentersEnd01;
    ofVec3f transitionPositionCentersEnd02;
    
    
    if( initVideoIndex < endVideoIndex ){
        centerTransitionPosition =  initTransitionPosition + distVector * .5;
        
        transitionPositionInitsCenter01 = initTransitionPosition + ofVec3f( 0 , deltaBezierY , 0 );
        transitionPositionInitsCenter02 = transitionPositionInitsCenter01 + ofVec3f( deltaBezierX , 0 , 0 );
        
        transitionPositionCentersEnd02 = endTransitionPosition + ofVec3f( 0 , -deltaBezierY  , 0 );
        transitionPositionCentersEnd01 = transitionPositionCentersEnd02 + ofVec3f( -deltaBezierX , 0 , 0 );
    }
    else if( initVideoIndex > endVideoIndex ){
        centerTransitionPosition =  initTransitionPosition + distVector * .5;
        
        transitionPositionInitsCenter01 = initTransitionPosition + ofVec3f( 0 , deltaBezierY , 0 );
        transitionPositionInitsCenter02 = transitionPositionInitsCenter01 + ofVec3f( -deltaBezierX , 0 , 0 );
        
        transitionPositionCentersEnd02 = endTransitionPosition + ofVec3f( 0 , -deltaBezierY  , 0 );
        transitionPositionCentersEnd01 = transitionPositionCentersEnd02 + ofVec3f( deltaBezierX , 0 , 0 );
    }
    else {
        centerTransitionPosition =  initTransitionPosition + distVector * .5 + ofVec3f( -deltaBezierX , 0 ,0 );
        
        transitionPositionInitsCenter01 = initTransitionPosition + ofVec3f( 0 , deltaBezierY , 0 );
        transitionPositionInitsCenter02 = transitionPositionInitsCenter01 + ofVec3f( -deltaBezierX , 0 , 0 );
        
        transitionPositionCentersEnd02 = endTransitionPosition + ofVec3f( 0 , -deltaBezierY  , 0 );
        transitionPositionCentersEnd01 = transitionPositionCentersEnd02 + ofVec3f(-deltaBezierX , 0 , 0 );
    }
    
    transitionStateNameInitial  .push_back( nameState01 );
    transitionStateNameFinal    .push_back( nameState02 );
    transitionStateProbabilities.push_back( theProbability );
    transitionsColors           .push_back( ofColor( 0 ,ofRandom( 100 , 250  ) , 0 ) );
    transitionsPositionsCenters .push_back( centerTransitionPosition );
    transitionsPositionsInits   .push_back( initTransitionPosition );
    transitionsPositionsEnds    .push_back( endTransitionPosition );
    transitionsPositionsInitsCenters01.push_back( transitionPositionInitsCenter01 );
    transitionsPositionsInitsCenters02.push_back( transitionPositionInitsCenter02 );
    transitionsPositionsCentersEnds01.push_back( transitionPositionCentersEnd01 );
    transitionsPositionsCentersEnds02.push_back( transitionPositionCentersEnd02 );
    
    return true;
}

//-------------------------------------------------------------
void KafkaStatesMachineView::update(){
    positionOrigin = ofVec3f( - ofGetWidth() / 1.7 , - ofGetHeight() / 1.6 , 0 );

    videoPlanePosition = machineVideosPositions[ activeVideoIndex ] + ofVec3f( 0 , currentVideo->getPosition() *  machineVideoSize.y ,  0  );
    
//    positionPointLight01.x = machineCenter.x + cos( .5 * ofGetElapsedTimef() ) * machineSize.x;
//    positionPointLight01.y = machineCenter.y;
//    positionPointLight01.z = machineCenter.z + sin( .5 * ofGetElapsedTimef() ) * machineSize.x;
//    
//    positionPointLight02.x = machineCenter.x + cos( PI + .5 * ofGetElapsedTimef() ) * machineSize.x;
//    positionPointLight02.y = machineCenter.y;
//    positionPointLight02.z = machineCenter.z + sin( PI + .5 * ofGetElapsedTimef() ) * machineSize.x;
//    
//    positionPointLight03.x = machineCenter.x;
//    positionPointLight03.y = machineCenter.y + cos( .2 * ofGetElapsedTimef() ) * machineSize.x;
//    positionPointLight03.z = machineCenter.z + sin( .2 * ofGetElapsedTimef() ) * 2 * machineSize.x;
//    
//    pointLight01.setPosition( positionPointLight01 );
//    pointLight02.setPosition( positionPointLight02 );
//    pointLight03.setPosition( positionPointLight03 );
    
    videoPlaneHorizontal.setWidth( machineVideoSize.x + 20 );
    videoPlaneHorizontal.setHeight( machineVideoSize.x + 30 );
    videoPlaneHorizontal.setPosition( videoPlanePosition );

}
//-------------------------------------------------------------
void KafkaStatesMachineView::draw(){
    if( !isItActive )
        return;
    
    ofPushMatrix();
    ofTranslate( positionOrigin );
    ofEnableDepthTest();
    
    //currentVideo->draw( videoPlanePosition.x +  ( ( machineVideoSize.x + 10 ) * 1.33 ) / 2 , videoPlanePosition.y - ( machineVideoSize.x + 10  ) / 2 , ( machineVideoSize.x + 10 ) * 1.33 , machineVideoSize.x + 10 );
    
    ofEnableLighting();
    
    material.begin();
    videoPlaneHorizontal.draw();
    material.end();
    
    map< int,of3dPrimitive* >::iterator it01 = statePrimitive01.begin();
    map< int,of3dPrimitive* >::iterator it02 = statePrimitive01.begin();
    map<int,vector<ofMeshFace>>::iterator ittr = statePrimitiveTriangles.begin();
    
    int stateIndex = 0;
    while( it01 != statePrimitive01.end() ){
        of3dPrimitive* tempPrimitive01 = (*it01).second;
        if( stateIndex == stateCurrent ){
            vector<ofMeshFace> triangles = (*ittr).second;;
            float angle = ofGetElapsedTimef() * 5;
            float strength = (sin( angle+.25 )) * 5 ;
            ofVec3f faceNormal;
            for( int i = 0; i < triangles.size(); i++ ) {
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    triangles[i].setVertex( j, triangles[i].getVertex(j) + faceNormal * strength);
                }
            }
            tempPrimitive01->getMesh().setFromTriangles( triangles );
        }
        
        of3dPrimitive* tempPrimitive02 = (*it02).second;
        ofSetColor( 255 );
        material.begin();
        tempPrimitive01->draw();
        material.end();
        tempPrimitive02->drawWireframe();
        
//        ofDisableLighting();
//        ofSetColor( 111,11,111);
//        ofPoint textPosition = statesPositionsCenters[ stateIndex ];
//        string textState = statesNames[ (*it01).first ] ;
//        ofDrawBitmapString( textState , textPosition );
//        
//        ofSetColor( 40 , 40 , 200  );
//        textPosition = statesPositionsInits[ stateIndex ];
//        textState = ofToString( statesPercentageStarts[ (*it01).first ] , 2 );
//        ofDrawBitmapString( textState , textPosition );
//        
//        ofSetColor( 200 , 40 , 40  );
//        textPosition = statesPositionsEnds[ stateIndex ];
//        textState = ofToString( statesPercentageEnds[ (*it01).first ] , 2 );
//        ofDrawBitmapString( textState , textPosition );
//        
//        ofEnableLighting();
        
        it01++;
        it02++;
        ittr++;
        stateIndex++;
    }
    
    int transitionIndex = 0;
    int numTransitions = transitionStateNameInitial.size();
    
    for( int t = 0 ; t < numTransitions ; t ++ ){
        
        //        ofDisableLighting();
        
        ofSetColor( 200 , 40 , 40  );
        ofDrawSphere(transitionsPositionsInits[ t ].x , transitionsPositionsInits[ t ].y , transitionsPositionsInits[ t ].z , machineVideoSize.x / 12);
        
        ofNoFill();
        ofDrawBezier(transitionsPositionsInits[ t ].x ,
                     transitionsPositionsInits[ t ].y ,
                     transitionsPositionsInits[ t ].z ,
                     transitionsPositionsInitsCenters01[ t ].x ,
                     transitionsPositionsInitsCenters01[ t ].y ,
                     transitionsPositionsInitsCenters01[ t ].z ,
                     transitionsPositionsInitsCenters02[ t ].x ,
                     transitionsPositionsInitsCenters02[ t ].y ,
                     transitionsPositionsInitsCenters02[ t ].z ,
                     transitionsPositionsCenters[ t ].x ,
                     transitionsPositionsCenters[ t ].y ,
                     transitionsPositionsCenters[ t ].z  );
        
        ofSetColor( ofColor( 20 , ofMap( transitionStateProbabilities[ t ] , 0 , 1 , 120 , 255 ) , 20 ) );
        ofDrawSphere(transitionsPositionsCenters[ t ].x , transitionsPositionsCenters[ t ].y , transitionsPositionsCenters[ t ].z , ofMap( transitionStateProbabilities[ t ] , 0 , 1 , 1 , machineVideoSize.x / 7 ) );
        
        ofSetColor( 40 , 40 , 200  );
        ofDrawSphere(transitionsPositionsEnds[ t ].x , transitionsPositionsEnds[ t ].y , transitionsPositionsEnds[ t ].z  , machineVideoSize.x / 12 );
        ofDrawBezier(transitionsPositionsCenters[ t ].x ,
                     transitionsPositionsCenters[ t ].y ,
                     transitionsPositionsCenters[ t ].z ,
                     transitionsPositionsCentersEnds01[ t ].x ,
                     transitionsPositionsCentersEnds01[ t ].y ,
                     transitionsPositionsCentersEnds01[ t ].z ,
                     transitionsPositionsCentersEnds02[ t ].x ,
                     transitionsPositionsCentersEnds02[ t ].y  ,
                     transitionsPositionsCentersEnds02[ t ].z ,
                     transitionsPositionsEnds[ t ].x ,
                     transitionsPositionsEnds[ t ].y ,
                     transitionsPositionsEnds[ t ].z  );
        
        
        //        ofSetColor( ofColor( 20 , ofMap( transitionStateProbabilities[ t ] , 0 , 1 , 120 , 255 ) , 20 )   );
        //        ofDrawSphere(transitionsPositionsInitsCenters01[ t ].x , transitionsPositionsInitsCenters01[ t ].y , transitionsPositionsInitsCenters01[ t ].z , machineVideoSize.x / 20);
        //
        //        ofSetColor( ofColor( 20 , ofMap( transitionStateProbabilities[ t ] , 0 , 1 , 120 , 255 ) , 20 )   );
        //        ofDrawSphere(transitionsPositionsInitsCenters02[ t ].x , transitionsPositionsInitsCenters02[ t ].y , transitionsPositionsInitsCenters02[ t ].z , machineVideoSize.x / 20);
        //
        //        ofSetColor( ofColor( 20 , ofMap( transitionStateProbabilities[ t ] , 0 , 1 , 120 , 255 ) , 20 )   );
        //        ofDrawSphere(transitionsPositionsCentersEnds01[ t ].x , transitionsPositionsCentersEnds01[ t ].y , transitionsPositionsCentersEnds01[ t ].z , machineVideoSize.x / 20);
        //
        //        ofSetColor( ofColor( 20 , ofMap( transitionStateProbabilities[ t ] , 0 , 1 , 120 , 255 ) , 20 )   );
        //        ofDrawSphere(transitionsPositionsCentersEnds02[ t ].x , transitionsPositionsCentersEnds02[ t ].y , transitionsPositionsCentersEnds02[ t ].z , machineVideoSize.x / 20);
        //
        //
        ofDisableLighting();
        ofSetColor( 10 , 50 , 10  );
        ofDrawBitmapString( ofToString( transitionStateProbabilities[ transitionIndex ]), transitionsPositionsCenters[ t ] );
        ofEnableLighting();
        transitionIndex++;
    }
    
    for( int videoIndex = 0 ; videoIndex < numVideos ; videoIndex ++ ){
        ofSetColor(videosColors[ videoIndex ]);
        //videosBoxesPrimitives[videoIndex]->drawWireframe();
        ofPoint textPosition = machineVideosPositions[videoIndex];
        string textState = " video #" + ofToString( videoIndex );
        textPosition.x -= machineVideoSize.x / 2;
        textPosition.y -= machineVideoSize.x / 2;
        ofDrawBitmapString( textState , textPosition );
    }
    ofPopMatrix();

    ofDisableLighting();
    ofDisableDepthTest();
    ofSetColor( 255 );
}
//----------------------------------------------------------------------------------
bool KafkaStatesMachineView::loadFromMachine( KafkaStatesMachine* machineRefernece ){
    for( int s = 0 ; s < machineRefernece->getNumStates() ; s ++ )
        addState( machineRefernece->getState( s )->getName() , machineRefernece->getState( s )->getVideoIndex(),  machineRefernece->getState( s )->getStart(), machineRefernece->getState( s )->getEnd() );
    
    for( int t = 0 ; t < machineRefernece->getNumTransition() ; t ++ )
        addTransition(  machineRefernece->getTransition( t )->getNameStateInitial() , machineRefernece->getTransition( t )->getNameStateFinal() , machineRefernece->getTransition( t )->getProbability() );
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
        
        addState( name , videoIndexCurrent , percentageStartCurrent , percentageEndCurrent );
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
    if( junk != "TotalFrames" ){
        cout << "* KafkaStatesMachine  load: Bad tag Energy\n";
        fileIn->close();
        return false;
    }
    
    for( int s = 0 ; s < numStates ; s ++ ){
        std::string junk;
        int videoIndexCurrent;
        long numFramesTotal;
        float percentageStartCurrent;
        float percentageEndCurrent;
        long frameStartCurrent;
        long frameEndCurrent;
        
        (*fileIn) >> name;
        (*fileIn) >> videoIndexCurrent;
        (*fileIn) >> frameStartCurrent;
        (*fileIn) >> frameEndCurrent;
        (*fileIn) >> numFramesTotal;
        
        percentageStartCurrent = float(frameStartCurrent) / float(numFramesTotal);
        percentageEndCurrent = float(frameEndCurrent) / float(numFramesTotal);
        
        addState( name , videoIndexCurrent , percentageStartCurrent , percentageEndCurrent );
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
//-----------------------------------------------------------
void KafkaStatesMachineView::updateViewDataVideo( int theActiveVideoIndex , ofVideoPlayer* theCurrentVideo ){
    activeVideoIndex = theActiveVideoIndex;
    currentVideo = theCurrentVideo;
    textureFromVideo = theCurrentVideo->getTexture();
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//private
//-------------------------------------------------------------
//-------------------------------------------------------------
void KafkaStatesMachineView::clear(){
    for( int s = 0 ; s < statePrimitive01.size() ; s ++ )
        delete statePrimitive01[s];
    statePrimitive01.clear();
    
    for( int v = 0 ; v < videosBoxesPrimitives.size() ; v ++ )
        delete videosBoxesPrimitives[v];
    videosBoxesPrimitives.clear();
    
    for( int s = 0 ; s < statePrimitive02.size() ; s ++ )
        delete statePrimitive02[s];
    statePrimitive02.clear();
    
    
    //todo:Clear all object memory
    //videosBoxesPrimitives
}
//-------------------------------------------------------------
int KafkaStatesMachineView::getStateIndex( string name){
    for( int s = 0 ; s < statesNames.size() ; s ++ )
        if( name == statesNames[s] )
            return s;
    return -1;
}

