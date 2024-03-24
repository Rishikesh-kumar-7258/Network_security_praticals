#ifndef COLUMNARTRANSPOSITION_C
#define COLUMNARTRANSPOSITION_C


#include "StreamCipher.cpp"
#include <algorithm>

class ColumnarTransposition : public StreamCipher {
private:
	std::string key;
public:

	ColumnarTransposition(std::string key) {
		this->key = toLowerCase(key);
	}

	std::string encrypt(std::string plain) override {
		plain = toLowerCase(plain);
		std::vector<int> spaces = removeSpaces(plain);

		int rows = (int)(plain.size() / key.size());
		if (plain.size() % key.size() != 0) {
			rows++;
		}

		std::vector<std::vector<char>> matrix(rows, std::vector<char>(key.size(), ' '));

		// In by rows
		int k = 0;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < key.size(); j++) {
				if (k < plain.size()) {
					matrix[i][j] = plain[k++];
				}
			}
		}

		std::vector<std::pair<char, int>> keyIndex;
		for (int i = 0; i < key.size(); i++) {
			keyIndex.push_back(std::make_pair(key[i], i));
		}

		std::sort(keyIndex.begin(), keyIndex.end());

		// out by columns
		std::string cipher;
		for (int i = 0; i < key.size(); i++) {
			int index = keyIndex[i].second;
			for (int j = 0; j < rows; j++) {
				if (matrix[j][index] != ' ')
					cipher.push_back(matrix[j][index]);
			}
		}

		int i = (int)key.size();
		while (i < cipher.size()) {
			cipher.insert(i, " ");

			i += (int)key.size() + 1;
		}

		return cipher;
	}

	std::string decrypt(std::string cipher) override {
		std::vector<int> spaces = removeSpaces(cipher);

		int rows = (int)(cipher.size() / key.size());
		if (cipher.size() % key.size() != 0) {
			rows++;
		}

		std::vector<std::vector<char>> matrix(rows, std::vector<char>(key.size(), ' '));

		int k = 0;
		for (int i = 0; i < key.size(); i++) {
			for (int j = 0; j < rows; j++) {
				matrix[j][i] = cipher[k++];
			}
		}

		std::vector<std::pair<char, int>> keyIndex;
		for (int i = 0; i < key.size(); i++) {
			keyIndex.push_back(std::make_pair(key[i], i));
		}

		std::sort(keyIndex.begin(), keyIndex.end());


		std::string plain;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < key.size(); j++) {
			int index = keyIndex[j].second;
				plain.push_back(matrix[i][index]);
			}
		}

		return plain;

	}
};

#endif // !COLUMNARTRANSPOSITION_C
