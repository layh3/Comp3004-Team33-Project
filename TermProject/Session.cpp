#include "Session.h"

Session::Session(int id)
    : sessionId(id), currentTreatmentProtocol(1, 0.0f) {
    //Protocol stuff just here to make it run, needs to be fixed


    // Constructor implementation could initialize member variables

}

Session::~Session() {
    // Destructor implementation, if needed for cleanup
}

void Session::calculateBaseline() {
    // Logic to calculate baseline frequencies
}

void Session::applyTreatment() {
    // Logic to apply a treatment protocol
}
