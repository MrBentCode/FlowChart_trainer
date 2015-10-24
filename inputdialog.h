#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

class InputDialog: public QDialog
{
    Q_OBJECT
private:
    QLineEdit* line;
public:
    InputDialog(QString text = "", QWidget* parent = 0);
    QString getLine() const;
    QPushButton *pcmdDelete;
};

#endif // INPUTDIALOG_H
