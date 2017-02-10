/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Command.h"

/**
 * Command implementation
 */


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

string Command::execute(string env, string params) {
    if(name != NULL && env.empty() != true){
        string cmd = env +"|"+*name+" "+params;
        array<char, 128> buffer;
        string result;
        shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) throw runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != NULL)
                result += buffer.data();
        }
        return result;
    }
    return NULL;
}

Command::Command(void){
    name = NULL;
}

Command::~Command(void){
    cout << "Deleting name in Command" << endl;
    if(name != NULL){
        delete name;
        name = NULL;
    }
    cout << "name deleted in command" << endl;
}