/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMANDFINDER_H
#define _COMMANDFINDER_H

#include <string>
#include <vector>

class CommandFinder {
public: 
    
    vector<std::string> getFolderPaths();
    
    /**
     * @param value
     */
    void setFolderPaths(vector<std::string> value);
    
    vector<Command> findAllCommands();
private: 
    vector<std::string> folderPaths;
};

#endif //_COMMANDFINDER_H
