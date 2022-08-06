# Text File (De)Compressor
Text File (De)Compressor is a ```C program``` used to compress and decompress text files with [huffman coding](https://en.wikipedia.org/wiki/Huffman_coding#Applications).

## Compilation 
First, clone the Git Repository into your local machine:

```bash
git clone https://github.com/Jun0610/text-compression-decompression.git
```

Then, run the following Make commands to compile the ```compress``` and ```decompress``` executables respectively:

```bash
make compress
```

```bash
make decompress
```

Two new executable files, ```compress``` and ```decompress```, should now be in your working directory.

## Usage
To compress a text file, run the following command:

```bash
./compress fileName
```

The compressed file, ```fileName.cmp``` should now be in your working directory.

To decompress the ```.cmp``` file, run the following command:
```bash
./decompress fileName.cmp
```

The decompressed file, ```fileName.dcmp``` should now be in your working directory.

To remove the executable files and all ```.cmp``` and ```.dcmp``` files, run:
```bash
make clean
```

## Additional Notes
Since huffman coding revolves around bit manipulation, the compression/decompression software can technically be used to compress all file formats eg, .pdf, .docx, .png, etc (since all files are simply a string of bits). However, its effectiveness on these additional file formats is not guranteed.  

From testing, huffman coding seems to be the most effective with text files.

## License
[MIT License]()

## Author
Jun Shern Lim  
lim321@purdue.edu  
Computer Engineering '24   
Purdue University