#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fc_widget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QtXml>
#include <QFileDialog>
#include <QTextBrowser>
//#include <QWebView>
#include "testwindow.h"
#include "exercisewindow.h"
#include "qwordcompleter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //void saveFlowChart(Block_Widget *fc);
    QDomElement getBlocks(const Block_Widget *fc, QDomDocument &doc, QString Name = "flowchart");
    void makeFlowChart();
    void loadFlowChart(Block_Widget *widget, QDomNode &node, bool isMain = false, bool delBottom = true);
    bool isCorrectedPunctuation();

public slots:
    //void saveSlot();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
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
    void changeFunctionName(QString name);
    void translateBack();

private:
    bool pressed;
    bool isToShowCode;
    Block_Widget *wdg;
    QVBoxLayout *fcvLayout;
    Ui::MainWindow *ui;
    TestWindow *test;
    ExerciseWindow *exercise;
    bool openFromNewLine;
    QTextBrowser *theoryBrowser;
    QString theory;
    QString html;
    QWordCompleter* completer;

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
