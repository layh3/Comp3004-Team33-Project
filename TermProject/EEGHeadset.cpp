#include "EEGHeadset.h"
#include <QDebug>

#define ELECTRODE_NUMBER 21

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

bool EEGHeadset::checkContact() {
    // Implement logic to check contact of electrodes with the scalp
    connect();
    return isConnected;
}

void EEGHeadset::connect() {
    // Logic to establish a connection, possibly setting isConnected to true
    // Loops through electrodes, checking the connection of each one. Returns false if any connections fail.
    for(auto& electrode : electrodes){
        electrode->checkConnection();
        if(!electrode->checkConnection()){
            isConnected = false;
            return;
        }
        //qDebug() << "Connecting Electrode";
    }
    isConnected = true;
}

void EEGHeadset::disconnect() {
    // Logic to disconnect, possibly setting isConnected to false
    // Emits connectionLost to NeuresetDevice, which then emits to mainwindow for easier testing of connection loss.
    isConnected = false;
    emit connectionLost();
}
