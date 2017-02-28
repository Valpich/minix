
/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */

#include "Shell.h"

/**
 * Shell implementation
 */

const string Shell::name = "SHELL NAME";

/**
 * @return Profile
 */
Profile *Shell::getProfile() {
    return profile;
}

/**
 * @param value
 */
void Shell::setProfile(Profile *value) {
    if (profile != NULL) {
        delete profile;
        profile = NULL;
    }
    profile = value;
}

/**
 * @return History
 */
History *Shell::getHistory() {
    return history;
}

/**
 * @param value
 */
void Shell::setHistory(History *value) {
    if (history != NULL) {
        delete history;
        history = NULL;
    }
    history = value;
}

/**
 * @return vector<Command>
 */
vector<Command *> *Shell::getCommandList() {
    return commandList;
}

/**
 * @param value
 */
void Shell::setCommandList(vector<Command *> *value) {
    if (commandList != NULL) {
        delete commandList;
        commandList = NULL;
    }
    commandList = value;
}

/**
 * @return CommandFinder
 */
CommandFinder *Shell::getCommandFinder() {
    return commandFinder;
}

/**
 * @param value
 */
void Shell::setCommandFinder(CommandFinder *value) {
    if (commandFinder != NULL) {
        delete commandFinder;
        commandFinder = NULL;
    }
    commandFinder = value;
}

const string &Shell::getName() {
    return name;
}

Command *Shell::getCommand() const {
    return command;
}

void Shell::setCommand(Command *value) {
    if (command != NULL) {
        delete command;
        command = NULL;
    }
    command = value;
}

void returnCMD(string command, int code)
{
    string temp = command;
    string token = " ";
    string name = "";
    string param = "";
    string env = "";
    int cnt = 0;
    size_t found;
    bool aa = false;

    if(code == 0){
      while((found = temp.find(""))!=std::string::npos && aa == false){
        cnt++;
        string cmp = temp.substr(0, temp.find(" "));
        temp = temp.substr(temp.find(" ")+1);
        if(cmp == temp){
          aa = true;
        }
      }

      switch(cnt)
      {
        case 1:
          name = command.substr(0, command.find(token));
          break;
        case 2:
          name = command.substr(0, command.find(token));
          command = command.substr(command.find(token)+1);
          param = command.substr(0, command.find(token));
          break;
        case 3:
          name = command.substr(0, command.find(token));
          command = command.substr(command.find(token)+1);
          param = command.substr(0, command.find(token));
          env = command.substr(command.find(token)+1);
          break;
        default:
          break;
      }


      Command *command = new Command();
      command->setEnv(new string(env));
      command->setName(new string(name));
      command->setParams(new string(param));
      command->executeWithExecve();
  }

}

void parseCommand(string chaine)
{
  string token;
  string otherToken;
  string tmp;
  string state;

  size_t found;

  int cntR = 0;
  int cntL = 0;

  bool ok = true;

  while(ok)
  {
    if((found = chaine.find_first_of("()"))!=string::npos)
    {

      if((tmp = chaine[found]) == "(")
      {
        token = "(";
        otherToken = ")";
        cntR++;
      }else{
        otherToken = "(";
        token = ")";
        cntL++;
      }

      if(cntR < cntL)
      {
        state = "ERROR";
        ok = false;
      }

      string cmd1 = chaine.substr(0, chaine.find(token));
      chaine = chaine.substr(chaine.find(token)+1);
      if(chaine == chaine.substr(chaine.find(otherToken)+1) && chaine == "")
      {
        if(cntR == cntL)
        {
          state = "OK";
          ok = false;
          cout << "ERROR WITH YOUR PARENTHESIS" << endl;
        }

      }
    }
    else
    {
      ok = false;
      if(cntR != cntL)
      {
        state = "ERROR";
      }
    }
  }

  if(state != "ERROR")
  {
    if(cntR == 0 && cntL == 0)
    {
      if((found = chaine.find_first_of("&;"))==string::npos){
        returnCMD(chaine, 0);
      }
      while((found = chaine.find_first_of("&;"))!=string::npos)
      {
        if((tmp = chaine[found]) == ";")
        {
          token = ";";
          otherToken = "&";
          }else{
            otherToken = ";";
            token = "&";
          }
          string cmd1 = chaine.substr(0, chaine.find(token));
          chaine = chaine.substr(chaine.find(token)+1);

          returnCMD(cmd1, 0);

          if(chaine == chaine.substr(chaine.find(token)+1)){
            if(chaine == chaine.substr(chaine.find(otherToken)+1))
            {
              returnCMD(chaine, 0);
            }
          }
      }
    }
  }else{
    cout << "ERROR WITH THE COMMAND" << '\n';
  }
}

bool Shell::run() {
    History *history = new History();
    vector<Command *> *commands = new vector<Command *>();
    history->getCommandHistory(commands);
    vector<string> substrings;
    bool exit = false;
    while (!exit) {
        bool scanning = true;
        while (scanning) {
            int c;
            // i is used to iterate throught the substrings
            int i = 1;
            string ch;
            /* Start curses mode */
            initscr();
            //One-character-a-time.
            cbreak();
            //No echo.
            noecho();
            //Special keys. In order to capture special keystrokes like Backspace, Delete and the four arrow keys by getch()
            keypad(stdscr, TRUE);
            cout << "Please, enter the command: " << '\r' << endl;
            string commandLine = "";
            bool suggestingMode = false;
            while (scanning) {
                c = getch();
                switch (c) {
                    case ESC_PRESSED:
                        suggestingMode = false;
                        scanning = false;
                        exit = true;
#ifdef DEBUG
                        cout << "Esc pressed" << '\r' << endl;
#endif
                        break;
                    case ENTER_PRESSED:
                        suggestingMode = false;
                        scanning = false;
                        parseCommand(commandLine);
                        // TODO: Create a command vector
                        // TODO: Parse command line into command vector
                        // TODO: Execute each command
#ifdef DEBUG
                        cout << "Enter pressed" << '\r' << endl;
#endif
                        break;
                    case TAB_PRESSED:
                        //TODO: Auto complete
                        suggestingMode = true;
                        for (Command *command : *commands) {
                            string value = *command->getName();
#ifdef DEBUG
                            cout << "this is value : " << value << endl;
                            cout << "this is commandLine : " << commandLine << endl;
#endif
                            if (value.find(commandLine) != string::npos) {
#ifdef DEBUG
                                cout << "new value found - adding " << endl;
                                cout << value << endl;
#endif
                                // checking if already there
                                if (find(substrings.begin(), substrings.end(), value) != substrings.end()) {
#ifdef DEBUG
                                    cout << "key already present" << endl;
#endif
                                } else {
                                    substrings.push_back(value);
                                }
                            }
                        }
                        commandLine.assign(substrings[0]);
#ifdef DEBUG
                        cout << " this is the value assigned " << commandLine << endl;
                        cout << "Tab pressed" << '\r' << endl;
#endif
                        break;
                    case UP_ARROW_PRESSED:
                        if (suggestingMode) {
                            if (i > 0) {
                                commandLine.assign(substrings[i - 1]);
                                i = i - 1;
                                cout <<'\r' << endl;
                                cout << "new suggestion found : "<<'\r'<<endl;
                                cout << commandLine << flush;
                            }
#ifdef DEBUG
                            cout << "Find previous suggestion" << '\r' << endl;
#endif
                        }
                        break;
                    case DOWN_ARROW_PRESSED:
                        if (suggestingMode) {
                            if (i < substrings.size()) {
                                commandLine.assign(substrings[i]);
                                i = i + 1;
                                cout <<'\r' << endl;
                                cout << "new suggestion found : "<<'\r' <<endl;
                                cout << commandLine << flush;
                            }
#ifdef DEBUG
                            cout << "Find next suggestion" << '\r' << endl;
#endif
                        }
                        break;
                    case LEFT_ARROW_PRESSED:
                        suggestingMode = false;
#ifdef DEBUG
                        cout << "Left arrow pressed" << '\r' << endl;
#endif
                        cout << '\r' << endl;
                        if (!commandLine.empty()) {
                            ch += commandLine.back();
                            commandLine = commandLine.substr(0, commandLine.size() - 1);
                        }
                        cout << commandLine << flush;
                        break;
                    case RIGHT_ARROW_PRESSED:
                        suggestingMode = false;
#ifdef DEBUG
                        cout << "Right arrow pressed" << '\r' << endl;
#endif
                        cout << '\r' << endl;
                        if (!ch.empty()) {
                            reverse(ch.begin(), ch.end());
                            commandLine += ch.at(0);
                            ch.erase(0, 1);
                            reverse(ch.begin(), ch.end());
                        }
                        cout << commandLine << flush;
                        break;
                    case DELETE_PRESSED:
                        suggestingMode = false;
#ifdef DEBUG
                        cout << "Delete pressed" << '\r' << endl;
#endif
                        cout << '\r' << endl;
                        if (!commandLine.empty()){
                            commandLine = commandLine.substr(0, commandLine.size() - 1);
                        }

                        cout << commandLine << flush;
                        break;
                    case DELETE_BIS_PRESSED:
                        suggestingMode = false;
#ifdef DEBUG
                        cout << "Delete pressed" << '\r' << endl;
#endif
                        cout << '\r' << endl;
                        if (!commandLine.empty()){
                            commandLine = commandLine.substr(0, commandLine.size() - 1);
                        }

                        cout << commandLine << flush;
                       break;
                    default:
                        suggestingMode = false;
#ifdef DEBUG
                        cout << static_cast<char>(c) << " char pressed" << '\r' << endl;
                        cout << c << " int pressed" << '\r' << endl;
#endif
                        if (c >= 32 && c <= 127) {// The first ascii char
                            char cToChar = static_cast<char>(c);
                            commandLine += cToChar;

                            reverse(ch.begin(), ch.end());
                            ch.erase(0, 1);
                            commandLine.append(ch);
                            cout << cToChar << flush;
                        }
                        break;
                }
                cbreak();
            }
            endwin();
            if (!exit) {
#ifdef DEBUG
                cout << "Scan of " << commandLine << " done!" << '\r' << endl;
#endif
                // TODO: Get the command and set it as the Command value in Shell
                // TODO: Execute the command from command
            }
        }
    }
    return exit;
}

ostream &operator<<(ostream &os, const Shell &shell) {
    os << " profile: ";
    if (shell.profile == NULL) {
        os << "NULL";
    } else {
        os << *shell.profile;
    }
    os << " command: ";
    if (shell.command == NULL) {
        os << "NULL";
    } else {
        os << *shell.command;
    }
    os << " history: ";
    if (shell.history == NULL) {
        os << "NULL";
    } else {
        os << *shell.history;
    }
    os << " commandList: ";
    if (shell.commandList == NULL) {
        os << "NULL";
    } else {
        for (Command *cmdTmp: *shell.commandList) {
            if (cmdTmp == NULL) {
                os << "NULL";
            } else {
                os << *cmdTmp;
            }
        }
    }
    os << " commandFinder: ";
    if (shell.commandFinder == NULL) {
        os << "NULL";
    } else {
        os << *shell.commandFinder;
    }
    return os;
}

Shell::Shell(void) {
    profile = new Profile();
    history = new History();
    command = NULL;
    commandFinder = new CommandFinder();
    commandFinder->setProfile(profile);
    commandList = new vector<Command *>();
    commandFinder->findAllCommands(commandList);
#ifdef DEBUG
    cout << "Command list in shell is :" << endl;
    for (Command *commandTmp : *commandList) {
        cout << *commandTmp << endl;
    }
    cout << "Profile content in shell is :" << endl;
    for (string stringTmp : *profile->getContent()) {
        cout << stringTmp << endl;
    }
#endif

}

Shell::~Shell(void) {
#ifdef CLEAN
#ifdef DEBUG
    cout << "Deleting commandFinder in shell" << endl;
#endif
    if (commandFinder != NULL) {
        delete commandFinder;
        commandFinder = NULL;
        profile = NULL;
    }
#ifdef DEBUG
    cout << "commandFinder deleted in shell" << endl;
    cout << "Deleting profile in shell" << endl;
#endif
    if (profile != NULL) {
        delete profile;
        profile = NULL;
    }
#ifdef DEBUG
    cout << "profile deleted in shell" << endl;
    cout << "Deleting history in shell" << endl;
#endif
    if (history != NULL) {
        delete history;
        history = NULL;
    }
#ifdef DEBUG
    cout << "history deleted in shell" << endl;
    cout << "Deleting commandList in shell" << endl;
#endif
    if (commandList != NULL) {
        delete commandList;
        commandList = NULL;
    }
#ifdef DEBUG
    cout << "commandList deleted in shell" << endl;
    cout << "Deleting command in shell" << endl;
#endif
    if (command != NULL) {
        delete command;
        command = NULL;
    }
#ifdef DEBUG
    cout << "command deleted in shell" << endl;
#endif
#endif
}
