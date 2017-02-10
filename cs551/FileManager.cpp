/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "FileManager.h"
#define DEBUG 1
/**
 * FileManager implementation
 */


FileManager::FileManager(void){
    outStream = NULL;
    inStream = NULL;
}

FileManager::~FileManager(void){
    // Close file stream if open
    cout<< "Deleting outStream in fileManager" << endl;
    if(outStream != NULL){
        if(outStream->is_open()){
            outStream->close();
        }
        delete outStream;
        outStream = NULL;
    }
    cout<< "outStream deleted in fileManager" << endl;
    cout<< "Deleting inStream in fileManager" << endl;
    if(inStream != NULL) {
        if (inStream->is_open()) {
            inStream->close();
        }
        delete inStream;
        inStream = NULL;
    }
    cout<< "inStream deleted in fileManager" << endl;
}

bool FileManager::appendToFile(string filePath, vector<string> textToAppend){
    outStream = new ofstream();
    outStream->open(filePath, ios::out |ios::ate |ios::app);
    if (outStream->is_open()){
        for(string line : textToAppend){
            *outStream << line << endl;
#ifdef DEBUG
            cout << line << endl;
#endif
        }
        outStream->close();
        delete outStream;
        outStream = NULL;
        return true;
    }else{
#ifdef DEBUG
        cout << "Failed to open the file" << '\n';
#endif
        delete outStream;
        inStream = NULL;
        return false;
    }
}

bool FileManager::replaceFileContent(string filePath, vector<string> newFileContent){
    outStream = new ofstream();
    outStream->open(filePath, ios::out |ios::trunc);
    if (outStream->is_open()){
        for(string line : newFileContent){
            *outStream << line << endl;
#ifdef DEBUG
            cout << line << endl;
#endif
        }
        outStream->close();
        delete outStream;
        outStream = NULL;
        return true;
    }else{
#ifdef DEBUG
        cout << "Failed to open the file" << '\n';
#endif
        delete outStream;
        outStream = NULL;
        return false;
    }
}

vector<string> FileManager::readFileToString(string filePath){
    inStream = new ifstream();
    inStream->open(filePath);
    vector<string> vectorOfString;
    if (inStream->is_open()){
        string line;
        while (getline(*inStream,line)){
#ifdef DEBUG
            cout << line << '\n';
#endif
            vectorOfString.push_back(line);
        }
        inStream->close();
        delete inStream;
        inStream = NULL;
        return vectorOfString;
    }else{
#ifdef DEBUG
        cout << "Failed to open the file" << '\n';
#endif
        delete inStream;
        inStream = NULL;
        return vectorOfString;
    }
}