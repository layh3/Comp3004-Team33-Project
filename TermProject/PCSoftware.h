#ifndef PCSOFTWARE_H
#define PCSOFTWARE_H

#include "SessionLog.h" // Include the SessionLog header

class PCSoftware {
private:
    SessionLog sessionLog; // Composition relationship with SessionLog

public:
    PCSoftware(); // Constructor
    ~PCSoftware(); // Destructor

    // Methods for interacting with the SessionLog
    void uploadSessionData();
    void displaySessionHistory();
};

#endif // PCSOFTWARE_H
