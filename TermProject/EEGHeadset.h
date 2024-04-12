#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include <vector>
#include "Electrode.h"
#include <QObject>

class EEGHeadset : public QObject {
    Q_OBJECT
private:
    bool isConnected;
    std::vector<Electrode*> electrodes;

public:
    EEGHeadset(QObject *parent = nullptr);
    ~EEGHeadset();

    bool makeContactWithUSer();
    void connect();
    void disconnect();


    //-----------------------------
    void activateElectrodes();

signals:
    void connectionLost();
};

#endif // EEGHEADSET_H
