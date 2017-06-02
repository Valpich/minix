# minix_project_1

CS 551 Project 1: Develop your own shell

Requirements:

Your shell shall be invoked from the Ash shell provided with Minix.

Your shell shall first execute a PROFILE file which defines the PATH variable 
that will allow you to access programs provided in /bin and /usr/bin. 

Once the PROFILE file is executed, you will be in a HOME directory specified by you in the PROFILE file. 

The PATH and HOME variables do not replace those of the Ash shell from which your shell is instantiated. 

In a command line of your shell you will be able to exercise any executable programs including 
the utilities provided in /bin and /usr/bin. 

Your shell will set an alarm which fires 5 seconds after it has launched a command. 

After the alarm fired, your shell will ask the user whether he/she wants to terminate 
the command and will terminate the command if the user approves. 

The user can turn on and off this feature in the PROFILE file or in a command.

Your shell will remember the commands that a user had entered. 

In the future, when the user wants to enter a command again, 
she needs to type only the first few characters and then the Tab key, 
your shell will fill in the rest of the command line. 

If she does not like that suggestion, she can use the up and down arrow keys 
to look for other suggestions from your shell. 

If she types any other key, your shell will stop suggesting until the next command. 

If she modifies the suggested command, your shell will remember that too. 

The memory of past commands survives after the shell exits.

Your shell shall support a command line with parentheses and the sequence 
and parallel execution operators (“;” and “&”). 

A command line is a command or a set of commands connected by the operators. 

A pair of parentheses encloses a command line which can be treated as a commands. 

Therefore cmd&(cmd1;cmd2) is a valid command line specifying that cmd will be executed in parallel with (cmd1;cmd2).

Objectifs:

Créer sous Minix un shell semblable à celui déjà présent.
