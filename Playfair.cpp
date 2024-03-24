#ifndef PLAYFAIR_C
#define PLAYFAIR_C

#include "StreamCipher.cpp"

class Playfair : public StreamCipher {
private:
	std::vector<std::vector<char>> table;
	std::string key;
public:

	Playfair(std::string key) {
		this->key = toLowerCase(key);

		for (int i = 0; i < 5; i++) {
			std::vector<char> temp;
			for (int j = 0; j < 5; j++) {
				temp.push_back('@');
			}
			table.push_back(temp);
		}

		std::vector<bool> used(26, false);

		int keyLen = (int)this->key.size();
		int i = 0, j = 0, k = 0;

		while (k < keyLen) {

			int currentIndex = this->key[k] - 'a';

			if (currentIndex > 25) {
				std::cout << "index : " << currentIndex << std::endl;
				continue;
			}

			// if the character is already used
			if (used[currentIndex]) {
				k++;
				continue;
			}

			table[i][j] = this->key[k];
			used[currentIndex] = true;

			if (this->key[k] == 'i') used['j' - 'a'] = true;
			else if (this->key[k] == 'j') used['i' - 'a'] = true;

			j++, k++;
			if (j >= 5) i++, j = 0;
		}

		for (k = 0; k < 26; k++) {
			if (used[k]) continue;

			table[i][j] = 'a' + k;
			j++;

			if (j >= 5) i++, j = 0;
		}
	}

	std::string encrypt(std::string plain) override {
		plain = toLowerCase(plain);
		std::vector<int> spaces = removeSpaces(plain);

		// Creating pair
		std::string pairString;
		int i = 0;
		while (i < plain.size()) {
			pairString.push_back(plain[i]);
			i++;


			if (i >= plain.size() || plain[i] == plain[i - 1] ) pairString.push_back('x');
			else {
				pairString.push_back(plain[i]);
				i++;
			}
		}

		std::string encrypted;
		i = 0;
		while (i < pairString.size()) {
			char first = pairString[i], second = pairString[i + 1];

			int r1, c1, r2, c2;
			std::pair<int,int> r1c1 = getRowAndCol(first);
			std::pair<int,int> r2c2 = getRowAndCol(second);

			r1 = r1c1.first, c1 = r1c1.second;
			r2 = r2c2.first, c2 = r2c2.second;

			if (r1 == -1 || r2 == -1 || c1 == -1 || c2 == -1) {
				std::cout << "-1 aa rha hai check karo" << std::endl;
				break;
			}

			if (r1 == r2) {
				c1 = (c1 + 1) % 5;
				c2 = (c2 + 1) % 5;

				encrypted.push_back(table[r1][c1]);
				encrypted.push_back(table[r2][c2]);
			}
			else if (c1 == c2) {
				r1 = (r1 + 1) % 5;
				r2 = (r2 + 1) % 5;

				encrypted.push_back(table[r1][c1]);
				encrypted.push_back(table[r2][c2]);
			}
			else {
				encrypted.push_back(table[r1][c2]);
				encrypted.push_back(table[r2][c1]);
			}

			i += 2;
		}

		return encrypted;
	}

	std::string decrypt(std::string cipher) override {
		std::vector<int> spaces = removeSpaces(cipher);

		std::string decrypted;
		int i = 0;
		while (i < cipher.size()) {
			char first = cipher[i], second = cipher[i + 1];

			int r1, c1, r2, c2;
			std::pair<int, int> r1c1 = getRowAndCol(first);
			std::pair<int, int> r2c2 = getRowAndCol(second);

			r1 = r1c1.first, c1 = r1c1.second;
			r2 = r2c2.first, c2 = r2c2.second;

			if (r1 == -1 || r2 == -1 || c1 == -1 || c2 == -1) {
				std::cout << "-1 aa rha hai check karo" << std::endl;
				break;
			}

			if (r1 == r2) {
				c1 = (c1 - 1 + 5) % 5;
				c2 = (c2 - 1 + 5) % 5;

				decrypted.push_back(table[r1][c1]);
				decrypted.push_back(table[r2][c2]);
			}
			else if (c1 == c2) {
				r1 = (r1 - 1 + 5) % 5;
				r2 = (r2 - 1 + 5) % 5;

				decrypted.push_back(table[r1][c1]);
				decrypted.push_back(table[r2][c2]);
			}
			else {
				decrypted.push_back(table[r1][c2]);
				decrypted.push_back(table[r2][c1]);
			}

			i += 2;
		}

		return decrypted;

	}

	std::pair<int, int> getRowAndCol(char c)  {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (table[i][j] == c) return std::make_pair(i, j);
			}
		}

		return std::make_pair(-1, -1);
	}

	void printTable() {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				std::cout << table[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	
};

#endif // !PLAYFAIR_C