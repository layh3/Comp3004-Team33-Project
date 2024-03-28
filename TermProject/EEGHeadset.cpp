#include "EEGHeadset.h"

EEGHeadset::EEGHeadset() : isConnected(false) {
    // Constructor logic, such as initializing electrodes if necessary
}

EEGHeadset::~EEGHeadset() {
    // Destructor logic, if any cleanup is required
}

void EEGHeadset::checkContact() {
    // Implement logic to check contact of electrodes with the scalp
    //Kesh comment to test pushing to the repo using git
}

void EEGHeadset::connect() {
    // Logic to establish a connection, possibly setting isConnected to true
    isConnected = true;
}

void EEGHeadset::disconnect() {
    // Logic to disconnect, possibly setting isConnected to false
    isConnected = false;
}
