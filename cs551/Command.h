/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMAND_H
#define _COMMAND_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>


using namespace std;

class Command {
public:


    /**
     * Construct a Command instance
     */
    Command();

    /**
     * Destroy a Command instance
     */
    ~Command();

    /**
     * @return The path string
     */
    string *getPath();

    /**
     * Set the path string and remove the previous one if exists
     * @param value The new value of the path
     */
    void setPath(string *value);

    /**
     * @return The name string
     */
    string *getName();

    /**
     * Set the name string and remove the previous one if exists
     * @param value The new value of the name
     */
    void setName(string *name);

    /**
     * @return The env string
     */
    string *getEnv();

    /**
     * @param value
     */
    void setEnv(string *name);

    /**
     * @return The params string
     */
    string *getParams();

    /**
     * Set the params string and remove the previous one if exists
     * @param value The new value of the params
     */
    void setParams(string *params);

    pid_t getPid() const;

    /**
     * Set the new pid value of the executed command
     * @param pid The new value of the pid
     */
    void setPid(pid_t pid);

    /**
     * @return If the the alarm is enable return true else false
     */
    static bool isAlarmEnabled();

    /**
     * Set the new alarmEnabled value to say if a command should use an alarm or not
     * @param alarmEnabled The new value of the alarmEnabled
     */
    static void setAlarmEnabled(bool alarmEnabled);

    /**
     * Execute a the command with popen if possible and return
     * @return th
     */
    string execute();

    /**
     * Execute a the command with popen if possible and return the output of the command
     * @return
     */
    void executeWithExecve();

    /**
     * Overload the ostream operator to display as a string a Command
     */
    friend ostream &operator<<(ostream &os, const Command &command);

    bool isRunning();

    void setRunning(bool running);

private:
    string *path;
    string *name;
    string *env;
    string *params;

    pid_t pid;

    static bool alarmEnabled;

    bool running;

    const char *generateFileName();

    char *const *generateParams();

    char *const *generateEnv();
};

#endif //_COMMAND_H
