#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

string readNal(string input, int startPosition, int &size) // считывание блока нал из входного потока с введённого символа
{                                                          // размер для вывода
    string nal;
    string nalSize;
    for (int i = startPosition; i < startPosition + 8; i++)
    {
        nalSize += input[i];
    }
    size = stoi(nalSize, 0, 16)*2; // умножить на два потому что бит это символа 
    nal += nalSize;
    nal = input.substr(startPosition, size+8);


    return nal;
}

void main()
{
    unsigned char x;
    std::ifstream input("C:/Users/ami/Desktop/cats/segment2.fmp4", std::ios::binary); // ввод
    stringstream stream;
    input >> std::noskipws;
    while (input >> x) { // чтение данных
        stream << std::hex << std::setw(2) << std::setfill('0')
            << (int)x;
    }
    string f;
    string nalTmp;
    f = stream.str().substr(stream.str().find("6d646174")+8); // поиск начала
    

    int curPos = 0;
    while (true)
    {
        int size;
        nalTmp = readNal(f, curPos, size);
        char nalType = nalTmp[9]; // здесь лежит тип
        if (nalType == '5') {
            cout << "WE WON";
            return;
        } else
            if (nalType == '1') {
                cout << "Wrong nal unit start";
                return;
            }
        curPos += size + 8;
    }
}
