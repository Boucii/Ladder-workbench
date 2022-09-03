#include "log.h"

ofstram fout;

void Log(string content){
    fout<<content<<endl;
}


void LogInit(){
  fout.open("../build/log/npc_log.txt",ios::out);
  assert(fout.isopen());
}
