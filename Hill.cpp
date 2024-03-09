#include "Cryptography.cpp"

class Hill : public Cryptography {
private:
	std::vector<std::vector<int>> encryptionKey, decryptionKey;
public:

	Hill() {
		encryptionKey = { {3, 7}, {5, 12} };
		decryptionKey = { {12, 19}, {21, 3} };
	}

	std::string encrypt(std::string plain) {
		
		std::vector<int> spaces = removeSpaces(plain);

		// make the plain text even
		if (plain.size() % 2 == 1) plain.push_back('\0');

		std::string cipher;
		int i = 0;
		while (i < plain.size()) {
			int first = plain[i] - 'a' + 1, second = plain[i + 1] == '\0' ? 0 : plain[i+1] - 'a' + 1;

			int efirst = (encryptionKey[0][0] * first + encryptionKey[0][1] * second) % 26;
			int esecond = (encryptionKey[1][0] * first + encryptionKey[1][1] * second) % 26;

			cipher.push_back('a' + efirst);
			cipher.push_back('a' + esecond);

			i += 2;
		}

		return cipher;
	}

	std::string decrypt(std::string cipher) {

		std::string plain;

		int i = 0;
		while (i < cipher.size()) {
			int first = cipher[i] - 'a', second = cipher[i + 1] - 'a';

			int dfirst = (decryptionKey[0][0] * first + decryptionKey[0][1] * second) % 26;
			int dsecond = (decryptionKey[1][0] * first + decryptionKey[1][1] * second) % 26;

			plain.push_back('a' + dfirst - 1);
			plain.push_back('a' + dsecond - 1);

			i += 2;
		}

		return plain;
	}
};