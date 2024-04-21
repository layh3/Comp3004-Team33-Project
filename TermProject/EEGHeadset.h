#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include <vector>
#include "Electrode.h"
#include <QObject>

#define ELECTRODE_NUMBER 10  // if u change this theres a part in mainwindow class where u also have to manually reflect the change in the spinbox

class EEGHeadset : public QObject {
    Q_OBJECT
private:
    bool isConnected;
    std::vector<Electrode*> electrodes;

    QVector<double> frequencies;
    int frequenciesRecievedCount = 0 ;  // used to ensure the frequency list signal is only sent once all electrodes have sent their dom frequencies

public:
    EEGHeadset(QObject *parent = nullptr);
    ~EEGHeadset();

    bool makeContactWithUSer();
    void connectElectrodes();
    void disconnectElectrodes();
    Electrode* updateElectrodeInDisplay(int newId, int oldId);


    //-----------------------------

    void endElectrodeOperations();  // this is only used if the ui stop button is pressed, disconnection is treated differently


private slots:
    void recievefrequency(int electrodeId, double domFreq);
    void recieveLightOn();
    void recieveLightOff();

signals:
    void connectionLost();
    void frequencyList(int identifier, QVector<double> dominants);  // sends the collected dom freqs to the device, the identifier is used to separate the initial(=0) and final(=1) freqs
    void gLightOn();
    void gLightOff();
};

#endif // EEGHEADSET_H

