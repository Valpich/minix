/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _SHELL_H
#define _SHELL_H

#include "SignalHandler.h"
#include "ContextSaver.h"
#include "Profile.h"
#include "History.h"
#include "Command.h"


class Shell : public SignalHandler, public ContextSaver {
public:

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

    vector <Command> getCommandList();

    /**
     * @param value
     */
    void setCommandList(vector <Command> value);

    CommandFinder getCommandFinder();

    /**
     * @param value
     */
    void setCommandFinder(CommandFinder value);

private:
    string name = Shell
    name;
    Profile profile;
    History history;
    vector <Command> commandList;
};

#endif //_SHELL_H