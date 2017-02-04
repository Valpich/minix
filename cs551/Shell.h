/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _SHELL_H
#define _SHELL_H

#include <iostream>
#include <string>
#include <vector>
#include <csetjmp>

#include "SignalHandler.h"
#include "Profile.h"
#include "History.h"
#include "Command.h"
#include "CommandFinder.h"


class Shell: SignalHandler, ContextSaver {
public: 
    
    Shell();

    ~Shell();

    std::string getName();
    
    /**
     * @param value
     */
    void setName(std::string value);
    
    Profile getProfile();
    
    /**
     * @param value
     */
    void setProfile(Profile value);
    
    History getHistory();

    /**
     * @param value
     */
    void setHistory(History value);
    
    std::vector<Command> getCommandList();
    
    /**
     * @param value
     */
    void setCommandList(std::vector<Command> value);
    
    CommandFinder getCommandFinder();
    
    /**
     * @param value
     */
    void setCommandFinder(CommandFinder value);
private: 
    std::string name;
    Profile profile;
    History history;
    std::vector<Command> commandList;
    CommandFinder commandFinder;
    std::jmp_buf jumpBuffer;
};

#endif //_SHELL_H
