#include <iostream>
#include "minNoOfCoins.cpp"

int main() {
    int coins[] {1,3,5,200};
    int sum=201, length=4;

    minNoOfCoins minNoOfCoins_t;
    std::cout << minNoOfCoins_t.findMinCoins(coins, length, sum);

}
