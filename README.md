# Text File (De)Compressor
Text File (De)Compressor is a ```C program``` used to compress and decompress text files with [huffman coding](https://en.wikipedia.org/wiki/Huffman_coding#Applications).

## Compilation 
First, ensure your computer can compile ```C programs``` and handle ```Makefile``` commands in the terminal.

**For Mac Users:** ensure [Xcode Command Line Tools](https://macpaw.com/how-to/install-command-line-tools) are installed before proceeding. 

**For Windows Users:** ensure the [GCC compiler](https://www.scaler.com/topics/c/c-compiler-for-windows/) and [Makefile](https://www.youtube.com/watch?v=a3ejgWLqelQ) are installed before proceeding.

Once that is done, clone the Git Repository into your local machine:

```bash
git clone https://github.com/Jun0610/text-compression-decompression.git
```

Then, run the following Make commands to compile the ```compress.exe``` and ```decompress.exe``` executables respectively:

```bash
make compress.exe
```

```bash
make decompress.exe
```

Two new executable files, ```compress.exe``` and ```decompress.exe```, should now be in your working directory.

## Usage
To compress a text file, run the following command:

```bash
./compress.exe fileName
```

The compressed file, ```fileName.cmp``` should now be in your working directory.

To decompress the ```.cmp``` file, run the following command:
```bash
./decompress.exe fileName.cmp
```

The decompressed file, ```fileName.dcmp``` should now be in your working directory.

A sample text file ```pride_and_prejudice``` has been included for your convenience.

To remove the executable files and all ```.cmp``` and ```.dcmp``` files, run:
```bash
make clean
```

## Additional Notes
1. Since huffman coding revolves around bit manipulation, the compression/decompression software can technically be used to compress all file formats eg, .pdf, .docx, .png, etc (since all files are simply a string of bits). However, its effectiveness on these additional file formats is not guranteed. From testing, huffman coding seems to be the most effective with text files.

2. All testing was done on an ARM64 chip (M1 MacBook Air).

## License
[MIT License](https://github.com/Jun0610/text-compression-decompression/blob/main/LICENSE)

## Author
Jun Shern Lim  
lim321@purdue.edu  
Computer Science '24   
Purdue University
