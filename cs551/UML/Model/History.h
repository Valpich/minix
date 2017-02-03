/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _HISTORY_H
#define _HISTORY_H

#include "FileManager.h"


class History {
public: 
    
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
    FileManager fileManager;
};

#endif //_HISTORY_H