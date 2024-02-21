/**
  * @file FileHandler.cpp
  * @brief Code for the FileHandler class methods
  */

#include "FileHandler/FileHandler.h"

FileHandler::FileHandler()
{
    this->fOpened = false;
}


FileHandler::FileHandler(std::string filename, bool binaryFlg, bool convertFlg)
{
    this->set_convert(convertFlg);
    this->set_binary(binaryFlg);
    this->set_filename(filename);
    this->create_file();
}


FileHandler::~FileHandler()
{
    this->close_all();
}

void FileHandler::close()
{
    if (this->binary)
    {
        int status = fclose(this->ptrFile);
        if (status == 0) this->fOpened = false;
    }
    else
    {
        this->filestream.close();
        this->filestream.clear();
    }
}

void FileHandler::close_all()
{
    if (this->filestream.is_open())
    {
        this->filestream.close();
        this->filestream.clear();
    }
    if (this->fOpened)
    {
        int status = fclose(this->ptrFile);
        if (status == 0) this->fOpened = false;
    }
}

void FileHandler::create_file()
{
    if (this->get_binary())
    {
        this->ptrFile = fopen(this->filename.c_str(), "wb");
        if (this->ptrFile != NULL)
        {
            this->fOpened = true;
        }
        else
        {
            this->fOpened = false;
        }
        //this->filestream.open(this->filename.c_str(), std::ios::out | std::ios::binary);
    }
    else
    {
        this->filestream.open(this->filename.c_str(), std::ios::out);
    }
}

std::string FileHandler::get_asciiFilename()
{
    return this->asciiFilename;
}

bool FileHandler::get_binary()
{
    return this->binary;
}

bool FileHandler::get_convert()
{
    return this->convert;
}

std::string FileHandler::get_filename()
{
    return this->filename;
}

void FileHandler::set_asciiFilename(std::string fname)
{
    this->asciiFilename = fname;
}

void FileHandler::set_binary(bool binaryFlg)
{
    this->binary = binaryFlg;
}

void FileHandler::set_filename(std::string fname)
{
    this->filename = fname;
}

void FileHandler::set_convert(bool flg)
{
    this->convert = flg;
}


