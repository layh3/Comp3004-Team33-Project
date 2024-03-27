#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include <vector>
#include "Electrode.h"

class EEGHeadset {
private:
    bool isConnected;
    std::vector<Electrode> electrodes;

public:
    EEGHeadset();
    ~EEGHeadset();

    void checkContact();
    void connect();
    void disconnect();
};

#endif // EEGHEADSET_H
