#ifndef EXERCISEWINDOW_H
#define EXERCISEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include "fc_widget.h"
#include <QtXml>
#include <ctime>
#include <vector>

namespace Ui {
class ExerciseWindow;
}

class ExerciseWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ExerciseWindow(QWidget *parent = 0);
    ~ExerciseWindow();
    void closeEvent(QCloseEvent *);
    void makeFlowChart();
    QDomElement getBlocks(const Block_Widget *fc, QDomDocument &doc, QString Name = "flowchart");
    void toCode();
    void XMLtoCode(QTextStream &text, QDomNode &node, int num_of_spaces);
    void generateSourceCode();
    void generateXML(QDomElement &domElement, QDomDocument &doc);
    void generateVars();
signals:
    void closed();
public slots:
    void checkExercise();
    void changeFontSize(int size);
private:
    Ui::ExerciseWindow *ui;
    Block_Widget *wdg;
    QVBoxLayout *fcvLayout;
    std::string var_abc;
    bool openFromNewLine;
    QDomDocument doc;
    QVector<QString> vars;

    /*enum var_name{
        x, y, z, i, j
    } varName;*/
    /*enum block_type{
        BlockAction_Widget, BlockInput_Widget, BlockOutput_Widget,
        Divar_Widget, Prefix_Cycle, Postfix_Cycle, Parameter_Cycle
    } blockType;*/
};

#endif // EXERCISEWINDOW_H
