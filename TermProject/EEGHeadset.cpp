#include "EEGHeadset.h"
#include <QDebug>

#define ELECTRODE_NUMBER 1

EEGHeadset::EEGHeadset(QObject *parent) : QObject(parent), isConnected(false) {
    // Constructor logic, such as initializing electrodes if necessary
    // Electrodes are initialized with ids from 0-20.
    for (int i = 0; i < ELECTRODE_NUMBER; ++i){
        electrodes.push_back(new Electrode(i));
    }
}

EEGHeadset::~EEGHeadset() {
    // Destructor logic, if any cleanup is required
    for(auto& electrode : electrodes){
        delete electrode;
    }
}

bool EEGHeadset::makeContactWithUSer() {
    // Implement logic to check contact of electrodes with the scalp
    connect();

    for(auto& electrode : electrodes){

        electrode->initiateOperation(); // activate electrodes
    }

    return isConnected;
}

void EEGHeadset::connect() {
    // Logic to establish a connection and start readsing brain signals
    // Loops through and connects electrodes .
    for(auto& electrode : electrodes){

        electrode->connectElectrode();
    }
    isConnected = true;
}


// when a helmet is taken of all electrodes connected to it should also be taken off hence all electrodes become disconnected
void EEGHeadset::disconnect() {

    for(auto& electrode : electrodes){

        electrode->disconnectElectrode();
    }

    // Emits connectionLost to NeuresetDevice, which then emits to mainwindow for easier testing of connection loss.
    isConnected = false;
    emit connectionLost();
}



void EEGHeadset::endElectrodeOperations()
{

}








