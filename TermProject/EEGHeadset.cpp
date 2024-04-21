#include "EEGHeadset.h"
#include <QDebug>



EEGHeadset::EEGHeadset(QObject *parent) : QObject(parent), isConnected(false) {
    // Constructor logic, such as initializing electrodes if necessary

    // Electrodes are initialized with ids from 1- ELECTRODE_NUMBER, less then 1 and program will break
    for (int i = 0; i < ELECTRODE_NUMBER; ++i){
        electrodes.push_back(new Electrode(i));
        qInfo("electrode %d has been created ",electrodes.at(i)->getId());
    }

    //  intialize frequency list to store dom freq
    frequencies.resize(ELECTRODE_NUMBER);

    // set up headset to recieve frequency from each electrode
    for (int i = 0; i < ELECTRODE_NUMBER; ++i){
        connect(electrodes.at(i), &Electrode::dominantFrequency, this, &EEGHeadset::recievefrequency);
        connect(electrodes.at(i), &Electrode::greenLightOn, this, &EEGHeadset::recieveLightOn);
        connect(electrodes.at(i), &Electrode::greenLightOff, this, &EEGHeadset::recieveLightOff);
    }

}



EEGHeadset::~EEGHeadset() {
    // Destructor logic, if any cleanup is required
    for(auto& electrode : electrodes){
        delete electrode;
    }
}

bool EEGHeadset::makeContactWithUSer() {
    // starts operations for all electrodes
    connectElectrodes();

    frequenciesRecievedCount  = 0;

    for(auto& electrode : electrodes){

        electrode->initiateOperation(); // activate electrodes
    }

    return isConnected;
}

void EEGHeadset::connectElectrodes() {
    // Logic to establish a connection and start readsing brain signals
    // Loops through and connects electrodes .
    for(auto& electrode : electrodes){

        electrode->connectElectrode();
    }
    isConnected = true;
}


// when a helmet is taken of all electrodes connected to it should also be taken off hence all electrodes become disconnected
void EEGHeadset::disconnectElectrodes() {

    for(auto& electrode : electrodes){

        electrode->disconnectElectrode();
    }

    // Emits connectionLost to NeuresetDevice, which then emits to mainwindow for easier testing of connection loss.
    isConnected = false;
    emit connectionLost();
}



Electrode* EEGHeadset::updateElectrodeInDisplay(int newId, int oldId) {
    if (newId < 0 || newId >= ELECTRODE_NUMBER) { return nullptr; }
    if (oldId < 0 || oldId >= ELECTRODE_NUMBER) { return nullptr; }

    electrodes[oldId]->inDisplay = false;
    electrodes[newId]->inDisplay = true;
    //electrodes.replace();

    //qDebug() <<" in display state updated";
    return electrodes[newId];

}




void EEGHeadset::endElectrodeOperations()
{
    for(auto& electrode : electrodes){

        electrode->endOperation();
    }

    isConnected = false;
}






void EEGHeadset::recievefrequency(int electrodeId, double domFreq) {

    frequencies.replace(electrodeId, domFreq);

    frequenciesRecievedCount++; // might need a mutex if concurrency issues arise, it did not for me


    // check for whhich frequencies have arrived
    if ( frequenciesRecievedCount == ELECTRODE_NUMBER ){

        // emit signal to transfer the intial frequency data to the device
        emit frequencyList(0,frequencies);

        // for debugging - can remove
        for (int i = 0; i < ELECTRODE_NUMBER; ++i){

            qInfo("electrode %d : intial dominant frequency  =  %f", i, frequencies.at(i));
        }

    }
    else if( frequenciesRecievedCount == 2*ELECTRODE_NUMBER ){  // final round of electrode freqs so count will be twice the number of electrodes

        //reset freqs count  as both intial annd final freqs have arrived
        frequenciesRecievedCount = 0;

        // emit signal to transfer the final frequency data to the device
        emit frequencyList(1,frequencies);

        // for debugging - can remove
        for (int i = 0; i < ELECTRODE_NUMBER; ++i){
            qInfo("electrode %d : final dominant frequency  =  %f", i, frequencies.at(i));
        }
    }

}

void EEGHeadset::recieveLightOn() {
    emit gLightOn();
}

void EEGHeadset::recieveLightOff() {
    emit gLightOff();
}
