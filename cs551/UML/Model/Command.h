/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMAND_H
#define _COMMAND_H

class Command {
public: 
    
    string getName();
    
    /**
     * @param value
     */
    void setName(string value);
private: 
    string name;
    string path;
};

#endif //_COMMAND_H