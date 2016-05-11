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
    volume = 1;
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
void KafkaStatesMachine::updateStates( float theEnergy ){
    float dice = ofRandom( 0 , 1 );
    map<float,KafkaStatesMachineTransition*> posibleTransitions;
    map<float,KafkaStatesMachineTransition*>::iterator posibleTransitionsIterator;
    for( int t = 0 ; t < transitions.size() ; t ++ ){
        if( transitions[t]->getNameStateInitial() == currentState->getName() ){
            pair<float,KafkaStatesMachineTransition*> newPosibleTransition;
            float energuiFinal = transitions[t]->getStateFinal()->getEnergy();
            float energyInitial = transitions[t]->getStateInitial()->getEnergy();
            
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
void KafkaStatesMachine::setVolume( float theVolume ){
    //todocurrentState->sound.setVolume( theVolume );
}
//-------------------------------------------------------------
void KafkaStatesMachine::start(){
    //todocurrentState->sound.play();
}
//-------------------------------------------------------------
void KafkaStatesMachine::stop(){
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
    
    (*fileIn) >> junk;
    if( junk != "States" ){
        cout << "* KafkaStatesMachine  load: Bad tag States\n";
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
        if( !newState->loadFromTSV( fileIn ) ){
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
float KafkaStatesMachine::getCurrentStatePercentaje(){
    return 1;//todocurrentState->sound.getPosition();
}
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
bool KafkaStatesMachine::justFinishidState(){
    return 1;//todo!currentState->sound.getIsPlaying();
}
//-----------------------------------------------------------
void KafkaStatesMachine::setCurrentState( KafkaStatesMachineState* newState ){
    cout << "Machine : " << name << "  form " << currentState->getName();
    currentState = newState;
    cout << " to " << currentState->getName() << "\n";
    //todocurrentState->sound.play();
    hasJustChangedState = true;
    //todoif( isAtcive() )
        //todocurrentState->sound.setVolume(volume);
    //todoelse
        //todocurrentState->sound.setVolume(0);
}