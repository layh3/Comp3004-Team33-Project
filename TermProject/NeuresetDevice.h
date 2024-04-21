#ifndef NEURESETDEVICE_H
#define NEURESETDEVICE_H

#include <string>
#include "Session.h"
#include "EEGHeadset.h"
#include <QObject>
#include <QDebug>

class NeuresetDevice : public QObject {
    Q_OBJECT
private:
    int batteryLevel;  // set iin the contructor
    bool sessionState;
    //std::string currentEEGSite;
    EEGHeadset* headset;
    Session* currentSession;
    QVector<Session*> SessionArchive;
    int noOfSessions = 0; // for session object

    // create an array to store sessions here

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
    Electrode* displayElectrode(int newId, int oldId);
    QVector<Session*> getSessionArchive();

    //----------------------------------------
    //void activateHeadset();
    int getBatteryLevel();
    void setBatteryLevel(int level);

    // session object controllers
    void initializeSessionObject(QString startingTime);
    void storeSessionObject();
    void printAllSessions();

private slots:
    void updateSessionObject(int identifier, QVector<double> dominants); // this updates the session object automatically u just have to iniitialize it
    void recGreenOn();
    void recGreenOff();

signals:
    void contactLost();
    void deadBattery();
    void gOn();
    void gOff();
};

#endif // NEURESETDEVICE_H
