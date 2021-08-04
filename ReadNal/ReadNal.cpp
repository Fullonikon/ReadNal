#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <istream>

using namespace std;


int blocktoint(char* input, int n) {
    char b[4];
    string strres;
    stringstream stream;
    for (int i = n; i < n + 4; i++) {
        b[i - n] = input[i];
        if (input[i] < 0) {
            stream << std::setw(2) << std::setfill('0') << hex
                << (int)input[i] + 256;  // как тут получается отрицательное значение? 
        }
        else {
            stream << std::setw(2) << std::setfill('0') << hex
                << (int)input[i];
        }
    }
    strres = stream.str();
    int res = stoi(strres, 0, 16);
    
    return res;
}

string readNal(string input, int startPosition, int &size) // считывание блока нал из входного потока с введённого символа
{                                                          // размер для вывода
    string nal;
    string nalSize;
    for (int i = startPosition; i < startPosition + 8; i++)
    {
        nalSize += input[i];
    }
    size = stoi(nalSize, 0, 16)*2; // умножить на два потому что бит это два символа 
    nal += nalSize;
    nal = input.substr(startPosition, size+8);


    return nal;
}

void main()
{
    //unsigned char x;
    //std::ifstream input("C:/Users/ami/Desktop/cats/segment1.fmp4", std::ios::binary); // ввод
    //stringstream stream;
    //input >> std::noskipws;
    //char pep[10];
    //if (input.is_open()) {
    //    int k = 0;
    //    while (input >> x) { // чтение данных
    //        stream << std::hex << std::setw(2) << std::setfill('0')
    //            << (int)x;
    //        //pep[k] = x;
    //        k++;
    //    }
    //}
    //else
    //{
    //    return;
    //}

    //string f;
    //vector<uint8_t> data;
    //input.read(pep, 1);
    //string nalTmp;
    //f = stream.str().substr(stream.str().find("6d646174")+8); // поиск начала
    //int mdatSize = stoi(stream.str().substr(stream.str().find("6d646174") - 8, 8), 0, 16);

    //int curPos = 0;
    //int counter = 0;
    //while (curPos < mdatSize)
    //{
    //    counter++;
    //    int size;
    //    nalTmp = readNal(f, curPos, size);
    //    char nalType = nalTmp[9]; // здесь лежит тип
    //    cout << "Nal Unit #" << counter << ": size - " << size / 2 << " bytes, type - " << nalType << "\n";
    //    curPos += size + 8;
    //}


    std::ifstream is("C:/Users/ami/Desktop/cats/segment1.fmp4", ios::binary); // C:/Users/ami/Desktop/Test_zapis_34_original_iso_fragmented/fileSequence4.m4s
    if (is.is_open()) {
        // get length of file:
        is.seekg(0, is.end); // ставим маркер в конец 
        int length = is.tellg(); // получаем позицию маркера, это длина файла
        is.seekg(0, is.beg); // ставим маркер в начало

        char* data = new char[length];

        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        is.read(data, length);

        if (is)
            std::cout << "all characters read successfully. \n";
        else
            std::cout << "error: only " << is.gcount() << " could be read \n";
        is.close();

        // 6d646174 == 109 100 97 116 == mdat
        int n = 0;
        char name[4];
        char mdat[4] = { 'm', 'd', 'a', 't' };
        bool flag = true;
        while (flag) {
            int size = blocktoint(data, n);
            name[0] = data[n + 4];
            name[1] = data[n + 5];
            name[2] = data[n + 6];
            name[3] = data[n + 7];
            n += size;
            if (!strncmp(name, mdat, 4)) flag = false;
        }

        // ...buffer contains the entire file...

        delete[] data;
    }


}


//NAL_UNIT_NUMBER - NAL_UNIT_SIZE - NAL_UNIT_TYPE
//std::vector<uint8_t> data
//uin8_t* data = new uint8_t[10];