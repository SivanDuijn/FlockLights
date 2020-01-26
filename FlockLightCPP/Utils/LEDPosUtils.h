#ifndef LEDPOSUTILS_H
#define LEDPOSUTILS_H

#include <climits>
#include <vector>
#include <string>
#include <sstream>  
#include "Vector3.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include "Utils.h"

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


	void putLEDPositionsInRelativeSpace(Vector3 *ledspos, Vector3 boxSize, int amountLEDS) {
		int minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, 
			maxX = 0, maxY = 0, maxZ = 0;
		for (int i=0; i<amountLEDS; i++) {
			minX = min(minX, (int)ledspos[i].x); maxX = max(maxX , (int)ledspos[i].x);
			minY = min(minY, (int)ledspos[i].y); maxY = max(maxY , (int)ledspos[i].y);
			minZ = min(minZ, (int)ledspos[i].z); maxZ = max(maxZ , (int)ledspos[i].z);
		}
		maxX -= minX;
		maxY -= minY;
		maxZ -= minZ;
		// the ratio of maxX maxY and maxZ should not change, else speed will vary across axis, right?
		// stretch all the axis with same modifier !
		int MaxAxis = max(max(maxX, maxY),maxZ) * 1.5;
		cout << MaxAxis << endl;
		float stretch = (boxSize.x) / (float)MaxAxis;
		cout << maxX << " " << maxY << " " << maxZ << endl;
		// values to use so the irl ledspos are in the middle of the virtual boxSize
		int extraXAxis = (1.0 - (float)maxX / MaxAxis) / 2 * boxSize.x;
		int extraYAxis = (1.0 - (float)maxY / MaxAxis) / 2 * boxSize.y;
		int extraZAxis = (1.0 - (float)maxZ / MaxAxis) / 2 * boxSize.z;
		cout << extraXAxis << " " << extraYAxis << " " << extraZAxis << " " << endl;
		// should really change boxSize axis ratios so that it matches the irl ratios
		
		for (int i=0; i<amountLEDS; i++) {
			ledspos[i].x -= minX;
			ledspos[i].y -= minY;
			ledspos[i].z -= minZ;
			cout << ledspos[i].toStr() << "  ";
			
			ledspos[i].x = extraXAxis + ledspos[i].x * stretch;
			ledspos[i].y = extraYAxis + ledspos[i].y * stretch;
			ledspos[i].z = extraZAxis + ledspos[i].z * stretch;
			cout << ledspos[i].toStr() << endl;
		}
	}
}

#endif