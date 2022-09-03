#include "log.h"

ofstram fout("../build/log/npc_log.txt",ios::out);
void LogInit(){
  assert(fout.isopen());
}
void Log(string content){
    fout<<content<<endl;
}

