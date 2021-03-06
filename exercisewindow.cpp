#include "exercisewindow.h"
#include "ui_exercisewindow.h"

ExerciseWindow::ExerciseWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExerciseWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(false);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->scrollArea->setAutoFillBackground(true);
    ui->scrollArea->setPalette(Pal);
    openFromNewLine = ui->checkBox_newLine->isChecked();
    srand((unsigned)time (0));
    makeFlowChart();
    QString name = windowTitle();
    doc = QDomDocument("flowchart");
    generateVars();
    QDomElement domElement = doc.createElement(name);
    generateXML(domElement, doc);
    doc.appendChild(domElement);
    ui->textBrowser->setFont(QFont("Courier", 10));
    ui->textBrowser_2->setFont(QFont("Courier", 10));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(checkExercise()));
    connect(ui->checkBox_newLine, SIGNAL(clicked()), SLOT(checkExercise()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), SLOT(changeFontSize(int)));
    connect(ui->checkBox, SIGNAL(toggled(bool)), wdg, SLOT(HideTexts(bool)));
}

ExerciseWindow::~ExerciseWindow()
{
    delete ui;
}

void ExerciseWindow::closeEvent(QCloseEvent *)
{
    emit(closed());
}

void ExerciseWindow::makeFlowChart()
{
    wdg = new FCWidget(this);
    ui->scrollArea->setWidget(wdg);
    fcvLayout = new QVBoxLayout();
    fcvLayout->setSpacing(0);
    wdg->setLayout(fcvLayout);
    fcvLayout->insertWidget(0, new FCBeginWidget(wdg));
    FCLine *line = new FCLine(wdg);
    fcvLayout->insertWidget(1, line);
    fcvLayout->insertWidget(2, new FCEndWidget(wdg));
    fcvLayout->setAlignment(Qt::AlignHCenter);
    wdg->UpdateSize();

    connect(line, SIGNAL(clicked(FCLine*)), line, SLOT(lineclicked(FCLine*)));
    connect(ui->radio_action, SIGNAL(clicked()), wdg, SLOT(ClickedAction()));
    connect(ui->radio_input, SIGNAL(clicked()), wdg, SLOT(ClickedInput()));
    connect(ui->radio_output, SIGNAL(clicked()), wdg, SLOT(ClickedOutput()));
    connect(ui->radio_Divar, SIGNAL(clicked()), wdg, SLOT(ClickedDivar()));
    connect(ui->radio_prefix_cycle, SIGNAL(clicked()), wdg, SLOT(ClickedPrefix()));
    connect(ui->radio_param_cycle, SIGNAL(clicked()), wdg, SLOT(ClickedParameter()));
    connect(ui->radio_postfix_cycle, SIGNAL(clicked()), wdg, SLOT(ClickedPostfix()));
}

QDomElement ExerciseWindow::getBlocks(const FCWidget *fc, QDomDocument &doc, QString Name)
{
    int n = fc->layout()->count();
    int i = 0;
    QDomElement domElement = doc.createElement(Name);
    while (i < n) {
        QDomElement block;
        QDomAttr attr = doc.createAttribute("text");
        QString tag = fc->layout()->itemAt(i)->widget()->metaObject()->className();
        if (tag != "FCLine" && tag != "FCLeftLine" && tag != "RightLine") {
            block = doc.createElement(tag);
            FCWidget *widget = (FCWidget*)fc->layout()->itemAt(i)->widget();
            attr.setValue(widget->getText());
            block.setAttributeNode(attr);
            domElement.appendChild(block);
            if (tag == "FCDivarWidget") {
                FCDivarWidget *divar = (FCDivarWidget*)widget;
                QDomElement left = getBlocks(divar->getRightLine(), doc, "Left");
                QDomElement right = getBlocks(divar->getLeftLine(), doc, "Right");
                block.appendChild(left);
                block.appendChild(right);
            }
            else if (tag == "FCPrefixCycleWidget") {
                FCPrefixCycleWidget *cycle = (FCPrefixCycleWidget*)widget;
                QDomElement body = getBlocks(cycle->getCycleBody(), doc, "Body");
                block.appendChild(body);
            }
            else if (tag == "FCPostfixCycleWidget") {
                FCPostfixCycleWidget *cycle = (FCPostfixCycleWidget*)widget;
                QDomElement body = getBlocks(cycle->getCycleBody(), doc, "Body");
                block.appendChild(body);
            }
            else if (tag == "FCParameterCycleWidget") {
                FCParameterCycleWidget *cycle = (FCParameterCycleWidget*)widget;
                QDomElement body = getBlocks(cycle->getCycleBody(), doc, "Body");
                block.appendChild(body);
            }
        }
        i++;
    }
    return domElement;
}

void ExerciseWindow::toCode()
{
    QDomDocument doc("flowchart");

    QDomElement domElement = getBlocks(wdg, doc);
    doc.appendChild(domElement);
    //qDebug()<<doc.toString();

    QTextStream text;
    QString line_text;
    text.setString(&line_text);

    //text.setDevice(doc.toString());
    QString name = windowTitle();

    int n = name.indexOf(".xml");
    if (n > 0) {
        name = QString::fromStdString((name.toStdString()).substr(0, name.length() - 4));
    }
    text << "void " << name << "()";
    XMLtoCode(text, domElement, 5);

    ui->textBrowser_2->setPlainText(text.readAll());
}

void ExerciseWindow::XMLtoCode(QTextStream &text, QDomNode &node, int num_of_spaces)
{
    QString space = "";
    for (int i = 0; i < num_of_spaces; i++){
        space += " ";
    }
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()) {
        if (domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if (!domElement.isNull()) {
                //if (domElement.tagName() == "Body") loadFlowChart(widget, domElement);
                if (domElement.tagName() == "Left" || domElement.tagName() == "Right"
                       || domElement.tagName() == "Body") XMLtoCode(text, domElement, num_of_spaces);

                if (domElement.tagName() != "FCLine" && domElement.tagName() != "Left" &&
                    domElement.tagName() != "Right" && domElement.tagName() != "Body") {

                    QString line = domElement.attribute("text");

                    if (domElement.tagName() == "FCBeginWidget") {
                        if (openFromNewLine) text << endl << "{" << endl;
                        else text << " {" << endl;
                        for (int i = 0; i < vars.size(); i++){
                            QString variable = vars[i];
                            text << space + "int " + variable + " = 0;" << endl;
                        }
                    }
                    else if (domElement.tagName() == "FCActionWidget") text << space + line + ";" << endl;
                    else if (domElement.tagName() == "FCInputWidget") text << space + "cin >> " + line + ";" << endl;
                    else if (domElement.tagName() == "FCOutputWidget") text << space + "cout << " + line + ";" << endl;
                    else if (domElement.tagName() == "FCDivarWidget") {
                        text << space + "if (" + line + ")";
                        if (openFromNewLine) text << endl << space + "{" << endl;
                        else text << " {" << endl;
                        XMLtoCode(text, domElement, num_of_spaces + 5);
                        text << space + "}" << endl;
                    }
                    else if (domElement.tagName() == "FCPrefixCycleWidget") {
                        text << space + "while (" + line + ")";
                        if (openFromNewLine) text << endl << space + "{" << endl;
                        else text << " {" << endl;
                        XMLtoCode(text, domElement, num_of_spaces + 5);
                        text << space + "}" << endl;
                    }
                    else if (domElement.tagName() == "FCPostfixCycleWidget") {
                        text << space + "do";
                        if (openFromNewLine) text << endl << space + "{" << endl;
                        else text << " {" << endl;
                        XMLtoCode(text, domElement, num_of_spaces + 5);
                        text << space + "} while (" + line + ");" << endl;
                    }
                    else if (domElement.tagName() == "FCParameterCycleWidget") {
                        text << space + "for (" + line + ")";
                        if (openFromNewLine) text << endl << space + "{" << endl;
                        else text << " {" << endl;
                        XMLtoCode(text, domElement, num_of_spaces + 5);
                        text << space + "}" << endl;
                    }

                    else if (domElement.tagName() == "FCEndWidget") text << "}" << endl;
                }
            }
            if (domElement.tagName() == "Left") {
                space = "";
                for (int i = 0; i < num_of_spaces - 5; i++){
                    space += " ";
                }
                text << space + "} else";
                if (openFromNewLine) text << endl << space + "{" << endl;
                else text << " {" << endl;
            }
        }
        domNode = domNode.nextSibling();
    }
}

void ExerciseWindow::generateSourceCode()
{
    QString name = windowTitle();
    /*
    QDomDocument doc("flowchart");

    QDomElement domElement = doc.createElement(name);
    generateXML(domElement, doc);
    doc.appendChild(domElement);
    //qDebug()<<doc.toString();
*/
    QDomElement domElement = doc.firstChildElement();
    QTextStream text;
    QString line_text;
    text.setString(&line_text);

    //text.setDevice(doc.toString());


    text << "void " << name << "()";
    XMLtoCode(text, domElement, 5);

    ui->textBrowser->setPlainText(text.readAll());
}

void ExerciseWindow::generateXML(QDomElement &domElement, QDomDocument &doc)
{
    int length = rand()%10+5;
    int choice = 0;
    int var_choice = 0;
    int i = 0;
    QDomElement block = doc.createElement("FCBeginWidget");
    QString sAttr = "operator()";
    QDomAttr attr = doc.createAttribute("text");
    domElement.appendChild(block);

    while (i < length){
        choice = rand()%7;
        attr = doc.createAttribute("text");
        var_choice = rand()%vars.size();
        QString var = vars[var_choice];
        switch(choice){
        case 0:{
            block = doc.createElement("FCActionWidget");
            sAttr = var + " = " + QString::number(rand()%10);
            attr.setValue(sAttr);

        }break;
        case 1:{
            block = doc.createElement("FCInputWidget");
            attr.setValue(var);
        }break;
        case 2:{
            block = doc.createElement("FCOutputWidget");
            attr.setValue(var);
        }break;
        case 3:{
            block = doc.createElement("FCDivarWidget");
            QString decision = var + " == " + QString::number(5);
            if (i%2 == 0) decision = var + " < " + QString::number(10);
            else decision = var + " > " + QString::number(0);
            attr.setValue(decision);
            QDomElement left = doc.createElement("Left");
            QDomElement right = doc.createElement("Right");
            block.appendChild(left);
            block.appendChild(right);
            QDomElement blockTrue = doc.createElement("FCActionWidget");
            QDomElement blockFalse = doc.createElement("FCActionWidget");
            QDomAttr attribute = doc.createAttribute("text");
            attribute.setValue(var + "++");
            blockTrue.setAttributeNode(attribute);
            attribute = doc.createAttribute("text");
            attribute.setValue(var + "--");
            blockFalse.setAttributeNode(attribute);
            left.appendChild(blockTrue);
            right.appendChild(blockFalse);
        }break;
        case 4:{
            block = doc.createElement("FCPrefixCycleWidget");
            QString decision = var + " < " + QString::number(i * 5);
            attr.setValue(decision);
            QDomElement body = doc.createElement("Body");
            block.appendChild(body);
            QDomElement blockTrue = doc.createElement("FCActionWidget");
            QDomAttr attribute = doc.createAttribute("text");
            attribute.setValue(var + "++");
            blockTrue.setAttributeNode(attribute);
            body.appendChild(blockTrue);
        }break;
        case 5:{
            block = doc.createElement("FCPostfixCycleWidget");
            QString decision = var + " < " + QString::number(i * 5);
            attr.setValue(decision);
            QDomElement body = doc.createElement("Body");
            block.appendChild(body);
            QDomElement blockTrue = doc.createElement("FCActionWidget");
            QDomAttr attribute = doc.createAttribute("text");
            attribute.setValue(var + "++");
            blockTrue.setAttributeNode(attribute);
            body.appendChild(blockTrue);
        }break;
        case 6:{
            block = doc.createElement("FCParameterCycleWidget");
            QString decision = "int i = 0; i < " + QString::number(i * 5) + "; i++";
            attr.setValue(decision);
            QDomElement body = doc.createElement("Body");
            block.appendChild(body);
            QDomElement blockTrue = doc.createElement("FCActionWidget");
            QDomAttr attribute = doc.createAttribute("text");
            attribute.setValue(var + "++");
            blockTrue.setAttributeNode(attribute);
            body.appendChild(blockTrue);
        }
        }
        block.setAttributeNode(attr);
        domElement.appendChild(block);
        i++;
    }
    block = doc.createElement("FCEndWidget");
    domElement.appendChild(block);
}

void ExerciseWindow::generateVars()
{
    int n = rand()%4 + 1;
    QString var_alphabet = "xyab";
    for (int i = 0; i < n; i++){
        vars.push_back(QString::fromStdString(var_alphabet.toStdString().substr(i, 1)));
    }
}

void ExerciseWindow::checkExercise()
{
    openFromNewLine = ui->checkBox_newLine->isChecked();
    generateSourceCode();
    toCode();
    QString etalon = ui->textBrowser->toPlainText();
    QString userAnswer = ui->textBrowser_2->toPlainText();
    if (etalon == userAnswer){
        ui->label->setStyleSheet("QLabel { background-color : green; color : white; }");
        ui->label->setText("Верно!");
    }
    else {
        ui->label->setStyleSheet("QLabel { background-color : red; color : white; }");
        ui->label->setText("Неверно!");
    }
}

void ExerciseWindow::changeFontSize(int size)
{
    ui->textBrowser->setFont(QFont("Courier", size));
    ui->textBrowser_2->setFont(QFont("Courier", size));
}
