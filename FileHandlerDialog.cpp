/**
  * @file FileHandlerDialog.cpp
  * @brief Code for the FileHandlerDialog class methods
  */

#include "FileHandlerDialog/FileHandlerDialog.h"

FileHandlerDialog::FileHandlerDialog(FileHandler * fh, QWidget *parent) : QDialog(parent)
{
    this->set_fileHandler(fh);
    this->initialize_dialog();
}

FileHandlerDialog::~FileHandlerDialog()
{

}

bool FileHandlerDialog::browse()
{
    QString fname;
    fname.clear();

    if (this->asciiButton->isChecked()) // ascii output
    {
        fname = QFileDialog::getSaveFileName(this, "Create file to save data to", QDir::currentPath(), tr("*.csv"));
    }
    else // binary output
    {
        fname = QFileDialog::getSaveFileName(this, "Create file to save data to", QDir::currentPath(), tr("*.dat"));
    }

    if (fname.isEmpty())
    {
        qDebug() << "No file selected";
        return false;
    }
    else
    {
        qDebug() << "Save data to " << fname;
        this->saveFilename = fname;
        this->filenameLineEdit->setText(fname);
        return true;
    }
}

bool FileHandlerDialog::browse_ascii()
{
    QString fname = QFileDialog::getSaveFileName(this, "Convert binary data to this file", QDir::currentPath(), tr("*.csv"));
    if (fname.isEmpty())
    {
        qDebug() << "No file selected";
        return false;
    }
    else
    {
        qDebug() << "Create file to convert binary data to " << fname;
        this->convertFilename = fname;
        this->asciiNameLineEdit->setText(fname);
        return true;
    }
}

void FileHandlerDialog::connect_buttons_to_slots()
{
    connect(this->acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(this, SIGNAL(accepted()), this, SLOT(save_fileHandler()));
    connect(this->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this->selectFileButton, SIGNAL(clicked()), this, SLOT(browse()));
    connect(this->selectAsciiButton, SIGNAL(clicked()), this, SLOT(browse_ascii()));
    connect(this->binaryButton, SIGNAL(toggled(bool)), this->convertCheckBox, SLOT(setEnabled(bool)));
    connect(this->binaryButton, SIGNAL(toggled(bool)), this->convertCheckBox, SLOT(setChecked(bool)));
}


void FileHandlerDialog::create_buttons()
{
    this->selectFileButton = new QPushButton();
    this->selectFileButton->setText("Browse");

    this->selectAsciiButton = new QPushButton();
    this->selectAsciiButton->setText("Browse");

    this->asciiButton = new QRadioButton("ASCII output", this);
    this->binaryButton = new QRadioButton("Binary output", this);

    this->cancelButton = new QPushButton();
    this->cancelButton->setText("Cancel");

    this->acceptButton = new QPushButton();
    this->acceptButton->setText("Accept");
    this->acceptButton->setDefault(true);

}

void FileHandlerDialog::create_checks()
{
    this->convertCheckBox = new QCheckBox("Convert to ASCII");
}

void FileHandlerDialog::create_displays()
{
    this->filenameLineEdit = new QLineEdit();
    this->filenameLineEdit->setText("No File Selected");
    this->asciiNameLineEdit = new QLineEdit();
    this->asciiNameLineEdit->setText("No File Selected");
}

void FileHandlerDialog::create_layouts()
{
    this->layout = new QVBoxLayout();

    this->group2 = new QGroupBox("Select output type");
    this->hlayout1 = new QHBoxLayout();
    this->hlayout1->addWidget(this->asciiButton);
    this->hlayout1->addWidget(this->binaryButton);
    this->group2->setLayout(this->hlayout1);

    this->hlayout2 = new QHBoxLayout();
    this->hlayout2->addWidget(this->group2);
    this->hlayout2->addWidget(this->selectFileButton);

    this->vlayout1 = new QVBoxLayout();
    this->vlayout1->addLayout(this->hlayout2);
    this->vlayout1->addWidget(this->filenameLineEdit);

    this->group1 = new QGroupBox("Save output");
    this->group1->setLayout(this->vlayout1);

    this->hlayout3 = new QHBoxLayout();
    this->hlayout3->addWidget(this->convertCheckBox);
    this->hlayout3->addWidget(this->selectAsciiButton);
    this->vlayout2 = new QVBoxLayout();
    this->vlayout2->addLayout(this->hlayout3);
    this->vlayout2->addWidget(this->asciiNameLineEdit);

    this->group3 = new QGroupBox("Convert Binary to ASCII");
    this->group3->setLayout(this->vlayout2);

    this->hlayout4 = new QHBoxLayout();
    this->hlayout4->addWidget(this->acceptButton);
    this->hlayout4->addWidget(this->cancelButton);

    this->layout->addWidget(this->group1);
    this->layout->addWidget(this->group3);
    this->layout->addLayout(this->hlayout4);
    this->setLayout(this->layout);
}

void FileHandlerDialog::initialize_dialog()
{
    this->create_buttons();
    this->create_checks();
    this->create_displays();
    this->create_layouts();

    this->connect_buttons_to_slots();

    this->binaryButton->setChecked(false);
    this->asciiButton->setChecked(true);

    this->setWindowTitle("File Handler");
    this->resize(480, 320);

    // clear QString variables
    this->body.clear();
    this->date.clear();
    this->ext.clear();
    this->saveFilename.clear();
    this->convertFilename.clear();

    this->verbose = true;

    this->body = "OutputData-";

    this->date = QDateTime::currentDateTime().toString();
    this->date.replace(' ', '-');
    this->date.replace(':', '.');

    this->ext = ".csv"; // defaults to ascii mode

    this->saveFilename = this->body + this->date + this->ext;
    this->filenameLineEdit->setText(this->saveFilename);
    this->convertFilename = this->body + this->date + "_toAscii.csv";
    this->asciiNameLineEdit->setText(this->convertFilename);
}

void FileHandlerDialog::save_fileHandler()
{
    if (this->binaryButton->isChecked())
    {
        this->fileHandler->set_binary(true);
        if (this->verbose)
        {
            qDebug() << "saving data to binary format";
            this->ext = ".dat";
        }
    }
    else
    {
        this->fileHandler->set_binary(false);
        if (this->verbose)
        {
            qDebug() << "saving data to ascii format";
            this->ext = ".csv";
        }
    }

    if (this->saveFilename.isEmpty())
    {
        this->saveFilename = this->body + this->date + this->ext;

        qDebug() << "defaulting to " << this->saveFilename;
        this->filenameLineEdit->setText(this->saveFilename);
        this->fileHandler->set_filename(this->saveFilename.toStdString());
    }
    else
    {
        this->fileHandler->set_filename(this->saveFilename.toStdString());
        if (this->verbose)
        {
            qDebug() << "Saving data to " << this->saveFilename;
        }
    }

    if (this->convertCheckBox->isChecked())
    {
        this->fileHandler->set_convert(true);
        if (this->convertFilename.isEmpty())
        {
            this->convertFilename = this->body + this->date + "_toAscii.csv";

            qDebug() << "Converting binary to ascii ... no file selected ... defaulting to " << this->convertFilename;
            this->asciiNameLineEdit->setText(this->convertFilename);
            this->fileHandler->set_asciiFilename(this->convertFilename.toStdString());
        }
        else
        {
            this->fileHandler->set_asciiFilename(this->convertFilename.toStdString());
            if (this->verbose)
            {
                qDebug() << "Converting binary to ascii ... saving data to " << this->convertFilename;
            }
        }
    }
    else
    {
        this->fileHandler->set_convert(false);
        if (this->verbose)
        {
            qDebug() << "no ascii conversion requested";
        }
    }
}

void FileHandlerDialog::set_fileHandler(FileHandler *fh)
{
    this->fileHandler = fh;
}

