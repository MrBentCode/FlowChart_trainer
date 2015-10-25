#include "inputdialog.h"


InputDialog::InputDialog(QString text, QWidget* parent/* = 0*/): QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    this->setModal(true);
    this->setWindowTitle("Редактирование блока");
    line = new QLineEdit;
    line->setText(text);
    QLabel* label = new QLabel("&Text");
    label->setBuddy(label);

    line->setMaxLength(20);
    QPushButton* pcmdOk     = new QPushButton("&Ok");
    QPushButton* pcmdCancel = new QPushButton("&Cancel");
    pcmdDelete = new QPushButton("Удалить блок");

    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(pcmdDelete, SIGNAL(clicked()), SLOT(reject()));

    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(label, 0, 0);
    ptopLayout->addWidget(line, 0, 1);
    ptopLayout->addWidget(pcmdOk, 1, 0);
    ptopLayout->addWidget(pcmdCancel, 1, 1);
    ptopLayout->addWidget(pcmdDelete, 1, 2);
    setLayout(ptopLayout);
}

QString InputDialog::getLine() const
{
    return line->text();
}
