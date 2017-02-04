/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _SIGNALHANDLER_H
#define _SIGNALHANDLER_H

#include <iostream>
#include <csignal>

class SignalHandler {
public: 
    
    SignalHandler();
    
	~SignalHandler();

    void signalHandler(int signum);
};

#endif //_SIGNALHANDLER_H