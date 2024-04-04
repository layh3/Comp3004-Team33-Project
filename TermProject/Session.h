#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <string>
#include "TreatmentProtocol.h"

class Session {
private:
    int sessionId;
    std::string startTime;
    std::string endTime;
    std::vector<float> baselineFrequencies;
    TreatmentProtocol currentTreatmentProtocol;

public:
    Session(int id); // Constructor that takes an ID
    ~Session(); // Destructor

    void calculateBaseline();
    void applyTreatment();
};

#endif // SESSION_H
