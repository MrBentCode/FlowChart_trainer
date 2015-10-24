#include "testwindow.h"
#include "ui_testwindow.h"
#include <QUrl>
#include <QDesktopServices>
//#include <QDebug>
#include <ctime>

TestWindow::TestWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    std::ifstream input;
    QString path=QApplication::applicationDirPath()+"/Test.txt";
    input.open(path.toLocal8Bit().data());
    if (input.is_open()){
        questions = num_of_questions(input);
        input.close();
    }
    done = 0;
    //ui->progressBar->setMaximum(questions);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(next()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(stop()));
}

void TestWindow::startTest()
{

    std::ifstream input;
    QString path=QApplication::applicationDirPath()+"/Test.txt";
    input.open(path.toLocal8Bit().data());
    if (input.is_open()){
        qts = 1;
        num_of_trues = 0;
        showQuest(input, qts);
        qts++;
        input.close();
    }
    else {
        ui->textBrowser->append("Отсутствует файл с тестовыми вопросами.");
    }
}

void TestWindow::stopTest()
{
    this->close();
    //this->destroy();
}

void TestWindow::findQuest(std::istream &input, int qts)
{
    int i = 1;
    qts--;
    qts*=7;
    std::string str;
    while (i <= qts)
    {
        getline(input, str);
        i++;
    }
}

void TestWindow::showQuest(std::istream &input, int qts)
{
    if (qts <= 5 && qts <= questions){
        int i = 1;
        qts--;
        qts*=6;
        std::string str;
        QString STR;
        while (i <= qts)
        {
            std::getline(input, str);
            i++;
        }
        i = 0;
        while (i < 5)
        {
            getline (input, str);
            //str = decreapt(str, 2);
            str = decreapt(str, 5);
            STR = QString::fromStdString(str);
            ui->textBrowser->append(STR);
            i++;
        }
        std::getline (input, str);
        str = decreapt(str, 5);
        true_ans = atoi(str.c_str());
    }
}

std::string TestWindow::encreapt(std::string text, int key)
{
    std::string code = "";
    for(int i = 0; i < text.length(); i++)
        {
            code += text[i] + key;
        }
    return code;
}

std::string TestWindow::decreapt(std::string code, int key)
{
    std::string text = "";
    for(int i = 0; i < code.length(); i++)
        {
            text += code[i] - key;
        }
    return text;
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::closeEvent(QCloseEvent *)
{
    emit(closed());
}

int TestWindow::num_of_questions(std::istream &input)
{
    int count = 0;
    std::string str;
    while(!input.eof()){
        std::getline(input, str);
        count++;
    }
    return count / 6;
}

void TestWindow::saveResult()
{
    std::ofstream output;
    QString path=QApplication::applicationDirPath()+"/Result.txt";
    output.open(path.toLocal8Bit().data(), std::ios_base::app);
    char buffer[80];
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    //char* format = "%A, %B %d, %Y %H:%M:%S";
    strftime(buffer, 80, "%A, %B %d, %Y %H:%M:%S", timeinfo);
    output<<"---------------------------------------"<<std::endl;
    output<<buffer<<std::endl;
    output<< getResult().toStdString() <<std::endl;
    output.close();
}

void TestWindow::showResult()
{
    ui->textBrowser->clear();
    ui->textBrowser->append(getResult());
}

QString TestWindow::getResult()
{
    return "Результаты теста:\nКоличество вопросов: " + QString::number(done)
            + "\nКоличество правильных ответов: " + QString::number(num_of_trues)
            + "\nОценка: " + QString::number(num_of_trues);
}

void TestWindow::stop()
{
    stopTest();
}

void TestWindow::next()
{
    if (done < 5){
        ui->progressBar->setValue(ui->progressBar->value() + 1);
        int answer = 0;
        if (ui->radioButton->isChecked()) answer = 1;
        else if (ui->radioButton_2->isChecked()) answer = 2;
        else if (ui->radioButton_3->isChecked()) answer = 3;
        else if (ui->radioButton_4->isChecked()) answer = 4;
        done++;
        if (answer == true_ans){
            num_of_trues++;
            ui->textBrowser->append("Правильно!");
        }
        ui->textBrowser->append("");
        std::ifstream input;
        QString path=QApplication::applicationDirPath()+"/Test.txt";
        input.open(path.toLocal8Bit().data());
        showQuest(input, qts);
        qts++;
        input.close();
    }
    if (done == 5) {
        showResult();
        saveResult();
        ui->widget_3->hide();
    }
}
