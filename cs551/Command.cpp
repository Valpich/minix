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
 * The static attribute representing if the alarm option is enabled
 */
bool Command::alarmEnabled;

string * Command::getPath() {
    return path;
}

void Command::setPath(string * value) {
    if(path != NULL){
        delete path;
        path = NULL;
    }
    path = value;
}

string * Command::getName() {
    return name;
}

void Command::setName(string * value) {
    if(name != NULL){
        delete name;
        name = NULL;
    }
	name = value;
}

string * Command::getEnv() {
    return env;
}

void Command::setEnv(string * value) {
    if(env != NULL){
        delete env;
        env = NULL;
    }
    env = value;
}

string * Command::getParams() {
    return params;
}

pid_t Command::getPid() const {
    return pid;
}

void Command::setPid(pid_t pid) {
    Command::pid = pid;
}

bool Command::isAlarmEnabled() {
    return alarmEnabled;
}

void Command::setAlarmEnabled(bool alarmEnabled) {
    Command::alarmEnabled = alarmEnabled;
}

void Command::setParams(string * value) {
    if(params != NULL){
        delete params;
        params = NULL;
    }
    params = value;
}

string Command::execute() {
    // If we have a name and an environment
    if(name != NULL && env != NULL){
        // If the environment is not empty
        if(!(*env).empty()) {
            string cmd;
            // If there is parameters
            if(params!=NULL){
                // We create the command
                cmd = *env + "|" + *name + " " + *params;
            } else{
                cmd = *env + "|" + *name;
            }
            array<char, 128> buffer;
            string result;
            // We create a pipe process to save a shared_ptr of FILE
            shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
            if (!pipe) throw runtime_error("popen() failed!");
            // We copy the content of the command output from the buffer to the result string
            while (!feof(pipe.get())) {
                if (fgets(buffer.data(), 128, pipe.get()) != NULL)
                    result += buffer.data();
            }
#ifdef DEBUG
            cout << result << endl;
#endif
            // We return the result
            return result;
        } else{
            string cmd;
            if(params!=NULL){
                cmd = *name + " " + *params;
            } else{
                cmd = *name;
            }            array<char, 128> buffer;
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
    // If failed
    return NULL;
}

#include "Main.h"

void Command::executeWithExecve() {
    cout << "Begin of execve with code " << endl;
    if(Command::alarmEnabled) {
        signal(SIGALRM, Main::signalHandler);
        alarm(5);
    }
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
#ifdef DEBUG
    cout << "Deleting path in Command" << endl;
#endif
    if(path != NULL){
        delete path;
        path = NULL;
    }
#ifdef DEBUG
    cout << "path deleted in command" << endl;
    cout << "Deleting name in Command" << endl;
#endif
    if(name != NULL){
        delete name;
        name = NULL;
    }
#ifdef DEBUG
    cout << "name deleted in command" << endl;
    cout << "Deleting env in Command" << endl;
#endif
    if(env != NULL){
        delete env;
        env = NULL;
    }
#ifdef DEBUG
    cout << "env deleted in command" << endl;
    cout << "Deleting params in Command" << endl;
#endif
    if(params != NULL){
        delete params;
        params = NULL;
    }
#ifdef DEBUG
    cout << "params deleted in command" << endl;
#endif
}