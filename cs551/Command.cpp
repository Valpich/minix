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

string *Command::getPath() {
    return path;
}

void Command::setPath(string *value) {
    if (path != NULL) {
        delete path;
        path = NULL;
    }
    path = value;
}

string *Command::getName() {
    return name;
}

void Command::setName(string *value) {
    if (name != NULL) {
        delete name;
        name = NULL;
    }
    name = value;
}

string *Command::getEnv() {
    return env;
}

void Command::setEnv(string *value) {
    if (env != NULL) {
        delete env;
        env = NULL;
    }
    env = value;
}

string *Command::getParams() {
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

bool Command::isRunning() {
    return running;
}

void Command::setRunning(bool running){
    Command::running = running;
}

void Command::setParams(string *value) {
    if (params != NULL) {
        delete params;
        params = NULL;
    }
    params = value;
}

string Command::execute() {
    // If we have a name and an environment
    if (name != NULL && env != NULL) {
        // If the environment is not empty
        if (!(*env).empty()) {
            string cmd;
            // If there is parameters
            if (params != NULL) {
                // We create the command
                cmd = *env + "|" + *name + " " + *params;
            } else {
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
        } else {
            string cmd;
            if (params != NULL) {
                cmd = *name + " " + *params;
            } else {
                cmd = *name;
            }
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
    // If failed
    return NULL;
}

void Command::executeWithExecve() {
#ifdef DEBUG
    cout << "Begin of execve with code " << endl;
#endif
    bool alarmEnabled = Command::alarmEnabled;
    int pipefd[2];
    if (alarmEnabled) {
        pipe(pipefd);
    }
    int status;
    if ((pid = fork()) == 0) {
        pid = getpid();
#ifdef DEBUG
        cout << "pid is " << pid << endl;
#endif
        const char * fileName = generateFileName();
        char *const * generatedParams = generateParams();
        char *const * generatedEnv = generateEnv();
        cout <<"Executed fileName "<<fileName << endl;
        if (alarmEnabled) {
            close(pipefd[0]);    // close reading end in the child
            dup2(pipefd[1], 1);  // send stdout to the pipe
            dup2(pipefd[1], 2);  // send stderr to the pipe
            close(pipefd[1]);
        }
        int i = execve(fileName, generatedParams, generatedEnv);
#ifdef DEBUG
        cout << "End of execve with code " << i << endl;
        cout << "Return not expected. Must be an execve error.n" << endl;
#endif
    } else {
        setRunning(true);
        if (alarmEnabled) {
#ifdef DEBUG
            cout << "ALARM STARTED" << endl;
#endif
            alarm(5);
        }
        char buffer[1024];

        if (alarmEnabled) {
            close(pipefd[1]);  // close the write end of the pipe in the parent

            while (read(pipefd[0], buffer, sizeof(buffer)) != 0) {}
        }
        pid = wait(&status);
#ifdef DEBUG
        cout<<"PID WAIT EXIT" <<endl;
#endif
        if (alarmEnabled) {
#ifdef DEBUG
            cout << "ALARM DISABLED" << endl;
#endif
            alarm(0);
        }
        setRunning(false);
    }
}

const char *Command::generateFileName() {
    //TODO: return the filename of the file that contains the executable image of the new process
#ifdef DEBUG_ALARM
    char buffer[300];
    char *answer = getcwd(buffer, sizeof(buffer));
    string currentPathToString;
    if (answer) {
        currentPathToString = answer;
        remove(currentPathToString.begin(), currentPathToString.end(), ' ');
    }
    currentPathToString += "/main_loop";
#ifdef TEST
    return "/bin/ls";
#endif
    return currentPathToString.c_str();
#endif
}

char *const *Command::generateParams() {
    // TODO:  return a param list like that: char *const parmList[] = {"/bin/ls", "-l", "/u/userid/dirname", NULL};
#ifdef TEST
    char *const paramList[] = { "-al",NULL};
    return paramList;
#endif
    return NULL;
}

char *const *Command::generateEnv() {
    // TODO:  return a env list like that: char *const envParms[2] = {"EXAMPLE=test", NULL};
#ifdef TEST
    char *const envParams[2] = {"EXAMPLE=test", NULL};
    return envParams;
#endif
    return NULL;
}

ostream &operator<<(ostream &os, const Command &command) {
    os << "path: " << (command.path == NULL ? "NULL" : *command.path) << " name: "
       << (command.name == NULL ? "NULL" : *command.name) << " env: " << (command.env == NULL ? "NULL" : *command.env)
       << " params: "
       << (command.params == NULL ? "NULL" : *command.params);
    return os;
}

Command::Command(void) {
    path = NULL;
    name = NULL;
    env = NULL;
    params = NULL;
}

Command::~Command(void) {
#ifdef DEBUG
    cout << "Deleting path in Command" << endl;
#endif
    if (path != NULL) {
        delete path;
        path = NULL;
    }
#ifdef DEBUG
    cout << "path deleted in command" << endl;
    cout << "Deleting name in Command" << endl;
#endif
    if (name != NULL) {
        delete name;
        name = NULL;
    }
#ifdef DEBUG
    cout << "name deleted in command" << endl;
    cout << "Deleting env in Command" << endl;
#endif
    if (env != NULL) {
        delete env;
        env = NULL;
    }
#ifdef DEBUG
    cout << "env deleted in command" << endl;
    cout << "Deleting params in Command" << endl;
#endif
    if (params != NULL) {
        delete params;
        params = NULL;
    }
#ifdef DEBUG
    cout << "params deleted in command" << endl;
#endif
}