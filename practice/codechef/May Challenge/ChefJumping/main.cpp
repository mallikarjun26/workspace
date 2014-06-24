#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <limits.h>
#include <tgmath.h>

#include "doesItFallOnWay.hpp"

using namespace std;

int main(int argc,char *argv[]) {
    string line;
    unsigned long long int stepNumber;
    bool itDoesFall;
    char * nullPointer;
    ifstream inputFile;
    ofstream outputFile;

    inputFile.open(argv[1]);
    outputFile.open(argv[2]);

    //inputFile.open("/home/mallikarjun/Documents/codechef/May Challenge/chefJumpingInput.txt");
    //outputFile.open("/home/mallikarjun/Documents/codechef/May Challenge/chefJumpingOutput.txt");

    if(inputFile.is_open()) {
        while(getline(inputFile, line)) {
            //stepNumber = getULLint(line);
            stepNumber = strtoull(line.c_str(), &nullPointer, 10);
            //cout << "DEBUG XXXX:     " << stepNumber << endl;
            //cout << "DEBUG YYYY:     " << line << endl;
            cout << "For the postion:" << line << " :: ";
            itDoesFall = doesItFallOnWay(stepNumber);

            if(itDoesFall == true) {
                cout << "yes" << endl;
                outputFile << "yes" << endl;
            }
            else {
                cout << "no" << endl;
                outputFile << "no" << endl;
            }
        }
        inputFile.close();
        outputFile.close();
    }
    else cout << "Unable to open the file";

    return 0;
}
