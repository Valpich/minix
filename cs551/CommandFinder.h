/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMANDFINDER_H
#define _COMMANDFINDER_H

#include <string>
#include <vector>

#include "Command.h"

using namespace std;

class CommandFinder {
public: 
    
    CommandFinder();

    ~CommandFinder();

    vector<string> * getFolderPaths();
    
    /**
     * @param value
     */
    void setFolderPaths(vector<string> * value);
    
    vector<Command> * findAllCommands();
private: 
    vector<string> * folderPaths;
};

#endif //_COMMANDFINDER_H
