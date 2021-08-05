#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <istream>

using namespace std;


int blocktoint(uint8_t* input, int startPosition) {  // парсинг размера
    string strres;
    stringstream stream;
    for (int i = startPosition; i < startPosition + 4; i++) {
        
         stream << std::setw(2) << std::setfill('0') << hex // переводим в 16-ричное значение
              << (int)input[i];
    }
    strres = stream.str();
    int res = stoi(strres, 0, 16); // переводим результат в число
    
    return res;
}

int readNalType(uint8_t* input, int startPosition) // для чтения типа нал юнита (нужна определённая часть байта)
{
    string strres;
    stringstream stream;
    //std bitset
            stream << std::setw(2) << std::setfill('0') << hex
                << (int)input[startPosition];
    strres = stream.str();
    strres = strres[1]; // тут лежит тип
    int res = stoi(strres, 0, 16);

    return res;
}

void main()
{
    std::ifstream is("C:/Users/ami/Desktop/Test_zapis_34_original_iso_fragmented/fileSequence5.m4s", ios::binary); // C:/Users/ami/Desktop/cats/segment3.fmp4
    if (is.is_open()) {
        // get length of file:
        is.seekg(0, is.end); // ставим маркер в конец 
        int length = is.tellg(); // получаем позицию маркера, это длина файла
        is.seekg(0, is.beg); // ставим маркер в начало

        uint8_t* data = new uint8_t[length];

        std::cout << "Reading " << length << " characters... ";
        // читаем данные
        is.read((char*)data, length);

        if (is)
            std::cout << "all characters read successfully. \n";
        else
            std::cout << "error: only " << is.gcount() << " could be read \n";
        is.close();

        int n = 0;
        uint8_t name[4];
        uint8_t mdat[4] = { 'm', 'd', 'a', 't' };
        bool flag = true;
        while (flag) {
            int size = blocktoint(data, n);
            name[0] = data[n + 4];
            name[1] = data[n + 5];
            name[2] = data[n + 6];
            name[3] = data[n + 7];
            n += size;
            if (!strncmp((char*)name, (char*)mdat, 4)) { // ищем мдат блок
                flag = false;
                n -= size; // возвращаемся к началу мдат блока
            }
        }

    int counter = 0;
    n += 8; // переводим на начало первого нал юнита
    while (n < length)
    {
        counter++;
        int size = blocktoint(data, n); // читаем размер 
        int nalType = readNalType(data, n + 4); // читаем тип юнита
        cout << "Nal Unit #" << counter << ": size - " << size << " bytes, type - " << nalType << "\n";
        n += size + 4;
    }

        delete[] data;
    }

}