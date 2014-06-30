#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

using namespace std;
using namespace boost;

void findEdgeWeights(const string outputLocation) {
    string featureVectorsTxtName = outputLocation + "/AllFaces/featureVectors.txt";
    ifstream featureVectorHandle(featureVectorsTxtName.c_str());
    vector<string> stringVector;
    string line;
    int noOfVertices;

    if(featureVectorHandle.is_open()) {
        if(getline(featureVectorHandle, line)) {
            try {
                noOfVertices = lexical_cast<int>(line);
                vector<float> featureVector;
            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }
        }

        while(getline(featureVectorHandle, line)) {
            split(stringVector, line, boost::is_any_of("^=:"));
            try {

            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }
        }

    }
    else {
        cout << "Couldn't open " << edgeWeightsTxtName << endl;
    }

}
