#ifndef BLOCK_CIPHER_C
#define BLOCK_CIPHER_C

#include "Cryptography.cpp"

class BlockCipher : public Cryptography {
private:
	std::string type;
	std::string xorBlocks(std::string block1, std::string block2) const
	{
		std::string result;
		for (int i = 0; i < block1.size(); i++)
		{
			result.push_back(block1[i] ^ block2[i]);
		}
		return result;
	}
public:
	BlockCipher(std::string type="ecb") : Cryptography() {
		this->type = type;
	}

	// The ECB and CBC functions cannot be used directly as they are not implemented yet
	std::vector<std::string> ECB(std::string plaintext, int blockSize)
	{
		std::vector<std::string> blocks = getBlocks(plaintext, blockSize);
	}
	std::vector<std::string> CBC(std::string plaintext, int blockSize)
	{
		std::vector<std::string> blocks = getBlocks(plaintext, blockSize);

		std::string IV(blockSize, '0');
		std::string prevCipher = IV;

		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i] = xorBlocks(blocks[i], prevCipher);
			prevCipher = blocks[i];
		}

		return blocks;
	}
	// The following functions are not implemented yet

	std::string getNextBlock() {
		return "";
	}

	std::vector<std::string> getBlocks(std::string text, int blockSize) {
		text = addPadding(text, blockSize);

		std::vector<std::string> blocks;
		for (int i = 0; i < text.size(); i += blockSize) {
			blocks.push_back(text.substr(i, blockSize));
		}

		return blocks;
	}

	std::string addPadding(std::string text, int blockSize) {
		int paddingAmount = (blockSize - (text.size() % blockSize)) % blockSize;

		for (int i = 0; i < paddingAmount; i++) {
			text.push_back((char)paddingAmount);
		}

		return text;
	}

	std::string removePadding(std::string text, int blockSize) {
		int paddingAmount = (int)text[text.size() - 1];

		if (paddingAmount > blockSize) {
			return text;
		}

		text = text.substr(0, text.size() - paddingAmount);

		return text;
	}
};

#endif // !BLOCK_CIPHER_C
