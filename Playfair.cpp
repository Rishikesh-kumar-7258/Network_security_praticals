#include "Cryptography.cpp"

class Playfair : public Cryptography {
private:
	std::vector<std::vector<char>> table;
	std::string key;
public:

	Playfair(std::string key) {
		this->key = key;

		// Create the table
		std::vector<char> temp;
		for (int i = 0; i < key.size(); i++) {
			if (key[i] == 'j') {
				key[i] = 'i';
			}
			if (std::find(temp.begin(), temp.end(), key[i]) == temp.end()) {
				temp.push_back(key[i]);
			}
		}

		for (char c = 'a'; c <= 'z'; c++) {
			if (c == 'j') {
				continue;
			}
			if (std::find(temp.begin(), temp.end(), c) == temp.end()) {
				temp.push_back(c);
			}
		}

		for (int i = 0; i < 5; i++) {
			std::vector<char> row;
			for (int j = 0; j < 5; j++) {
				row.push_back(temp[i * 5 + j]);
			}
			table.push_back(row);
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				std::cout << table[i][j] << " ";
			}
			std::cout << std::endl;
		}

	}
	
};