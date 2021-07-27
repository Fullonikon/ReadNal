#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

string readNal(string input, int startPosition, int &size)
{
    string nal;
    string nalSize1;
    for (int i = startPosition; i < startPosition + 8; i++)
    {
        nalSize1 += input[i];
    }
    size = stoi(nalSize1, 0, 16)*2;
    nal += nalSize1;
    nal = input.substr(startPosition, size+8);

    return nal;
}

int main()
{
    unsigned char x;
    //std::ifstream input("C:/Users/ami/Desktop/cats/segment8.fmp4", std::ios::binary);
    std::ifstream input("C:/Users/ami/Desktop/Test_zapis_34_original_iso_fragmented/fileSequence0.mp4", std::ios::binary);
    stringstream stream;
    input >> std::noskipws;
    while (input >> x) {
        stream << std::hex << std::setw(2) << std::setfill('0')
            << (int)x;
        cout << x;
    }
    string f;
    string nalTmp;
    f = stream.str().substr(stream.str().find("6d646174")+8);
    string nalSizeStr;
    for (int i = 0; i < 8; i++)
    {
        nalSizeStr += f[i];
    }
    int nalSize; // = stoi(nalSizeStr, 0, 16);
    nalTmp = readNal(f, 0, nalSize);
    cout << nalTmp << "  ";
    //nalSize = stoi(nalTmp.substr(0, 8), 0, 16);
    nalTmp = readNal(f, nalSize+8, nalSize);
    cout << nalTmp;
    

    int curPos = 0;
    for (int i = 0; i < 10; i++)
    {
        int size;
        nalTmp = readNal(f, curPos, size);
        char nalType = nalTmp[9];
        curPos += size + 8;
    }
}
