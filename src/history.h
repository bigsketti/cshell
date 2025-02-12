#ifndef HISTORY_H
#define HISTORY_H

char* history[500];

void createHistoryDir() {}; // create history in /home/user/cshell_history
void initHistory() {};  //  read history from file on start
void writeHistory() {}; //  write history at end of session
void accessHistory() {}; // handle user up arrow key to history

#endif