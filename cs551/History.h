/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _HISTORY_H
#define _HISTORY_H

#include <string>
#include <vector>

#include "FileManager.h"
#include "Command.h"

using namespace std;

class History {
public:

    static const string defaultPath;

    History();

    ~History();

    string * getPath();
    
    /**
     * @param value
     */
    void setPath(string * value);

    FileManager * getFileManager();

    /**
     * @param value
     */
    void setFileManager(FileManager * value);

    /**
    * @param command
    */
    void logCommand(Command * command);

private:
    string * path;
    FileManager * fileManager;
};

#endif //_HISTORY_H
