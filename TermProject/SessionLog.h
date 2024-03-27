#ifndef SESSIONLOG_H
#define SESSIONLOG_H

#include <vector>
#include "Session.h"

class SessionLog {
private:
    std::vector<Session> sessionLogs;

public:
    SessionLog(); // Constructor
    ~SessionLog(); // Destructor

    void addSession(const Session& session);
    Session getSessionDetails(int sessionId) const;
};

#endif // SESSIONLOG_H
