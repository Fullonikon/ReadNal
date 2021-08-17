#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <istream>
#include <bitset>


size_t bytesToInt(const uint8_t* bytes, size_t startPosition, bool littleEndian = false) {
	size_t result = 0;
	if (littleEndian) {
		for (int n = sizeof(result) + startPosition; n >= startPosition; n--) {
			result = (result << 8) + bytes[n];
		}
	}
	else {
		for (int n = startPosition; n < startPosition + sizeof(result); n++) {
			result = (result << 8) + bytes[n];
		}
	}
	return result;
}

size_t readNalType(uint8_t* input, int startPosition) // для чтения типа нал юнита (нужна определённая часть байта)
{
	std::bitset<5> res = input[startPosition];
	return (size_t)res.to_ulong();
}

size_t findTrun(uint8_t* input, int startPosition, size_t size) // заходим в moof -> ищем размер первого traf -> переходим ко второму -> 
																// ищем trun -> если нет второго traf то идём до конца mdat
{
	size_t curSize = startPosition + size + 4;
	size_t cur = startPosition + 8;
	char name[4];
	char trun[4] = { 't', 'r', 'u', 'n' };
	char traf[4] = { 't', 'r', 'a', 'f' };
	size_t counter = 0;
	while (cur < curSize) {
		int size = bytesToInt(input, cur); // ищем второй тиран, потому что его смещение это конец данных первого тирана
		name[0] = input[cur + 4];
		name[1] = input[cur + 5];
		name[2] = input[cur + 6];  
		name[3] = input[cur + 7];
		if (!strncmp(name, traf, 4)) {
			counter++;
			if (counter == 2) {
				break;
			}
		}
		cur += size;
	}
	if (counter < 2) {
		return 0;
	}
	cur += 8;
	while (cur < curSize) {
		int size = bytesToInt(input, cur);
		name[0] = input[cur + 4];
		name[1] = input[cur + 5];
		name[2] = input[cur + 6];
		name[3] = input[cur + 7];
		if (!strncmp(name, trun, 4)) {
			size_t res = bytesToInt(input, cur + 16);
			return res;
		}
		cur += size;
	}
	return (size_t)0;
}

int main()
{
	std::ifstream offset("C:/Users/ami/Desktop/cats/segment1.fmp4", std::ios::binary); // C:/Users/ami/Desktop/Test_zapis_34_original_iso_fragmented/fileSequence4.m4s
	if (!offset.is_open())
	{
		std::cout << "Couldn't open file" << std::endl;
		return 1;
	}
	// get length of file:
	offset.seekg(0, offset.end); // ставим маркер в конец 
	size_t length = offset.tellg(); // получаем позицию маркера, это длина файла
	offset.seekg(0, offset.beg); // ставим маркер в начало

	uint8_t* data = new uint8_t[length];

	std::cout << "Reading " << length << " characters... ";
	// читаем данные
	offset.read((char*)data, length);

	if (offset) {
		std::cout << "all characters read successfully. \n";
	}
	else {
		std::cout << "error: only " << offset.gcount() << " could be read \n";
	}
	offset.close();

	size_t n = 0;
	size_t ofset;
	size_t mdatSize;
	uint8_t name[4];
	uint8_t mdat[4] = { 'm', 'd', 'a', 't' };
	uint8_t moof[4] = { 'm', 'o', 'o', 'f' };
	bool mdatFound = false;
	size_t firstTrunSize = 0;
	while (n < length) {
		int size = bytesToInt(data, n);
		name[0] = data[n + 4];
		name[1] = data[n + 5];
		name[2] = data[n + 6];
		name[3] = data[n + 7];
		n += size;
		if (!strncmp((char*)name, (char*)moof, 4)) { 
			ofset = findTrun(data, n - size, size);
		}
		if (!strncmp((char*)name, (char*)mdat, 4)) { // ищем мдат блок
			n -= size; // возвращаемся к началу мдат блока
			mdatSize = size;
			mdatFound = true;
			break;
		}
	}

	if (!mdatFound) 
	{ 
		std::cout << "Mdat block wasn't found" << std::endl; 
		delete[] data;
		return 1; 
	}

	size_t counter = 0;
	size_t end = 0;
	if (ofset == 0) {
		end = n + mdatSize; // если нет второго тирана то щупаем весь мдат
	}
	else {
		end = n + ofset;
	}
	n += 8; // переводим на начало первого нал юнита
	
	while (n < end)
	{
		counter++;
		int size = bytesToInt(data, n); // читаем размер 
		int nalType = readNalType(data, n + 4); // читаем тип юнита
		std::cout << "Nal Unit #" << counter << ": size - " << size << " bytes, type - " << nalType << "\n";
		n += size + 4;
	}

	delete[] data;

	return 0;
}
