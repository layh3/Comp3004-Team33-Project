#include "NeuresetDevice.h"
#include <QObject>

NeuresetDevice::NeuresetDevice(QObject *parent) : QObject(parent), headset(new EEGHeadset()) {
    // Initialize the device, potentially set up the EEGHeadset
    currentSession = new Session(sessionProgression);
    sessionProgression++;
    connect(headset, &EEGHeadset::connectionLost, this, &NeuresetDevice::handleConnectionLost);
}

NeuresetDevice::~NeuresetDevice() {
    // Clean up resources, including deleting the headset and currentSession if necessary
    delete currentSession;
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
    sessionState = false;
}

void NeuresetDevice::resumeSession() {
    // Logic to resume the current session
    sessionState = true;
}

void NeuresetDevice::endSession() {
    // Logic to end the current session, may involve cleaning up the Session object
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
}

void NeuresetDevice::powerOff() {
    // Logic to power off the device
}

void NeuresetDevice::handleConnectionLost(){
    emit contactLost();
}
