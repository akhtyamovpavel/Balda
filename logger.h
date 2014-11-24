#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>
#include <QString>
#include <QFile>

const int INFO = -1;
const int DEBUG = 0;
const int WARNING = 1;
const int ERROR = 2;

namespace {
    class Logger
    {
        QTextStream* outLog;
        QFile* file;
    public:
        Logger();

        template<class T>
        void printLog(int level, T message) const;

        ~Logger() {
            file->close();
            delete file;
            delete outLog;
        }
    };

    Logger::Logger()
    {
        file = new QFile("log.txt");
        if (file->open(QIODevice::Append | QIODevice::Text)) {
            outLog = new QTextStream(file);
        }
    }

    template<class T>
    void Logger::printLog(int level, T message) const {
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
}

#endif // LOGGER_H
