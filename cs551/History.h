/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _HISTORY_H
#define _HISTORY_H

#include <string>

#include "FileManager.h"

using namespace std;

class History {
public: 
    
    History();

    ~History();


    string getPath();
    
    /**
     * @param value
     */
    void setPath(string value);
    
    /**
     * @param value
     */
    void setFileManager(FileManager value);
    
    FileManager getFileManager();
private: 
    string path;
    FileManager * fileManager;
};

#endif //_HISTORY_H
