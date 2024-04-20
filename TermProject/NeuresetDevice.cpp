#include "NeuresetDevice.h"
#include <QObject>

NeuresetDevice::NeuresetDevice(QObject *parent) : QObject(parent), batteryLevel(55), headset(new EEGHeadset()) {
    // Initialize the device, potentially set up the EEGHeadset
    sessionState = false;
    //currentSession = new Session(sessionProgression);   // initialize tthis elsewhere

    connect(headset, &EEGHeadset::connectionLost, this, &NeuresetDevice::handleConnectionLost); // occurs when electrode is disconnected

    // set up response for incoming session object data
    connect(headset, &EEGHeadset::frequencyList, this, &NeuresetDevice::updateSessionObject);
    connect(headset, &EEGHeadset::gLightOn, this, &NeuresetDevice::recGreenOn);
    connect(headset, &EEGHeadset::gLightOff, this, &NeuresetDevice::recGreenOff);
}

NeuresetDevice::~NeuresetDevice() {
    // Clean up resources, including deleting the headset and currentSession if necessary

    //delete all Sessions in the session array as well as the currennt ssession object
}

void NeuresetDevice::startSession() {
    // Logic to start a new session
    if (headset->makeContactWithUSer()) {
        sessionState = true;
    }else{
        sessionState = false;
    }
}

void NeuresetDevice::pauseSession() {
    // Logic to pause the current session
    headset ->disconnectElectrodes();
    sessionState = false;


}

void NeuresetDevice::resumeSession() {
    // Logic to resume the current session
    headset->connectElectrodes();
    sessionState = true;
}

void NeuresetDevice::endSession() {
    // Logic to end the current session, may involve cleaning up the Session object
    headset->endElectrodeOperations();
    sessionState = false;
}

Session* NeuresetDevice::getCurrentSession() const {
    // Return the current session
    return currentSession;
}

bool NeuresetDevice::getSessionState(){
    return sessionState;
}

void NeuresetDevice::lowBattery() {
    // Logic for low battery indication
    batteryLevel = 0;
    emit deadBattery();
}

void NeuresetDevice::powerOff() {
    // Logic to power off the device
    headset->endElectrodeOperations();
    currentSession = nullptr;
    sessionState = false;
    noOfSessions = 0;

}

void NeuresetDevice::handleConnectionLost(){
    emit contactLost();
}


int NeuresetDevice::getBatteryLevel() {
    return batteryLevel;
}

void NeuresetDevice::setBatteryLevel(int level) {
    batteryLevel = level;
}



Electrode* NeuresetDevice::displayElectrode(int newId, int oldId) {


    //qDebug()<< "inside neuroset device update displayed electrode";
    return headset->updateElectrodeInDisplay(newId, oldId);
}



void NeuresetDevice::initializeSessionObject(QString startingTime){
    currentSession  =  new Session(noOfSessions);
    noOfSessions++;

    currentSession->setStartTime(startingTime);

}



void NeuresetDevice::storeSessionObject(){ // should only be usedd if the session was successful completed

    SessionArchive.append(currentSession);

}



void NeuresetDevice::printAllSessions(){

    for(int i=0; i< SessionArchive.size(); i++){
        SessionArchive.at(i)->toString();
    }
}


void NeuresetDevice::updateSessionObject(int identifier, QVector<double> dominants){

    if(identifier == 0){ // 0 = initial frequencies
        currentSession->setInitialDominantsAndBaseline(dominants);
    }
    else if(identifier == 1){ // 1 = final frequencies
        currentSession->setFinalDominantsAndBaseline(dominants);

    }

}

void NeuresetDevice::recGreenOn() {
    emit gOn();
}

void NeuresetDevice::recGreenOff() {
    emit gOff();
}

QVector<Session*> NeuresetDevice::getSessionArchive(){
    return SessionArchive;
}



