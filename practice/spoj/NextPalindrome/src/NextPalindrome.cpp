//============================================================================
// Name        : NextPalindrome.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <math.h>
using namespace std;

bool isPalindrome(int number) {

	//cout << "Debug 00:: Entered isPalindrome: " << number << endl;
	int numberOfDigits=0;
	int number_t = number;
	bool palindrome = true;
	int index=1, compIndex;

	while(number_t) {
		numberOfDigits++;
		number_t = number_t / 10;
	}
	compIndex = numberOfDigits;

	//cout << "Debug 00:: numberOfDigits:" << numberOfDigits << endl;

	while(index<=compIndex) {
		//cout << "Debug 00:: :: " << endl;
		int start = ( number % int(pow(10,index)) ) / int(pow(10,index-1));
		int end   = ( number % int(pow(10,compIndex)) ) / int(pow(10,compIndex-1));

		//cout << "Debug 00:: start:" << start << " end:" << end << endl;

		if(end != start) {
			palindrome = false;
			break;
		}

		index++;
		compIndex--;
		numberOfDigits--;
	}

	//cout << "Debug 00:: Leaving isPalindrome: " << palindrome << endl;

	return palindrome;

}

int findNextPalindrome(int number) {

	//cout << "Debug 11:: Entered findNextPalindrome: " << number << endl;

	int palindrome, number_t;
	int numberOfDigits=0;
	int index=1, compIndex;

	number_t = number;
	while(number_t) {
		numberOfDigits++;
		number_t = number_t / 10;
	}
	compIndex = numberOfDigits;

	//cout << "Debug 11:: numberOfDigits=" << numberOfDigits << endl;

	while(isPalindrome(number)==0) {

		//cout << "Debug 11:::: " << endl;

		int start = ( number % int(pow(10,index)) ) / int(pow(10,index-1));
		int end   = ( number % int(pow(10,compIndex)) ) / int(pow(10,compIndex-1));

		//cout << "Debug 11:: start: " << start << " end: " << end << endl;

		if(start < end) {
			number = number + ( (end-start) * pow(10, index-1) );
		}
		else if(start > end) {
			number = number + ( (10 - start) + end ) * pow(10, index-1) ;
		}

		start = ( number % int(pow(10,index)) ) / int(pow(10,index-1));
		end   = ( number % int(pow(10,compIndex)) ) / int(pow(10,compIndex-1));

		if(start == end){
			index++;
			compIndex--;
		}

	}

	//cout << "Debug 11:: Leaving findNextPalindrome: " << number << endl;

	return number;
}


int main() {
	int numberOfCases;
	int number;

	cin >> numberOfCases;

	while(numberOfCases--) {
		cin >> number;
		if(number == 0){
			cout << 1 << endl;
		}
		else {
			cout << findNextPalindrome(number+1) << endl;
		}
	}

	return 0;
}
