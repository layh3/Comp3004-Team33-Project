#ifndef NEURESETDEVICE_H
#define NEURESETDEVICE_H

#include <string>
#include "Session.h"
#include "EEGHeadset.h"

class NeuresetDevice {
private:
    int batteryLevel;
    std::string sessionState;
    std::string currentEEGSite;
    EEGHeadset* headset;
    Session* currentSession;

public:
    NeuresetDevice();
    ~NeuresetDevice();

    void startSession();
    void pauseSession();
    void resumeSession();
    void endSession();
    Session* getCurrentSession() const;
    void lowBattery();
    void powerOff();
};

#endif // NEURESETDEVICE_H
