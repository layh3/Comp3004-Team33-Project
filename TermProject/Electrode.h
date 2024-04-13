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

    double wavefunction; // combined function of 3 sine waves

    QTimer *operationTimer;
    int operationTimeElapsed;
public:
    Electrode(int id); // Constructor that takes an ID
    ~Electrode(); // Destructor

    bool checkConnection();
    void connectElectrode();
    void disconnectElectrode();



    // Potentially more functions/variables related to signal measurement

    QVector<double> xGraphForm; // initialize empty vectors to store operation graph data coordinates
    QVector<double> yGraphForm;

    double calculateWaveFormYCoordinate(int time); // x ccoordinate of waveform is just the time

    double getRandomDouble(double min, double max){
        return min + QRandomGenerator::global()->generateDouble() * (max - min);
    };

    void initiateOperation();
    void cleanUpOperation();


private slots:
    void runOperation();



};

#endif // ELECTRODE_H
