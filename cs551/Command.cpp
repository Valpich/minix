/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Command.h"

/**
 * Command implementation
 */
#define DEBUG 1

/**
 * @return string
 */
string * Command::getName() {
    return name;
}

/**
 * @param value
 */
void Command::setName(string * value) {
    if(name != NULL){
        delete name;
    }
	name = value;
}

/**
 * @return string
 */
string * Command::getEnv() {
    return env;
}

/**
 * @param value
 */
void Command::setEnv(string * value) {
    if(env != NULL){
        delete env;
    }
    env = value;
}

/**
 * @return string
 */
string * Command::getParams() {
    return params;
}

/**
 * @param value
 */
void Command::setParams(string * value) {
    if(params != NULL){
        delete params;
    }
    params = value;
}

string Command::execute() {
    if(name != NULL && env != NULL){
        if(!(*env).empty()) {
            string cmd = *env + "|" + *name + " " + *params;
            array<char, 128> buffer;
            string result;
            shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
            if (!pipe) throw runtime_error("popen() failed!");
            while (!feof(pipe.get())) {
                if (fgets(buffer.data(), 128, pipe.get()) != NULL)
                    result += buffer.data();
            }
#ifdef DEBUG
            cout << result << endl;
#endif
            return result;
        }
    }
    return NULL;
}

Command::Command(void){
    name = NULL;
    env = NULL;
    params = NULL;
}

Command::~Command(void){
    cout << "Deleting name in Command" << endl;
    if(name != NULL){
        delete name;
        name = NULL;
    }
    cout << "name deleted in command" << endl;
    cout << "Deleting env in Command" << endl;
    if(env != NULL){
        delete env;
        env = NULL;
    }
    cout << "env deleted in command" << endl;
    cout << "Deleting params in Command" << endl;
    if(params != NULL){
        delete params;
        params = NULL;
    }
    cout << "params deleted in command" << endl;
}