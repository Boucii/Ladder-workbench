#include "log.h"

ofstream fout("../build/log/npc_log.txt",ios::out);

void LogInit(){
  assert(fout.is_open());
}
void Log(string content){
    fout<<content<<endl;
    sync();
}
