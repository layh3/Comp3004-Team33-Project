#include "Electrode.h"

Electrode::Electrode(int id) : electrodeId(id), isConnected(false) {
    // Initialize the electrode with the given ID and as disconnected
}

Electrode::~Electrode() {
    // Cleanup tasks if necessary
}

void Electrode::checkConnection() {
    // Implement the logic to check if the electrode is properly connected
}

void Electrode::connect() {
    // Logic to connect the electrode
    isConnected = true;
}

void Electrode::disconnect() {
    // Logic to disconnect the electrode
    isConnected = false;
}
