#include "minNoOfCoins.h"
#include <limits>
#include <iostream>

minNoOfCoins::minNoOfCoins()
{

}

int minNoOfCoins::findMinCoins(int coins[], int noOfCoins, int sum) {
    int minValue[sum+1];
    //ctor

    for(int j=0; j<noOfCoins; j++) {
        std::cout << "coins: " << coins[j] << std::endl;
    }

    std::cout << " NoOfCoins: " << noOfCoins << " sum:" << sum << std::endl;

    for(int i=0; i<sum+1; i++) {
        minValue[i] = 32000;
    }

    minValue[0] = 0;

    for(int i=1; i<sum+1; i++) {
        for(int j=0; j<noOfCoins; j++) {
            if ( (coins[j]<= i) && ( (minValue[i-coins[j]] + 1 ) < minValue[i]))
                minValue[i] = minValue[i - coins[j]] + 1;
                std::cout << "Inside Loop minValue for " << i << ": " << minValue[i] << std::endl;
        }
    }

    for(int i=0; i<sum+1; i++) {
        std::cout << "Min Value for " << i << " :" << minValue[i] << std::endl;
    }


    std::cout << "Local Value:" << minValue[sum] << std::endl;

    return(minValue[sum]);
}

minNoOfCoins::~minNoOfCoins()
{
    //dtor
}
