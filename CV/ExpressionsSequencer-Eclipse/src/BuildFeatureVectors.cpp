/*
 * BuildFeatureVectors.cpp
 *
 *  Created on: 07-Jul-2014
 *      Author: mallikarjun
 */

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

void buildFeatureVectors(const string outputLocation) {
	string globalFeatureVectorsName = outputLocation + "/AllFaces/Backup 6 July/featureVectors.txt";
	string listOfFacesName = outputLocation + "/AllFaces/ListOfFaces.txt";
	string featureVectorsName = outputLocation + "/AllFaces/featureVectors.txt";

	ifstream listOfFacesPtr(listOfFacesName.c_str());
	ifstream globalFeatureVectorsPtr(globalFeatureVectorsName.c_str());
	//ofstream featureVectorsPtr(featureVectorsName);

	string line;
    vector<string> stringVector;
	map<string, int> listOfFaces;

	// Build the map of faces for which feature vectors have to be found
	if(listOfFacesPtr.is_open()) {
		if(getline(listOfFacesPtr, line)) {
			try {
				split(stringVector, line, boost::is_any_of(" "));
				listOfFaces[stringVector[0].c_str()] = lexical_cast<int>(stringVector[1]);
            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }
		}
	}

	// Skim through the global feature vectors and cache it in local object before dumping into the file
	if(globalFeatureVectorsPtr.is_open()) {
		if(getline(globalFeatureVectorsPtr, line)) {
			try {
				split(stringVector, line, boost::is_any_of(""));
				listOfFaces[stringVector[0].c_str()] = lexical_cast<int>(stringVector[1]);
            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }
		}
	}

}


