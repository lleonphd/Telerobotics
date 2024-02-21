/**
  * @file FileHandler.h
  * @brief Declaration of the FileHandler class
  */

#ifndef _FILE_HANDLER_H_
#define _FILE_HANDLER_H_

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

/// Setup file for streaming output to
/**
  * FileHandler class is used to setup a file for writing in ascii or binary
  * includes a member function that convert the binary file into ascii
  * while this class can be used apart from Qt, it is primarily intended
  * to be used with a companion widget as a graphical client.
  * this is done by passing a pointer to the FileHandler object
  * to the FileHandler class.
  * see FileHandlerDialog class for more information
  */
class FileHandler
{
private:
    bool binary;
    bool convert;
    bool fOpened; // tells me if FILE contains a valid stream

    FILE * ptrFile;
    std::fstream filestream;

    std::string filename;
    std::string asciiFilename;

public:
    FileHandler();
    FileHandler(std::string filename, bool binaryFlg = false, bool convertFlg = false);
    ~FileHandler();

    void close();
    void close_all();

    void create_file();

    std::string get_asciiFilename();
    bool get_binary();
    bool get_convert();
    std::string get_filename();

    void set_asciiFilename(std::string fname);
    void set_binary(bool binaryFlg);
    void set_filename(std::string fname);
    void set_convert(bool flg);


    template<class T>
    void to_ascii(std::string inputName, std::string outputName, T *buffer, size_t count = 1);

    template<class T>
    void write(T *buffer, size_t count = 1);
};

template<class T>
void FileHandler::write(T *buffer, size_t count)
{
    if (this->binary)
    {
        fwrite(buffer, sizeof(*buffer), count, this->ptrFile);
        //this->binaryFilestream.write((char *)buffer, sizeof(*buffer));
    }
    else
    {
        this->filestream << *buffer;
    }
}

template<class T>
void FileHandler::to_ascii(std::string inputName, std::string outputName, T *buffer, size_t count)
{
    FILE * inputFile = fopen(inputName.c_str(), "rb");

    if (!(inputFile == NULL))
    {
        std::fstream outputFile;
        outputFile.open(outputName.c_str(), std::ios::out);

        while(!feof(inputFile))
        {
            if (fread(buffer, sizeof(*buffer), count, inputFile))
                /*
                 * the command is in this form
                 * fread(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);
                 * where
                 * x is the array address
                 * sizeof(x[0]) is the size of the first element of that array
                 * sizeof(x)/sizeof(x[0]) is the size of the whole array divided by the size of the first element
                 * fp is the file pointer
                 *
                 */
            {
                for (unsigned int i = 0; i < count; i++)
                {
                    outputFile << buffer[i];
                    if (!(i == count-1))
                    {
                        outputFile << ",";
                    }
                }
                outputFile << std::endl;
            }
            else if (feof(inputFile))
            {
                std::cout << "reached end of file" << std::endl;
                break;
            }
            else
            {
                std::cout << "ascii conversion is not successful" << std::endl;
                break;
            }
        }
        outputFile.close();
        outputFile.clear();
    }
    else
    {
        std::cout << "error opening binary file" << std::endl;
    }
    fclose(inputFile);
}

#endif // _FILE_HANDLER_H_
