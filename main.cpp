#include <iostream>
#include <vector>
#include "Caesar.cpp"
#include "Playfair.cpp"
#include <string>

int main() {
	std::string plain;
	std::cout << "Enter a string: ";
	std::getline(std::cin, plain);

	std::string key;
	std::cout << "Enter a key: ";
	std::cin >> key;

	Playfair playfair(key);
	std::string cipher = playfair.encrypt(plain);
	std::string decrypted = playfair.decrypt(cipher);

	/*int shift;
	std::cout << "Enter a shift (value should be between 1 to 25): ";
	std::cin >> shift;

	Caesar caesar(shift);
	std::string cipher = caesar.encrypt(plain);
	std::string decrypted = caesar.decrypt(cipher);*/

	std::cout << "Encrypted: ";
	for (int i = 0; i < cipher.size(); i++) {
		std::cout << (char)cipher[i];
	}

	std::cout << std::endl << "Decrypted: ";
	for (int i = 0; i < decrypted.size(); i++) {
		std::cout << (char)decrypted[i];
	}
}