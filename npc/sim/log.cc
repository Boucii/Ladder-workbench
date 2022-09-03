#include "log.h"

ofstram fout("../build/log/npc_log.txt",ios::out);

void Log(string content){
    fout<<content<<endl;
}


void LogInit(){
  assert(fout.isopen());
}
