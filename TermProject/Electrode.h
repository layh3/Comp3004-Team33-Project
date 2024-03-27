#ifndef ELECTRODE_H
#define ELECTRODE_H

class Electrode {
private:
    int electrodeId;
    bool isConnected;

public:
    Electrode(int id); // Constructor that takes an ID
    ~Electrode(); // Destructor

    void checkConnection();
    void connect();
    void disconnect();
    // Potentially more functions related to signal measurement
};

#endif // ELECTRODE_H
