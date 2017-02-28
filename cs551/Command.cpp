/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Command.h"

/**
 * Command implementation
 */

#include "CommandFinder.h"

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

void Command::setRunning(bool running) {
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
    // We fork the child process
    if ((pid = fork()) == 0) {
        pid = getpid();
#ifdef DEBUG
        cout << "pid is " << pid << endl;
#endif
        const char *fileName = generateFileName();
#ifdef DEBUG
        cout << "Executed fileName " << fileName << endl;
#endif
        // We set off the output if alarm enabled
        if (alarmEnabled) {
            close(pipefd[0]);    // close reading end in the child
            dup2(pipefd[1], 1);  // send stdout to the pipe
            dup2(pipefd[1], 2);  // send stderr to the pipe
            close(pipefd[1]);
        }
        // We execute the command
        cout<<(*params).c_str()<<endl;
        int i = execlp(fileName, (*params).c_str(), NULL);
        cout << "Failed to execute command with code" <<endl;
    } else {
        // We set as running the command
        setRunning(true);
        // We launch the command if required
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
        // We wait the child to return
        pid = wait(&status);
#ifdef DEBUG
        cout << "PID WAIT EXIT" << endl;
#endif
        // We set off the alarm
        if (alarmEnabled) {
#ifdef DEBUG
            cout << "ALARM DISABLED" << endl;
#endif
            alarm(0);
        }
        // We say the program as not runnable
        setRunning(false);
    }
}

const char *Command::generateFileName() {
#ifndef DEBUG_ALARM
    CommandFinder cf;
    string *tmp = getName();
#ifdef DEBUG
    cout << '\r' << endl;
    cout << *tmp << endl;
#endif
    return tmp->c_str();
#endif
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
    char *toEncodePoint = new char[getName()->size() + 1];
    toEncodePoint[getName()->size()] = '\0';
    copy(getName()->begin(), getName()->end(), toEncodePoint);
    char *const paramListND[2] = {toEncodePoint, NULL};
#ifdef DEBUG
    cout << toEncodePoint << endl;
#endif
    return paramListND;
}

char *const *Command::generateEnv() {
    // TODO:  return a env list like that: char *const envParms[2] = {"EXAMPLE=test", NULL};
    CommandFinder cf;
    char *toEncodePoint2 = new char[cf.getEnvPath().size() + 1];
    toEncodePoint2[cf.getEnvPath().size()] = '\0';
    copy(cf.getEnvPath().begin(), cf.getEnvPath().end(), toEncodePoint2);
    //  char *const envParamsND[2] = {toEncodePoint2, NULL};
    char *const envParams[] = {NULL};
    return envParams;
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
#ifdef CLEAN
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
#endif
}
