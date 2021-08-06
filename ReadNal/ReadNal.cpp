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
		for (int n = sizeof(result) + startPosition; n + startPosition >= startPosition; n--) {
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

int main()
{
	std::ifstream offset("C:/Users/ami/Desktop/Test_zapis_34_original_iso_fragmented/fileSequence2.m4s", std::ios::binary); // C:/Users/ami/Desktop/cats/segment3.fmp4
	if (!offset.is_open())
	{
		std::cout << "Error description" << std::endl;
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
	uint8_t name[4];
	uint8_t mdat[4] = { 'm', 'd', 'a', 't' };
	bool mdatFound = false;
	while (n < length) {
		int size = bytesToInt(data, n);
		name[0] = data[n + 4];
		name[1] = data[n + 5];
		name[2] = data[n + 6];
		name[3] = data[n + 7];
		n += size;
		if (!strncmp((char*)name, (char*)mdat, 4)) { // ищем мдат блок
			n -= size; // возвращаемся к началу мдат блока
			mdatFound = true;
			break;
		}
	}

	if (!mdatFound) { std::cout << "Error description" << std::endl; return 1; }
	size_t counter = 0;
	n += 8; // переводим на начало первого нал юнита
	while (n < length)
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