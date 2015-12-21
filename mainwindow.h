#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "fc_widget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QtXml>
#include <QFileDialog>
#include <QTextBrowser>
#include <QRadioButton>
//#include <QWebView>
#include "testwindow.h"
#include "exercisewindow.h"
//#include "qwordcompleter.h"
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //void saveFlowChart(FCWidget *fc);
    QDomElement getBlocks(const FCWidget *fc, QDomDocument &doc, QString Name = "flowchart");
    void makeFlowChart();
    void loadFlowChart(FCWidget *widget, QDomNode &node, bool isMain = false, bool delBottom = true);
    bool isCorrectedPunctuation();
    //QDomElement codeToXML(QString text, QDomDocument &doc, QDomElement &domElement, QVector<QString> vector);
public slots:
    //void saveSlot();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void onRadioButtonCheck();
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void savePicture();
    void about();
    void documentWasModified();
    void translate();
    void showCodeWidget();
    void saveSourceCode();
    void changeFontSize(int size);
    void getTested();
    void endTest();
    void startExercise();
    void endExercise();
    void showTheory();
    void build();
    void buildAndRun();
    //void changeFunctionName(QString name);
    //void translateBack();

private:
    bool pressed;
    bool isToShowCode;
    FCWidget *wdg;
    QVBoxLayout *fcvLayout;
    Ui::MainWindow *ui;
    TestWindow *test;
    ExerciseWindow *exercise;
    bool openFromNewLine;
    QTextBrowser *theoryBrowser;
    QString theory;
    QString html;
    QString programName;
    //QWordCompleter* completer;
    QVector<QRadioButton *> radioButtons;

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void toCode();
    void XMLtoCode(QTextStream &text, QDomNode &node, int num_of_spaces);

    //QPlainTextEdit *textEdit;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *savePix;
    QAction *saveCode;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *passTest;
    QAction *doExercise;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *aboutFlowCharts;
    QString myName, myOrganization, myApplicationName, programVersion;
};

#endif // MAINWINDOW_H
