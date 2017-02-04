/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "ContextSaver.h"

/**
 * ContextSaver implementation
 */


/**
 * @return jmp_buf
 */
jmp_buf ContextSaver::getJumpBuffer() {
    return jumpBuffer;
}

/**
 * @param value
 */
void ContextSaver::setJumpBuffer(jmp_buf value) {
	jumpBuffer = value;
}

ContextSaver::ContextSaver(void){

}

ContextSaver::~ContextSaver(void){
	
}