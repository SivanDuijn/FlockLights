#include <fstream>
#include <iostream>
#include <string.h>

#include "../../Utils/LEDPosUtils.h"

#define DEFAULT_FILENAME "test.txt"

using namespace std;

int main (int argc, char *argv[])
{
    string filename = DEFAULT_FILENAME;
    if (argc < 2)
        cout << "no filename given, using default " << DEFAULT_FILENAME << endl;
    else
        filename = argv[1];

    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            // cout << line.c_str() << endl;
            
            vector<string> words = LEDPosUtils::split(line.c_str(), ' ');
            for (int i=0; i<words.size(); i++)
                cout << words[i] << endl;
            cout << endl;
        }
        file.close();
    }
    else 
        cout << "That file does not exist!" << endl;
}
