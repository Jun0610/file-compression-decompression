int decodeCodingTree(TreeNode * root, FILE * infile, FILE * outfile, long * count);
TreeNode * buildDecompressHuffmanTree(FILE * in, int * curByte, int * byteIndex);
int readAscii(FILE * in, int * curByte, int * readByteIndex);
void decompress(TreeNode * huffmanRoot, FILE * compressed, FILE * restored, long uncompressedSize);
void displayDecompression(long uncompressedSize, long compressedSize);
int verifyCompressedFile(char * fileName);
void createDecompressedFile(char * fileName, char * toReturn);
