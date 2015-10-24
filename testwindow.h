#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <fstream>
#include <string>

namespace Ui {
class TestWindow;
}

class TestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = 0);
    void startTest();
    void stopTest();
    void findQuest(std::istream &input, int qts);
    void showQuest(std::istream &input, int qts);
    std::string encreapt(std::string text, int key);
    std::string decreapt(std::string code, int key);
    ~TestWindow();
    void closeEvent(QCloseEvent *);
    int num_of_questions(std::istream &input);
    void saveResult();
    void showResult();
    QString getResult();
public slots:
    void stop();
    void next();
signals:
    void closed();

private:
    Ui::TestWindow *ui;
    int qts;
    int questions;
    int done;
    int true_ans;
    int num_of_trues;
};

#endif // TESTWINDOW_H
