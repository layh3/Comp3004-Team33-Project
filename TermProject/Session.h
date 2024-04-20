#ifndef SESSION_H
#define SESSION_H

#include <QVector>
#include <string>
#include <QObject>
#include "TreatmentProtocol.h"
#include "EEGHeadset.h"

class Session: public QObject {

    Q_OBJECT

private:
    // the duration of each session is fixed at 60 seconds so it does not have to be recorded, although the electrode operations are 50 seconds

    int sessionId;
    QString startTime = "start time not set";
    //std::string endTime;
    //TreatmentProtocol currentTreatmentProtocol; // remove

    QVector<double> intialDominants;
    QVector<double> finalDominants;

    double intialBaseline;
    double finalBaseline;

public:
    Session(int id); // Constructor that takes an ID
    ~Session(); // Destructor

    double calculateBaseline(QVector<double>& dominants);
    //void applyTreatment(); // this is redundant treatment is applied in the electrode  class

    const QString& getStartTime() const;
    void setStartTime(QString startingTime);
    void setInitialDominantsAndBaseline(QVector<double>& domFrequencies );
    void setFinalDominantsAndBaseline(QVector<double>& domFrequencies);

    void toString();
};

#endif // SESSION_H
