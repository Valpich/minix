/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _CONTEXTSAVER_H
#define _CONTEXTSAVER_H

#include <setjmp.h>

class ContextSaver {
public: 
    
    jmp_buf getJumpBuffer();
    
    /**
     * @param value
     */
    void setJumpBuffer(jmp_buf value);
private: 
    jmp_buf jumpBuffer;
};

#endif //_CONTEXTSAVER_H
