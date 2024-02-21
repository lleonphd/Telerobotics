/**
  * @file FileHandlerDialog.h
  * @brief Declaration of the FileHandlerDialog class
  */


#ifndef _FILE_HANDLER_DIALOG_H_
#define _FILE_HANDLER_DIALOG_H_

#include <QCheckBox>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "FileHandler/FileHandler.h"

/// Graphical client for FileHandler class
/**
  * Graphical class used as a client for FileHandler
  * this class is developed with Qt framework
  */
class FileHandlerDialog : public QDialog
{
    Q_OBJECT

private:
    QCheckBox * convertCheckBox;

    QGroupBox * group1;
    QGroupBox * group2;
    QGroupBox * group3;

    QHBoxLayout * hlayout1;
    QHBoxLayout * hlayout2;
    QHBoxLayout * hlayout3;
    QHBoxLayout * hlayout4;
    QHBoxLayout * hlayout5;
    QHBoxLayout * hlayout6;
    QHBoxLayout * hlayout7;
    QHBoxLayout * hlayout8;



    QLineEdit * filenameLineEdit;
    QLineEdit * asciiNameLineEdit;

    QPushButton * acceptButton;
    QPushButton * cancelButton;
    QPushButton * selectFileButton;
    QPushButton * selectAsciiButton;

    QRadioButton * asciiButton;
    QRadioButton * binaryButton;

    QString body;
    QString date;
    QString ext;
    QString saveFilename;
    QString convertFilename;

    QVBoxLayout * layout;
    QVBoxLayout * vlayout1;
    QVBoxLayout * vlayout2;
    QVBoxLayout * vlayout3;
    QVBoxLayout * vlayout4;

    FileHandler *fileHandler;

    bool verbose;

    void connect_buttons_to_slots();

    void create_buttons();
    void create_checks();
    void create_displays();
    void create_layouts();

    void initialize_dialog();

    void set_fileHandler(FileHandler *fh);

public:
    FileHandlerDialog(FileHandler *fh, QWidget *parent = 0);
    ~FileHandlerDialog();

public slots:
    bool browse();
    bool browse_ascii();
    void save_fileHandler();
};

#endif // _FILE_HANDLER_DIALOG_H_
