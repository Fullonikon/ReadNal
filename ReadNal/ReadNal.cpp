#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>

using namespace std;


// Функция для поиска подстроки в строке
// + поиск позиции, с которой начинается подстрока
int pos(char* s, char* c, int n)
{
    int i, j; // Счетчики для циклов
    int lenC, lenS; // Длины строк

    //Находим размеры строки исходника и искомого
    for (lenC = 0; c[lenC]; lenC++);
    for (lenS = 0; s[lenS]; lenS++);

    for (i = 0; i <= lenS - lenC; i++) // Пока есть возможность поиска
    {
        for (j = 0; s[i + j] == c[j]; j++); // Проверяем совпадение посимвольно
        // Если посимвольно совпадает по длине искомого
        // Вернем из функции номер ячейки, откуда начинается совпадение
        // Учитывать 0-терминатор  ( '\0' )
        if (j - lenC == 1 && i == lenS - lenC && !(n - 1)) return i;
        if (j == lenC)
            if (n - 1) n--;
            else return i;
    }
    //Иначе вернем -1 как результат отсутствия подстроки
    return -1;
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


    std::ifstream is("C:/Users/ami/Desktop/cats/segment1.fmp4", std::ifstream::binary); // C:/Users/ami/Desktop/Test_zapis_34_original_iso_fragmented/fileSequence4.m4s
    if (is) {
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



        std::string str;
        char* writable = new char[str.size() + 1];
        std::copy(str.begin(), str.end(), writable);
        writable[str.size()] = '\0';

        // 6d646174 == 109 100 97 116 == mdat

        char* psh;
        char mdat[] = "mdat";

        psh = strstr(writable, mdat);

        for (int i = 0; i < length/1000; i++) {
            cout << std::hex << std::setw(2) << std::setfill('0')
                 << (int)psh[i];
        }

        // ...buffer contains the entire file...

        delete[] data;
    }


}


//NAL_UNIT_NUMBER - NAL_UNIT_SIZE - NAL_UNIT_TYPE
//std::vector<uint8_t> data
//uin8_t* data = new uint8_t[10];