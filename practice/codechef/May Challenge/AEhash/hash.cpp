#include <iostream>
#include <math.h>

using namespace std;

int noOfAs(string input){
    int returnValue=0;
    for(int i=0; i<input.length(); i++){
        if(input.at(i)=='A')
            returnValue++;
    }
    return returnValue;
}

int maxOf(int a, int b){
    if(a>b)
        return a;
    else
        return b;
}

void split(string input, string &subInput1, string &subInput2){
    subInput1 = input.substr( 0 , input.size()/2 );
    subInput2 = input.substr( input.size()/2, ceil(float(input.size()/float(2))) );
}

int hash(string input) {
    int result=0;
    string subInput1, subInput2;

    result = noOfAs(input);
    split(input, &subInput1, &subInput2);
    if(input.length()>1) {
        result = result + maxOf( hash(subInput1), hash(subInput2));
    }

    return result;
}
