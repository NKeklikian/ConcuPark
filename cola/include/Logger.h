#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "Runnable.h"
#include "../../fifos/FifoEscritura.h"
#define LOG_MODES 5

typedef enum Logmode{
    DEBUG,
    WARNING,
    ERROR,
    INFO,
    TRACE
} LOG_MODE;

class Logger : public Runnable
{
    public:
        virtual ~Logger();

        void setLogPath(std::string path) { log_path = path; }
        static Logger* getInstance();
        void Log(std::string name, std::string comment, LOG_MODE mode);

    protected:

        virtual void _run();

    private:

        Logger();
        Logger* instancia;

        LOG_MODE mode;
        std::string log_path;
        std::string mode_symbols[LOG_MODES];
        std::string timestamp();
};

#endif // LOGGER_H
