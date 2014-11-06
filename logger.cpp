#include "logger.h"

Logger::Logger()
{
    outLog = new QTextStream(stdout);
}

void Logger::printLog(int level, QString message) {
    if (level == DEBUG) {
        (*outLog) << "[DEBUG] ";
    }
    if (level == ERROR) {
        (*outLog) << "[ERROR] ";
    }
    if (level == WARNING) {
        (*outLog) << "[WARNING] ";
    }
    if (level == INFO) {
        (*outLog) << "[INFO] ";
    }

    (*outLog) << message << "\n";
    (*outLog).flush();
}
