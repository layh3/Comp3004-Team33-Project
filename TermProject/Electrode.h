#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>



class Electrode: public QObject {

     Q_OBJECT

private:
    int electrodeId;
    bool isConnected;




    // randomly generated frequencies and amplitudes during operation
    double opFrequency1 = 0 ; // delta band
    double opFrequency2 = 0; // theta  band
    double opFrequency3 = 0; // beta band

    double opAmplitude1 = 0;
    double opAmplitude2 = 0;
    double opAmplitude3 = 0;


    double currentDominatFrequency = 0;
    double wavefunction; // combined function of 3 sine waves

    QTimer *disconnectedTimer;
    QTimer *operationTimer;
    QTimer *treatmentTimer;
    int operationTimeElapsed;
public:
    Electrode(int id); // Constructor that takes an ID
    ~Electrode(); // Destructor

    // Potentially more functions/variables related to signal measurement

    int getId();
    bool checkConnection();
    void connectElectrode();
    void disconnectElectrode();

    void initiateOperation();
    void endOperation();

    void applyTreatment();


    double calculateWaveFormYCoordinate(int time); // x ccoordinate of waveform is just the time
    double getRandomDouble(double min, double max);
    double calculateDominant();
    void emitGreenLightOff();



    QVector<double> xGraphForm; // initialize empty vectors to store operation graph data coordinates
    QVector<double> yGraphForm;

    bool inDisplay =  false;  // this is to limit which electrode can print in the terminal




private slots:
    void runOperation();
    void operationEndSequence();


signals:
    void dominantFrequency(int electrodeId, double df);
    void greenLightOn();
    void greenLightOff();


};

#endif // ELECTRODE_H
