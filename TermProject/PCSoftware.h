#ifndef PCSOFTWARE_H
#define PCSOFTWARE_H


class PCSoftware {
private:
    //SessionLog sessionLog; // Composition relationship with SessionLog // sessionlog has been removed

public:
    PCSoftware(); // Constructor
    ~PCSoftware(); // Destructor

    // Methods for interacting with the SessionLog
    void uploadSessionData();
    void displaySessionHistory();
};

#endif // PCSOFTWARE_H
