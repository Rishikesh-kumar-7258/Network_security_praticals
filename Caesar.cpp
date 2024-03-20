#ifndef CAESAR_C
#define CAESAR_C

#include "Cryptography.cpp"

class Caesar : public Cryptography {
private:
	int shift;
public:

	Caesar(int shift) {
		this->shift = shift;
	}

	std::string encrypt(std::string plain) const {
		plain = toLowerCase(plain);
		std::cout << plain << std::endl;

		std::string cipher;
		

		for (int i = 0; i < plain.size(); i++) {
			if (plain[i] == ' ' || plain[i] == '\n' || plain[i] == '\t') {
				cipher.push_back(plain[i]);
				continue;
			}
			cipher.push_back('a' + (((plain[i] - 'a') + shift) % 26));
		}

		return cipher;
	}

	std::string decrypt(std::string cipher) const{
		std::string plain;
		for (int i = 0; i < cipher.size(); i++) {
			if (cipher[i] == ' ' || cipher[i] == '\n' || cipher[i] == '\t') {
				plain.push_back(cipher[i]);
				continue;
			}
			plain.push_back('a' + (((cipher[i] - 'a') - shift + 26) % 26));
		}

		return plain;
	}
};

#endif // !CAESAR_C
