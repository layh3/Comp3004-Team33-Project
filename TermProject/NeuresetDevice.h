#ifndef NEURESETDEVICE_H
#define NEURESETDEVICE_H

#include <string>
#include "Session.h"
#include "EEGHeadset.h"
#include <QObject>

class NeuresetDevice : public QObject {
    Q_OBJECT
private:
    int batteryLevel;
    bool sessionState;
    std::string currentEEGSite;
    EEGHeadset* headset;
    Session* currentSession;
    int sessionProgression = 0;

public:
    NeuresetDevice(QObject *parent = nullptr);
    ~NeuresetDevice();

    void startSession();
    void pauseSession();
    void resumeSession();
    void endSession();
    Session* getCurrentSession() const;
    bool getSessionState();
    void lowBattery();
    void powerOff();
    void handleConnectionLost();

    //----------------------------------------
    void activateHeadset();
    int getBatteryLevel();
    void setBatteryLevel(int level);

signals:
    void contactLost();
    void deadBattery();
};

#endif // NEURESETDEVICE_H
