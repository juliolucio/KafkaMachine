//
//  KafkaStatesMachine.cpp
//  KafkaMachine
//
//  Created by Julio Lucio on 10/April/2016.
//
//
#include "KafkaStatesMachine.h"
#include "KafkaStatesMachineState.h"
#include "KafkaStatesMachineTransition.h"
//-------------------------------------------------------------
KafkaStatesMachine::KafkaStatesMachine( string theName ){
    name = theName;
    currentState = 0;
    fileIn = 0;
    fileOut = 0;
    isItActive = true;
    hasJustChangedState = false;
}
//-------------------------------------------------------------
KafkaStatesMachine::~KafkaStatesMachine(){
    clear();
}
//-------------------------------------------------------------
bool KafkaStatesMachine::addState( string theName , int theVideoIndex , vector<float> theParameters ){
    for( int s = 0 ; s < states.size() ; s ++ )
        if( states[ s ]->getName() == theName ){
            cout << "State name already exist... \n";
            return false;
        }
    KafkaStatesMachineState* newState = new KafkaStatesMachineState( theName , theVideoIndex ,  theParameters );
    states.push_back( newState );
    if( !currentState )
        currentState = states[0];
    return true;
}
//-------------------------------------------------------------
bool KafkaStatesMachine::addTransition( string nameState01 , string nameState02 , float theProbability ){
    for( int t = 0 ; t < transitions.size() ; t ++ )
        if( transitions[ t ]->getNameStateInitial() == nameState01 && transitions[ t ]->getNameStateFinal() == nameState02 ){
            cout << "Transition already defined ... \n";
            return false;
        }
    
    KafkaStatesMachineState* tempInitialState = 0;
    KafkaStatesMachineState* tempFinalState = 0;
    for( int s = 0 ; s < states.size() ; s ++ ){
        if( states[ s ]->getName() == nameState01 )
            tempInitialState = states[ s ];
        if( states[ s ]->getName() == nameState02 )
            tempFinalState = states[ s ];
    }
    if( !tempInitialState || !tempFinalState ){
        cout << "Missing state in trasition ... \n";
        return false;
    }
    KafkaStatesMachineTransition* newTransition = new KafkaStatesMachineTransition( tempInitialState , tempFinalState , theProbability );
    transitions.push_back( newTransition );
    return true;
}

//-------------------------------------------------------------
bool KafkaStatesMachine::step(){
    float dice = ofRandom( 0 , 1 );
    vector<KafkaStatesMachineTransition*> posibleTransitions;
    vector<float> posibleTransitionsProbabilities;
    int t;
    for( t = 0 ; t < transitions.size() ; t ++ ){
        if( transitions[t]->getNameStateInitial() == currentState->getName() ){
            posibleTransitions.push_back( transitions[ t ] );
            posibleTransitionsProbabilities.push_back( transitions[ t]->getProbability() );
        }
    }
    
    if( !posibleTransitions.size() ){
        cout << "Machine stuck for steps";
        return false;
    }
    
    else{
        float total = 0;
        for( t = 0 ; t < posibleTransitionsProbabilities.size() ;t ++ )
            total += posibleTransitionsProbabilities[ t ];
        
        for( t = 0 ; t < posibleTransitionsProbabilities.size() ;t ++ )
           posibleTransitionsProbabilities[ t ] /= total;
        
        float valuerReached = 0;
        for( t = 0 ; t < posibleTransitionsProbabilities.size() ;t ++ ){
            valuerReached += posibleTransitionsProbabilities[ t ];
            if( valuerReached >= dice ){
                setCurrentState( posibleTransitions[ t ]->getStateFinal() );
                return true;
            }
        }
        if( posibleTransitions.size() ){
            t--;
            setCurrentState( posibleTransitions[ t ]->getStateFinal());
            return true;
        }
    }
}
//-------------------------------------------------------------
bool KafkaStatesMachine::stepRandom(){
    float dice = ofRandom( 0 , states.size() );
    setCurrentState( states[ dice] );
    return true;
}
//-------------------------------------------------------------
bool KafkaStatesMachine::stepEnergys(vector<float> theEnergys){
    if( theEnergys.size() != 3 )
        return false;
    
    float dice = ofRandom( 0 , 1 );
    float energy01 = theEnergys[0];
    float energy02 = theEnergys[1];
    float energy03 = theEnergys[2];
    
    vector<float> nextStatesProbabilities;
    float totalProbability = 0;
    int s;
    for( s = 0 ; s < states.size() ; s ++ ){
        float probability = energy01 * states[ s ]->getEnergy01() + energy02 * states[ s ]->getEnergy02() + energy03 * states[ s ]->getEnergy03();
        totalProbability += probability;
        nextStatesProbabilities.push_back(( probability ) ) ;
    }
    
    for( s = 0 ; s < states.size() ; s ++ )
        nextStatesProbabilities[ s ] /= totalProbability;

    float valuerReached = 0;
    for( s = 0 ; s < states.size() ; s ++ ){
        valuerReached += nextStatesProbabilities[ s ];
        if( valuerReached >= dice ){
            setCurrentState( states[ s ] );
            return true;
        }
    }
    setCurrentState( states[ s-1 ] );
    return true;
}
//-------------------------------------------------------------
void KafkaStatesMachine::updateStates( float theEnergy ){
    cout << "ERROROROROR:::NOT TO BE HERE JET\n";
    float dice = ofRandom( 0 , 1 );
    map<float,KafkaStatesMachineTransition*> posibleTransitions;
    map<float,KafkaStatesMachineTransition*>::iterator posibleTransitionsIterator;
    for( int t = 0 ; t < transitions.size() ; t ++ ){
        if( transitions[t]->getNameStateInitial() == currentState->getName() ){
            pair<float,KafkaStatesMachineTransition*> newPosibleTransition;
            float energuiFinal = transitions[t]->getStateFinal()->getEnergy01();
            float energyInitial = transitions[t]->getStateInitial()->getEnergy01();
            
            if( theEnergy > 0.65 ){
                if( energuiFinal > energyInitial ){
                    newPosibleTransition.first = transitions[t]->getProbability();
                    newPosibleTransition.second = transitions[t];
                    posibleTransitions.insert(newPosibleTransition);
                }
            }
            
            else if( theEnergy < 0.35 ){
                if( energuiFinal < energyInitial ){
                    newPosibleTransition.first = transitions[t]->getProbability();
                    newPosibleTransition.second = transitions[t];
                    posibleTransitions.insert(newPosibleTransition);
                }
            }
            else{
                newPosibleTransition.first = transitions[t]->getProbability();
                newPosibleTransition.second = transitions[t];
                posibleTransitions.insert(newPosibleTransition);
            }
        }
    }
    
    if( !posibleTransitions.size() ){
        //todo///currentState->sound.play();
    }
    
    else{
        //normalizing probabilities
        float total = 0;
        for( posibleTransitionsIterator = posibleTransitions.begin() ; posibleTransitionsIterator != posibleTransitions.end() ;posibleTransitionsIterator ++ ){
            pair<float,KafkaStatesMachineTransition*> tempPosibleTransition = (*posibleTransitionsIterator);
            total += tempPosibleTransition.first;
        }
        
        for( posibleTransitionsIterator = posibleTransitions.begin() ; posibleTransitionsIterator != posibleTransitions.end() ;posibleTransitionsIterator ++ ){
            pair<float,KafkaStatesMachineTransition*> tempPosibleTransition = (*posibleTransitionsIterator);
            tempPosibleTransition.first /= total;
        }
        float valuerReached = 0;
        for(posibleTransitionsIterator = posibleTransitions.begin() ; posibleTransitionsIterator != posibleTransitions.end() ;posibleTransitionsIterator ++ ){
            pair<float,KafkaStatesMachineTransition*> tempPosibleTransition = (*posibleTransitionsIterator);
            valuerReached += tempPosibleTransition.first;
            if( valuerReached >= dice ){
                setCurrentState( tempPosibleTransition.second->getStateFinal() );
                return;
            }
        }
        if( posibleTransitions.size() ){
            posibleTransitionsIterator--;
            setCurrentState( (*posibleTransitionsIterator).second->getStateFinal() );
            return;
        }
    }
}
//-------------------------------------------------------------
void KafkaStatesMachine::draw(){
}
//-------------------------------------------------------------
void KafkaStatesMachine::clear(){
    for( int s = 0 ; s < states.size() ; s ++ )
        delete states[s];
    states.clear();
    
    for( int t = 0 ; t < transitions.size() ; t ++ )
        delete transitions[t];
    transitions.clear();
}
///----------------------------------------------------------------------------------
bool KafkaStatesMachine::load( string fileName ){
    if( !fileIn )
        fileIn = new ifstream();
    
    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
    if ( !fileIn->is_open() ){
        cout << "Machine File not found: ";
        cout << fileName << "\n";
        fileIn->close();
        return false;
    }
    
    clear();
    
    std::string junk;
    int numStates;
    int numTransitions;
    
    (*fileIn) >> junk;
    if( junk != "numStates=" ){
        cout << "* KafkaStatesMachine  load: Bad tag numStates\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numStates;
    
    (*fileIn) >> junk;
    if( junk != "numTransitions=" ){
        cout << "* KafkaStatesMachine  load: Bad tag numTransitions\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numTransitions;
    
    for( int s = 0 ; s < numStates ; s ++ ){
        KafkaStatesMachineState* newState = new KafkaStatesMachineState();
        if( !newState->load(fileIn) ){
            cout << "* KafkaStatesMachine  load: Bad tag numTransitions\n";
            fileIn->close();
            return false;
        }
        states.push_back(newState);
        if( !currentState )
            currentState = states[0];
    }
    
    for( int t = 0 ; t < numTransitions ; t ++ ){
        (*fileIn) >> junk;
        std::string stateNameInitial;
        if( junk != "stateNameInitial=" ){
            cout << "* KafkaStatesMachine  load: Bad tag stateNameInitial\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameInitial;
        
        (*fileIn) >> junk;
        std::string stateNameFinal;
        if( junk != "stateNameFinal=" ){
            cout << "* KafkaStatesMachine  load: Bad tag stateNameFinal\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameFinal;
        
        (*fileIn) >> junk;
        float probability;
        if( junk != "probability=" ){
            cout << "* KafkaStatesMachine  load: Bad tag probability\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> probability;
        
        if( !addTransition( stateNameInitial , stateNameFinal, probability ) ){
            cout << "* KafkaStatesMachine  load: couldnt add transition at load\n";
            fileIn->close();
            return false;
        }
    }
    fileIn->close();
    return true;
}
//-----------------------------------------------------------
bool KafkaStatesMachine::loadFromTSV( string fileName ){
    if( !fileIn )
        fileIn = new ifstream();
    
    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
    if ( !fileIn->is_open() ){
        cout << "Machine File not found: ";
        cout << fileName << "\n";
        fileIn->close();
        return false;
    }
    
    clear();
    std::string junk;
    int numStates;
    long numFrames;
    
    (*fileIn) >> junk;
    if( junk != "States" ){
        cout << "* KafkaStatesMachine  load: Bad tag States\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numStates;
    
    (*fileIn) >> junk;
    if( junk != "Frames" ){
        cout << "* KafkaStatesMachine  load: Bad tag States\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numFrames;
    
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
        cout << "* KafkaStatesMachine  load: Bad tag Statr\n";
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
        KafkaStatesMachineState* newState = new KafkaStatesMachineState();
        if( !newState->loadFromTSV( fileIn , numFrames ) ){
            cout << "* KafkaStatesMachine  load: Bad tag numTransitions\n";
            fileIn->close();
            return false;
        }
        states.push_back(newState);
        if( !currentState )
            currentState = states[0];
    }
    
    int numTransitions;
    (*fileIn) >> junk;
    if( junk != "Transitions" ){
        cout << "* KafkaStatesMachine  load: Bad tag numTransitions\n";
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
        float probability;
        std::string stateNameInitial;
        std::string stateNameFinal;
        
        (*fileIn) >> stateNameInitial;
        (*fileIn) >> stateNameFinal;
        (*fileIn) >> probability;
        
        if( !addTransition( stateNameInitial , stateNameFinal, probability ) ){
            cout << "* KafkaStatesMachine  load: couldnt add transition at load\n";
            fileIn->close();
            return false;
        }
    }
    fileIn->close();
    return true;
}
//-----------------------------------------------------------
bool KafkaStatesMachine::addStatesFormFile( string fileName , int videoIndex ){
    if( !fileIn )
        fileIn = new ifstream();
    
    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
    if ( !fileIn->is_open() ){
        cout << "Machine File not found: ";
        cout << fileName << "\n";
        fileIn->close();
        return false;
    }
    
    std::string junk;
    int numStates;
    long numFrames;
    
    (*fileIn) >> junk;
    if( junk != "FramesTotal" ){
        cout << "* KafkaStatesMachine  add states : Bad tag FramesTotal\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numFrames;
    
    (*fileIn) >> junk;
    if( junk != "NumCuts" ){
        cout << "* KafkaStatesMachine  add states: Bad tag NumCuts\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numStates;
    
    //crap tags
    (*fileIn) >> junk;
    if( junk != "CutEnds" ){
        cout << "* KafkaStatesMachine  add states: Bad tag CutEnds\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "Energy01" ){
        cout << "* KafkaStatesMachine  add states: Bad tag Energy01\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "Energy02" ){
        cout << "* KafkaStatesMachine  add states: Bad tag Energy02\n";
        fileIn->close();
        return false;
    }
    
    
    (*fileIn) >> junk;
    if( junk != "Energy03" ){
        cout << "* KafkaStatesMachine  add states: Bad tag Energy03\n";
        fileIn->close();
        return false;
    }
    
    float frameEnd;
    for( int s = 0 ; s < numStates ; s ++ ){
        float frameStart;
        float energy01;
        float energy03;
        float energy02;
        vector<float> params;

        if( s == 0 ){
            frameStart = 0;
            (*fileIn) >> frameEnd;
        }

        else{
            frameStart = frameEnd + 1;
            (*fileIn) >> frameEnd;
        }
        
        (*fileIn) >> energy01;
        (*fileIn) >> energy02;
        (*fileIn) >> energy03;
        
        params.push_back( numFrames );
        params.push_back( frameStart + 1 );
        params.push_back( frameEnd - 2 );//what the hack???
        params.push_back( energy01 );
        params.push_back( energy02 );
        params.push_back( energy03 );
        
        string stateName = "STATE_VIDEO_" + ofToString( videoIndex ) + "_CUT_" + ofToString( states.size() );
        
        KafkaStatesMachineState* newState = new KafkaStatesMachineState( stateName , videoIndex , params );
        states.push_back(newState);
        if( !currentState )
            currentState = states[0];
    }
    fileIn->close();
    return true;
}
//-----------------------------------------------------------
//void KafkaStatesMachine::fullPopulateTransitions(){
//    float equalTransitionProbability = 1.0f / float(states.size());
//    for( int sx = 0 ; sx < states.size() ; sx ++ )
//        for( int sy = 0 ; sy < states.size() ; sy ++ ){
//            addTransition( states[sx]->getName() , states[sy]->getName(), equalTransitionProbability );
//            cout << "Transition " << ofToString( transitions.size() ) << "\n";
//        }
//}
//-----------------------------------------------------------
bool KafkaStatesMachine::save( string fileName ){
    if( !fileOut )
        fileOut = new ofstream();
    fileOut->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::out );
    if ( !fileOut->is_open() ){
        cout << "Couldt create Machien file: ";
        cout << fileName << "\n";
        fileOut->close();
        return false;
    }
    (*fileOut) << "numStates= ";
    (*fileOut) << states.size();
    (*fileOut) << "\n";
    
    (*fileOut) << "numTransitions= ";
    (*fileOut) << transitions.size();
    (*fileOut) << "\n";
    
    for( int s = 0 ; s < states.size() ; s ++ ){
        if( !states[s]->save(fileOut) ){
            cout << "* KafkaStatesMachine  save: couldnt save state\n";
            fileOut->close();
            return false;
        }
    }
    
    for( int t = 0 ; t < transitions.size() ; t ++ ){
        if( !transitions[t]->save(fileOut) ){
            cout << "* KafkaStatesMachine  save transition: couldnt save transition" << t << "\n";
            fileOut->close();
            return false;
        }
    }
    fileOut->close();
    return  true;
}
//-----------------------------------------------------------
string KafkaStatesMachine::getName(){
    return name;
}
//-----------------------------------------------------------
string KafkaStatesMachine::getCurrentStateName(){
    return currentState->getName();
}
//-----------------------------------------------------------
int KafkaStatesMachine::getCurrentStateVideoIndex(){
    return currentState->getVideoIndex();
}
//-----------------------------------------------------------
float KafkaStatesMachine::getCurrentStateStart(){
    return currentState->getStart();
}
//-----------------------------------------------------------
float KafkaStatesMachine::getCurrentStateEnd(){
    return currentState->getEnd();
}
//-----------------------------------------------------------
float KafkaStatesMachine::getCurrentStateEnergy(){
    return currentState->getEnergy01();
}
//-----------------------------------------------------------
//-----------------------------------------------------------
void KafkaStatesMachine::setActive( bool ifIsItActive ){
    isItActive = ifIsItActive;
}
//-----------------------------------------------------------
bool KafkaStatesMachine::isAtcive(){
    return  isItActive;
}
//-----------------------------------------------------------
bool KafkaStatesMachine::justChangedState(){
    if( hasJustChangedState ){
        hasJustChangedState = false;
        return true;
    }
    return false;
}
//-----------------------------------------------------------
int KafkaStatesMachine::getNumStates(){
    return states.size();
}
//-----------------------------------------------------------
int KafkaStatesMachine::getNumTransition(){
    return transitions.size();
}
//-----------------------------------------------------------
KafkaStatesMachineState* KafkaStatesMachine::getState( int stateIndex ){
    if( stateIndex < states.size() )
        return states[ stateIndex ];
    else return NULL;
}
//-----------------------------------------------------------
KafkaStatesMachineTransition* KafkaStatesMachine::getTransition( int transitionIndex ){
    if( transitionIndex < transitions.size() )
        return transitions[ transitionIndex ];
    else return NULL;
}
//-----------------------------------------------------------
void KafkaStatesMachine::setCurrentState( KafkaStatesMachineState* newState ){
    cout << "Machine : " << name << "  form " << currentState->getName();
    currentState = newState;
    cout << " to " << currentState->getName() << "\n";
    hasJustChangedState = true;
}
