/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMAND_H
#define _COMMAND_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

#include <unistd.h>


using namespace std;

class Command {
public: 
    
    Command();

    ~Command();

    string * getPath();

    /**
     * @param value
     */
    void setPath(string * value);

    string * getName();
    
    /**
     * @param value
     */
    void setName(string * name);

    string * getEnv();

    /**
     * @param value
     */
    void setEnv(string * name);

    string * getParams();

    /**
     * @param value
     */
    void setParams(string * params);

    string execute();

    void executeWithExecve();

    friend ostream &operator<<(ostream &os, const Command &command);

private:
    string * path;
    string * name;
    string * env;
    string * params;
    const char *  generateFileName();
    char * const * generateParams();
    char * const * generateEnv();
};

#endif //_COMMAND_H
