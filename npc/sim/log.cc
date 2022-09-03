#include "log.h"
#include <fstream>
using namespace std;
void LogInit(){
  ofstream fout("../build/log/npc_log.txt",ios::out);
  assert(fout.isopen());
}
void Log(string content){
    fout<<content<<endl;
}

