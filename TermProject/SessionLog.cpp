#include "SessionLog.h"
#include <QtGlobal>

SessionLog::SessionLog() {
    // Constructor for SessionLog can initialize the sessionLogs vector if needed
}

SessionLog::~SessionLog() {
    // Destructor for SessionLog, if any cleanup is necessary
}

void SessionLog::addSession(const Session& session) {
    // Logic to add a Session object to the sessionLogs vector

    //TO BE REMOVED, CURRENTLY ALLOWING BUILD TO PASS
    Q_UNUSED(session);
}

Session SessionLog::getSessionDetails(int sessionId) const {
    // Logic to retrieve details of a session by sessionId

    //TO BE REMOVED, CURRENTLY ALLOWING BUILD TO PASS
    Q_UNUSED(sessionId);
    return Session(1);

}
