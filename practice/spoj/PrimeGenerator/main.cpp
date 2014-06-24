#include <iostream>
#include <math.h>
#include <inttypes.h>
#include <deque>

using namespace std;

deque<uint64_t> primeNumbers;
uint64_t largestPrimeNumber=2;

void buildPrimeNumberQueue(uint64_t b) {

    for(uint64_t i=largestPrimeNumber+1; i<b+1; i++) {
        bool boolPrimeNumber = true;

        //cout << "checking for " << i << endl;

        deque<uint64_t>::iterator it = primeNumbers.begin();

        while(it != primeNumbers.end()){
            uint64_t temp = *it++;
            //cout << ":::::"  << temp << endl;
            if(i%temp == 0) {
                boolPrimeNumber = false;
                break;
            }
        }

        if(boolPrimeNumber == true) {
            //cout << "building::" << i << endl;
            primeNumbers.push_back(i);
            largestPrimeNumber = i;
        }

    }
}

bool isPrime(uint64_t a){

    deque<uint64_t>::iterator it = primeNumbers.begin();

    if(a == 1){
        return false;
    }

    else{
        while((it != primeNumbers.end()) && (*it * *it <= a) ){
            uint64_t temp = *it;
            it++;
            if(a%temp==0)
                return false;
        }
    }
    return true;
}


int main() {
    primeNumbers.push_back((uint64_t)2);
    uint64_t t, a, b;
    bool p1=true;

    cin >> t;

    while (t--) {
        cin >> a;
        cin >> b;

        if(sqrt(b) > largestPrimeNumber)
            buildPrimeNumberQueue(sqrt(b));

        for( ; (a%6) != 0 ; a++) {
            //cout << "debug " << a << endl;
            if(isPrime(a)){
                cout << a << endl;
            }
        }
        a++;

        while(a<=b) {
            //cout << "debug 1 " << a << endl;
            if(isPrime(a)){
                cout << a << endl;
            }

            if(p1 == true){
                a = a + 4;
                p1 = false;
            }
            else {
                a = a + 2;
                p1 = true;
            }

        }

    }
    return 0;

}
