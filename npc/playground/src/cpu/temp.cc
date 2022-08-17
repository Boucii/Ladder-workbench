#include <iostream>
#include <svdpi.h>

using namespace std;

extern "C" int CheckEnd(svLogic a) {
    int res=0;
    if(a==1){ //end of simulation
        res=1;
    }
  cout << "End Of Simulation" << endl;
  cout << "Scope is " << svGetNameFromScope(svGetScope()) << endl;
  return res;
}
