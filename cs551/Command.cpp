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
#define TEST 1

/**
 * @return string
 */
string * Command::getPath() {
    return path;
}

/**
 * @param value
 */
void Command::setPath(string * value) {
    if(path != NULL){
        delete path;
        path = NULL;
    }
    path = value;
}

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
        name = NULL;
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
        env = NULL;
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
        params = NULL;
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

void Command::executeWithExecve() {
    cout << "Begin of execve with code " << endl;
    pid_t pid;
    if ((pid = fork()) ==-1) {
        perror("fork error");
    } else if (pid == 0){
        cout << "env " << env->c_str() << endl;
        int i = execve(generateFileName(),generateParams(),generateEnv());
        cout << "End of execve with code " << i << endl;
        cout << "Return not expected. Must be an execve error.n" <<endl;
    }
}

const char * Command::generateFileName() {
    //TODO: return the filename of the file that contains the executable image of the new process
#ifdef TEST
    return "/bin/ls";
#endif
    return NULL;
}

char * const * Command::generateParams(){
    // TODO:  return a param list like that: char *const parmList[] = {"/bin/ls", "-l", "/u/userid/dirname", NULL};
#ifdef TEST
    char *const paramList[] = { "-al",NULL};
    return paramList;
#endif
    return NULL;
}

char * const * Command::generateEnv(){
    // TODO:  return a env list like that: char *const envParms[2] = {"EXAMPLE=test", NULL};
#ifdef TEST
    char *const envParams[2] = {"EXAMPLE=test", NULL};
    return envParams;
#endif
    return NULL;
}

ostream &operator<<(ostream &os, const Command &command) {
    os << "path: " << (command.path  == NULL ? "":*command.path) << " name: " << (command.name  == NULL ? "":*command.name) << " env: " << (command.env  == NULL ? "":*command.env) << " params: "
       << (command.params  == NULL ? "":*command.params);
    return os;
}

Command::Command(void){
    path = NULL;
    name = NULL;
    env = NULL;
    params = NULL;
}

Command::~Command(void){
    cout << "Deleting path in Command" << endl;
    if(path != NULL){
        delete path;
        path = NULL;
    }
    cout << "path deleted in command" << endl;
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