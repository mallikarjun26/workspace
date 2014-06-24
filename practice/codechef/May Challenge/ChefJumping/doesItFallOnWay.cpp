#include <iostream>

using namespace std;

bool doesItFallOnWay(unsigned long long int stepNumber) {

    if( stepNumber == 0 ) {
        return true;
    }
    else {
        int tempModulo = stepNumber%6;
        cout << "Modulo:" << tempModulo << endl;
        if( (tempModulo == 0) || (tempModulo == 1) || (tempModulo == 3) )   return true;
        else    return false;
    }

}
