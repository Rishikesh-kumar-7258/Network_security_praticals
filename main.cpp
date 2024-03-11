#include <iostream>
#include <vector>
//#include "Caesar.cpp"
//#include "Playfair.cpp"
//#include "Hill.cpp"
//#include "ColumnarTransposition.cpp"
#include "DES.cpp"
#include <string>

int main() {

	std::string plain;
	std::cout << "Enter a string: ";
	std::getline(std::cin, plain);

	int key[64] = { 1, 0, 1, 0, 0, 0, 0, 1,
					0, 1, 0, 0, 0, 0, 1, 0,
					1, 0, 1, 0, 0, 0, 0, 1,
					0, 1, 0, 0, 0, 0, 1, 0,
					1, 0, 1, 0, 0, 0, 0, 1,
					0, 1, 0, 0, 0, 0, 1, 0,
					1, 0, 1, 0, 0, 0, 0, 1,
					0, 1, 0, 0, 0, 0, 1, 0 };

	DES des(key);
	std::string cipher = des.encrypt(plain);
	std::string decrypted = des.decrypt(cipher);

	/*std::string key;
	std::cout << "Enter a key: ";
	std::cin >> key;*/

	/*ColumnarTransposition columnarTransposition(key);
	std::string cipher = columnarTransposition.encrypt(plain);
	std::string decrypted = columnarTransposition.decrypt(cipher);*/

	/*Hill hill;
	std::string cipher = hill.encrypt(plain);
	std::string decrypted = hill.decrypt(cipher);*/

	/*Playfair playfair(key);
	std::string cipher = playfair.encrypt(plain);
	std::string decrypted = playfair.decrypt(cipher);*/

	/*int shift;
	std::cout << "Enter a shift (value should be between 1 to 25): ";
	std::cin >> shift;

	Caesar caesar(shift);
	std::string cipher = caesar.encrypt(plain);
	std::string decrypted = caesar.decrypt(cipher);*/

	std::cout << "Encrypted: " << cipher << std::endl;

	std::cout << "Decrypted: " << decrypted << std::endl;
}