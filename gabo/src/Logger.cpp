#include "Logger.h"
#include "defines.h"
#include <fstream>
#include <ctime>
#include "FifoLectura.h"

static Logger* instance = 0;

Logger::Logger()
{
    log_path = "log.txt";
    mode = DEBUG; //cambiar para cambiar el default. en la version final el default deberia ser ERROR
    mode_symbols[0] = "D.\t";
    mode_symbols[1] = "W.\t";
    mode_symbols[2] = "E.\t";
    mode_symbols[3] = "I.\t";
    mode_symbols[4] = "T.\t";
    std::ofstream log_file(log_path.c_str(), std::ofstream::app);
    log_file << std::endl << "/---------------------- " << timestamp() << " ----------------------/" << std::endl;
    log_file.close();
}

Logger::~Logger()
{
    if(instancia != NULL)
        delete instancia;
    instancia = NULL;
}

std::string Logger::timestamp(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);

    return str;
}

void Logger::Log(std::string name, std::string comment, LOG_MODE comment_mode){
    if(comment_mode >= mode){
        /*std::ofstream log_file(log_path.c_str(), std::ofstream::out | std::ofstream::app);
        log_file << mode_symbols[comment_mode] << comment << std::endl;
        log_file.close();*/
        std::string out_final = name + "\t" + std::to_string(getpid()) + "\t" + mode_symbols[comment_mode] + comment + "\n";

        FifoEscritura canal_logger ( C_LOGGER );

        canal_logger.abrir();
        canal_logger.escribir ( static_cast<const void*>(out_final.c_str()),out_final.length() );

        canal_logger.cerrar ();
    }
}

Logger* Logger::getInstance(){
    if(!instance){
        instance = new Logger;
    }
    return instance;
}

void Logger::_run(){
    // esto tal vez va en la clase como variables
    static const int BUFFSIZE = sizeof(char) * 100;
    FifoLectura canal_logger ( C_LOGGER );
    canal_logger.abrir();

    char buffer[BUFFSIZE];
    while(true){ // ver cuando frenar (para que cierre bien y se cierren los pipes)
        canal_logger.abrir();
        std::string mensaje = "";
        ssize_t bytes_leidos;
        while(bytes_leidos = canal_logger.leer(static_cast<void*>(&buffer),BUFFSIZE)){
            mensaje = buffer;
            mensaje.resize(bytes_leidos);
            std::ofstream log_file(log_path.c_str(), std::ofstream::out | std::ofstream::app);
            log_file /*<< mode_symbols[comment_mode]*/ << mensaje;
            log_file.close();
        }
        canal_logger.cerrar();
    }

    canal_logger.cerrar();
    canal_logger.eliminar();
}
