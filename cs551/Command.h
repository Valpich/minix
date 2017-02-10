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


using namespace std;

class Command {
public: 
    
    Command();

    ~Command();
    
    string * getName();
    
    /**
     * @param value
     */
    void setName(string * value);

    string execute(string env, string params);
private: 
    string * name;
};

#endif //_COMMAND_H
