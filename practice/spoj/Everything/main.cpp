#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    int c;
    cin >> c;
    while(c != 42) {
        cout << c << endl;
        cin >> c;
    }

    return 0;
}
