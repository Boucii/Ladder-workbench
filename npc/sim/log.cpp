#include "log.h"

ofstream fout("./build/log/npc_log.txt",ios::out);

void LogInit(){
  cout<<"Current path is:"<<getcwd(NULL,0)<<'\n';
  assert(fout.is_open());
  cout<<"log file is open now\n"
}
void Log(string content){
    fout<<content;
    sync();
}
