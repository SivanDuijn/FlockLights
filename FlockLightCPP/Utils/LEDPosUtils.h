#ifndef LEDPOSUTILS_H
#define LEDPOSUTILS_H

#include <vector>
#include <string>
#include <sstream>  
#include "Vector3.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include "Utils.h"
#include <limits>

using namespace std;

namespace LEDPosUtils {
    vector<string> split(const string& s, char delimiter)
	{
		vector<string> tokens;
		string token;
		istringstream tokenStream(s);
		while (getline(tokenStream, token, delimiter))
		{
			if (!token.empty())
				tokens.push_back(token);
		}
		return tokens;
	}

    Vector3* readLedsPosFromFile(string filename, int *ledsAmount) {
        int count = 0;
        ifstream filee(filename);
        if (filee.is_open()) {
            std::string line;
            while (getline(filee, line)) {
                if (line.c_str() != "") {
                    vector<string> words = LEDPosUtils::split(line.c_str(), ' ');
                    if (words.size() >= 3) 
                        count++;
                    else
                        cout << "skipping ledsPos entry: " << line.c_str() << "   incorrect syntax" << endl;
                }
            }
        }

        *ledsAmount = count;
        Vector3 *results = new Vector3[count];
        
        count = 0;
        ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (getline(file, line)) {
                if (line.c_str() != "") {
                    vector<string> words = LEDPosUtils::split(line.c_str(), ' ');
                    if (words.size() >= 3) {
                        results[count] = Vector3(stoi(words[0]), stoi(words[1]), stoi(words[2]));
                        count++;
                    }
                }
            }
            file.close();
        }
        else 
            cout << "That file does not exist!" << endl;

        return results;
    }


	Vector3 putLEDPositionsInBox(Vector3 *ledspos, int amountLEDS, int minAxisBoxSize, int extraSpaceOnSides) {
		Vector3 minV = Vector3(1000000000, 1000000000, 1000000000);
		Vector3 maxV = Vector3(0, 0, 0);

		for (int i=0; i<amountLEDS; i++) {
			minV.x = min((int)minV.x, (int)ledspos[i].x); maxV.x = max((int)maxV.x, (int)ledspos[i].x);
			minV.y = min((int)minV.y, (int)ledspos[i].y); maxV.y = max((int)maxV.y, (int)ledspos[i].y);
			minV.z = min((int)minV.z, (int)ledspos[i].z); maxV.z = max((int)maxV.z, (int)ledspos[i].z);
			
		}
		Vector3 box = maxV - minV;

		int smallestAxis = min((int)box.x, min((int)box.y, (int)box.z));
		float ratio = (float)minAxisBoxSize / (float)smallestAxis;
		// multiply ledspos axi with ratio to put them in virtual space

		for (int i=0; i<amountLEDS; i++) {
			ledspos[i] = (ledspos[i] - minV) * ratio + extraSpaceOnSides;
			cout << ledspos[i].toStr() << endl;
		}

		return box * ratio + (2 * extraSpaceOnSides);
	}
}

#endif