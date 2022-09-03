#include "log.h"

void LogInit(){
  ofstream fout("../build/log/npc_log.txt",ios::out);
  assert(fout.isopen());
}
void Log(string content){
    fout<<content<<endl;
}

