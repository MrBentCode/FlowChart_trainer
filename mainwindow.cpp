#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <QFile>

QString line_stream(std::string tempLine, bool in){
    QString str;
    //std::string tempLine = line.toStdString();
    std::string tempStr = tempLine;
    int n = 0;
    int m = -1;
    m = tempStr.find("\"");
    //qDebug()<<QString::number(m);
    if (!(m >= 0 && m < tempStr.length()) || in) {
        //qDebug()<<"WELL";
        while (tempLine.find(",") > 0 && tempLine.find(",") < tempLine.size()){
            n = tempLine.find(",");
            if (in) tempStr = tempLine.replace(n, 1, " >>");
            else tempStr = tempLine.replace(n, 1, " <<");
        }
    }

    str = QString::fromStdString(tempStr);
    return str;
}

/*std::string encreapt(std::string text, int key)
{
    std::string code = "";
    for(int i = 0; i < text.length(); i++)
        {
            code += text[i] + key;
        }
    return code;
}

void encodeQuestions(){
    std::ifstream input;
    std::ofstream output;
    QString path = QApplication::applicationDirPath()+"/Source.txt";
    input.open(path.toLocal8Bit().data());
    path = QApplication::applicationDirPath()+"/Test.txt";
    if (input.is_open()){
        output.open(path.toLocal8Bit().data());
        std::string line;
        while (!input.eof()){
            std::getline(input, line);
            line = encreapt(line, 5);
            output << line << std::endl;
        }
        output.close();
        input.close();
    }
}*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(false);
    isToShowCode = false;
    //ui->translate_button->hide();
    ui->widget_3->hide();
    //ui->textEdit->hide();
    ui->code_viewer_widget->hide();
    myName = "Есеркепов Андрей";
    programVersion = "0.9";
    openFromNewLine = ui->checkBox_newLine->isChecked();
    ui->textEdit->setFont(QFont("Courier", 10));
    //encodeQuestions();

    createActions(); //Для разгрузки конструктора создадим действия (команды меню),
    createMenus(); //главное меню,
    createToolBars(); //кнопочные панели инструментов и
    createStatusBar(); //строку состояния в отдельных функциях

    setCurrentFile(""); //Установить текущий файл, в начале - пусто

    theoryBrowser = NULL;
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->scrollArea->setAutoFillBackground(true);
    ui->scrollArea->setPalette(Pal);
    ui->translate_button_back->hide();

/*
    QStringList keywords;
    keywords << "int" << "double" << "void" << "cin" << "cout" << "if";
    completer = new QWordCompleter(keywords, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWidget(ui->textEdit);
    connect(completer, SIGNAL(activated(QString)), completer, SLOT(replaceCurrentWord(QString)));
*/

    makeFlowChart();
    //connect(wdg, SIGNAL(modified()), this, SLOT(translate()));
    connect(ui->show_hide_button, SIGNAL(clicked()), SLOT(showCodeWidget()));
    connect(ui->translate_button, SIGNAL(clicked()), SLOT(translate()));
    connect(ui->checkBox_newLine, SIGNAL(clicked()), SLOT(translate()));
    connect(ui->code_font_slider, SIGNAL(valueChanged(int)), SLOT(changeFontSize(int)));
    connect(ui->checkBox, SIGNAL(toggled(bool)), wdg, SLOT(HideTexts(bool)));
    connect(this, SIGNAL(windowTitleChanged(QString)), SLOT(changeFunctionName(QString)));
    connect(ui->translate_button_back, SIGNAL(clicked()), SLOT(translateBack()));

    radioButtons.push_back(ui->radio_action);
    radioButtons.push_back(ui->radio_Divar);
    radioButtons.push_back(ui->radio_input);
    radioButtons.push_back(ui->radio_output);
    radioButtons.push_back(ui->radio_param_cycle);
    radioButtons.push_back(ui->radio_postfix_cycle);
    radioButtons.push_back(ui->radio_prefix_cycle);

    for(int i = 0; i < radioButtons.size(); i++) {
        radioButtons[i]->setStyleSheet("QRadioButton::indicator { width: 0px; height: 0px }");
    }

    connect(ui->radio_action, SIGNAL(released()), this, SLOT(onRadioButtonCheck()));
    connect(ui->radio_Divar, SIGNAL(released()), this, SLOT(onRadioButtonCheck()));
    connect(ui->radio_input, SIGNAL(released()), this, SLOT(onRadioButtonCheck()));
    connect(ui->radio_output, SIGNAL(released()), this, SLOT(onRadioButtonCheck()));
    connect(ui->radio_param_cycle, SIGNAL(released()), this, SLOT(onRadioButtonCheck()));
    connect(ui->radio_postfix_cycle, SIGNAL(released()), this, SLOT(onRadioButtonCheck()));
    connect(ui->radio_prefix_cycle, SIGNAL(released()), this, SLOT(onRadioButtonCheck()));
    connect(ui->compilerButton, SIGNAL(clicked()), SLOT(build()));
    connect(ui->runButton, SIGNAL(clicked()), SLOT(buildAndRun()));

}

void MainWindow::onRadioButtonCheck() {
    for (int i = 0; i < radioButtons.size(); i++) {
        if (radioButtons[i]->isChecked()) {
            radioButtons[i]->setStyleSheet("QRadioButton::indicator { width: 0px; height: 0px } "
                                           "QRadioButton { border: 3px solid #aaa; } ");
        }
        else {
            radioButtons[i]->setStyleSheet("QRadioButton::indicator { width: 0px; height: 0px }");
        }
    }
    connect(ui->translate_button_back, SIGNAL(clicked()), SLOT(translateBack()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QDomElement MainWindow::getBlocks(const FCWidget *fc, QDomDocument& doc, QString Name)
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

void MainWindow::makeFlowChart()
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

void MainWindow::loadFlowChart(FCWidget *widget, QDomNode &node, bool isMain, bool delBottom)
{
    QVBoxLayout *layout = (QVBoxLayout*)widget->layout();
    int n = 1, m = 1;
    if (isMain) n = 0;
    if (!delBottom) {
        m++;
        n = 2;
    }
    while (layout->count() > n) delete layout->itemAt(layout->count()-m)->widget();
    QDomNode domNode = node.firstChild();
    int i = n;
    if (!delBottom) i--;
    FCWidget *block;
    while(!domNode.isNull()) {
        if (domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if (!domElement.isNull()) {
                //if (domElement.tagName() == "Body") loadFlowChart(widget, domElement);

                if (domElement.tagName() != "FCLine" && domElement.tagName() != "Left" &&
                    domElement.tagName() != "Right" && domElement.tagName() != "Body") {

                    QString text = domElement.attribute("text");

                    if (domElement.tagName() == "FCBeginWidget") block = new FCBeginWidget(widget);
                    else if (domElement.tagName() == "FCActionWidget") block = new FCActionWidget(widget, text);
                    else if (domElement.tagName() == "FCInputWidget") block = new FCInputWidget(widget, text);
                    else if (domElement.tagName() == "FCOutputWidget") block = new FCOutputWidget(widget, text);
                    else if (domElement.tagName() == "FCDivarWidget") block = new FCDivarWidget(widget, text);
                    else if (domElement.tagName() == "FCPrefixCycleWidget") block = new FCPrefixCycleWidget(widget, text);
                    else if (domElement.tagName() == "FCPostfixCycleWidget") block = new FCPostfixCycleWidget(widget, text);
                    else if (domElement.tagName() == "FCParameterCycleWidget") block = new FCParameterCycleWidget(widget, text);
                    if (domElement.tagName() != "FCEndWidget") widget->addBlock(i, block);
                    else if (domElement.tagName() == "FCEndWidget") {
                        block = new FCEndWidget(widget);
                        widget->addBlock(i, block, false);
                    }
                    i+=2;

                    if (domElement.tagName() == "FCDivarWidget") {
                        FCDivarWidget *divar = (FCDivarWidget*)block;
                        QDomNode leftNode = domElement.firstChild();
                        QDomElement leftElement = leftNode.toElement();
                        loadFlowChart(divar->getRightLine(), leftElement);
                        QDomNode rightNode = leftElement.nextSibling();
                        QDomElement rightElement = rightNode.toElement();
                        loadFlowChart(divar->getLeftLine(), rightElement);
                    }
                    else if (domElement.tagName() == "FCPrefixCycleWidget") {
                        FCPrefixCycleWidget *cycle = (FCPrefixCycleWidget*)block;
                        QDomElement body = domElement.firstChild().toElement();
                        qDebug()<<"CYCLE";
                        loadFlowChart(cycle->getCycleBody(), body, false, false);
                        //cycle->getCycleBody()->layout()->addWidget(new FCLeftLine(cycle->getCycleBody(), true));
                    }
                    else if (domElement.tagName() == "FCParameterCycleWidget") {
                        FCParameterCycleWidget *cycle = (FCParameterCycleWidget*)block;
                        QDomElement body = domElement.firstChild().toElement();
                        qDebug()<<"CYCLE";
                        loadFlowChart(cycle->getCycleBody(), body, false, false);
                        //cycle->getCycleBody()->layout()->addWidget(new FCLeftLine(cycle->getCycleBody(), true));
                    }
                    else if (domElement.tagName() == "FCPostfixCycleWidget") {
                        FCPostfixCycleWidget *cycle = (FCPostfixCycleWidget*)block;
                        QDomElement body = domElement.firstChild().toElement();
                        qDebug()<<"CYCLE";
                        loadFlowChart(cycle->getCycleBody(), body);
                        //cycle->getCycleBody()->layout()->addWidget(new FCLeftLine(cycle->getCycleBody(), true));
                    }
                }


            }



        }


        //
        domNode = domNode.nextSibling();
    }
}


bool MainWindow::isCorrectedPunctuation()
{
    int RBrack = 0, LBRack = 0, RFigureBrack = 0, LFigureBrack = 0;
    QString text = ui->textEdit->toPlainText();
    for(int i = 0; i < text.length(); i++)
    {
        if(text[i] == '(')
            LBRack++;
        else if(text[i] == ')')
            RBrack++;
        else if(text[i] == '{')
            LFigureBrack++;
        else if(text[i] == '}')
            RFigureBrack++;
    }
    return (LBRack == RBrack && LFigureBrack == RFigureBrack);
}
/*
QDomElement MainWindow::codeToXML(QString text, QDomDocument &doc, QDomElement &domElement, QVector<QString> vector)
{
    QDomElement element = doc.createElement("FCBeginWidget");
    QString line;
    domElement.appendChild(element);

    for (int i = 1; i < vector.length(); i++) {
        if (vector[i] != "") {
            QString tag;
            if (vector[i].indexOf("cin") != -1) {
                vector[i] = vector[i].replace(0, 6, "");
                tag = "FCInputWidget";
            }
            else if (vector[i].indexOf("cout") != -1) {
                vector[i] = vector[i].replace(0, 7, "");
                tag = "FCOutputWidget";
            }
            else {
                tag = "FCActionWidget";
            }
            QDomAttr attr = doc.createAttribute("text");
            line = vector[i];
            if (line.contains(";")) line = QString::fromStdString(line.toStdString().substr(0, line.length()-1));
            attr.setValue(line);
            element = doc.createElement(tag);
            element.setAttributeNode(attr);
            domElement.appendChild(element);
        }
    }
    element = doc.createElement("FCEndWidget");
    domElement.appendChild(element);
    return domElement;
}
*/
/*
void MainWindow::saveSlot()
{
    saveFlowChart(wdg);
}
*/

void MainWindow::createActions() //Создадим действия для пунктов меню
{
    newAct = new QAction(QIcon(":/resources/menu_icons/page.png"), tr("&Новый"), this);
     //Параметры конструктора QAction: иконка из ресурсов, текст меню, родитель
    newAct->setShortcuts(QKeySequence::New);
     //В классе QKeySequence уже определены стандартные комбинации клавиш
    newAct->setStatusTip(tr("Создать новый файл"));
     //Подсказка для строки состояния;
     //tr() - макрос, который может пригодиться
     //при автоматической локализации приложения
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
     //Назначили слот для обработки выбора пункта меню

    openAct = new QAction(QIcon(":/resources/menu_icons/folder.png"), tr("&Открыть..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Открыть существующий файл"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/resources/menu_icons/disk.png"), tr("&Сохранить"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить документ на диск"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/resources/menu_icons/disk.png"), tr("Сохранить к&ак..."), this);
     //Более простой конструктор QAction: текст меню, родитель
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Сохранить копию документа под новым именем"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    savePix = new QAction(QIcon(":/resources/menu_icons/photos.png"), tr("Сохранить картинку..."), this);
    savePix->setStatusTip(tr("Сохранить изображние блок-схемы"));
    connect(savePix, SIGNAL(triggered()), this, SLOT(savePicture()));

    saveCode = new QAction(QIcon(":/resources/menu_icons/plus.png"), tr("Сохранить код алгоритма..."), this);
    saveCode->setStatusTip(tr("Сохранить исходный код алгоритма"));
    connect(saveCode, SIGNAL(triggered()), this, SLOT(saveSourceCode()));

    exitAct = new QAction(tr("В&ыход"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Закрыть приложение"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    passTest = new QAction(QIcon(":/resources/menu_icons/monitor.png"), tr("Пройти тестирование..."), this);
    passTest->setStatusTip(tr("Пройти тестирование по теоретическим вопросам"));
    connect(passTest, SIGNAL(triggered()), this, SLOT(getTested()));

    doExercise = new QAction(QIcon(":/resources/menu_icons/pencilstraight.png"), tr("Упражнения..."), this);
    doExercise->setStatusTip(tr("Выполнить практическое задание"));
    connect(doExercise, SIGNAL(triggered()), this, SLOT(startExercise()));

    aboutFlowCharts = new QAction(tr("&О блок-схемах..."), this);
    aboutFlowCharts->setStatusTip(tr("Просмотреть теоретический материал по блок-схемам алгоритмов"));
    connect(aboutFlowCharts, SIGNAL(triggered()), this, SLOT(showTheory()));

    aboutAct = new QAction(tr("&О программе"), this);
    aboutAct->setStatusTip(tr("Информация о приложении"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("О &Qt"), this);
    aboutQtAct->setStatusTip(tr("Информация о библиотеке Qt"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
     //qApp - глобальный указатель на экземпляр приложения
     //у QWidget уже есть стандартный слот aboutQt()

    //cutAct->setEnabled(false); //Некоторые пункты меню сначала недоступны
    //copyAct->setEnabled(false);
    //connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    //connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
     //Слоты для управления состоянием этих пунктов
}

void MainWindow::createMenus() //Создание главного меню
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
     //В QMainWindow уже есть menuBar, просто добавим туда меню
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(savePix);
    fileMenu->addAction(saveCode);
    fileMenu->addAction(passTest);
    fileMenu->addAction(doExercise);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpMenu->addAction(aboutFlowCharts);
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars() //Создание кнопочных тулбаров
{
    fileToolBar = ui->mainToolBar;
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(savePix);
    fileToolBar->addAction(saveCode);
    fileToolBar->addAction(passTest);
    fileToolBar->addAction(doExercise);
    //Добавили команды, для которых определили QIcon
}

void MainWindow::createStatusBar() //Создание строки состояния
{
    statusBar()->showMessage(tr("Готово"));
}

bool MainWindow::maybeSave() //Есть ли несохранённыеи изменения?
{
    /*
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Приложение"),
                     tr("В документе есть несохранённые изменения.\n"
                        "Хотите сохранить документ?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) return save();
        else if (ret == QMessageBox::Cancel) return false;
    }
    return true;
    */
    return false;
}

void MainWindow::setCurrentFile(const QString &fileName)
//Управляет отображением имени текущего файла
{
    curFile = fileName;
    //textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled";
    setWindowFilePath(shownName);
     //В QWidget уже есть метод для связи пути к файлу с виджетом
    setWindowTitle(strippedName(shownName)); //лишнее
}

QString MainWindow::strippedName(const QString &fullFileName)
//Вернёт имя файла без пути
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::toCode()
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

    ui->textEdit->setPlainText(text.readAll());
}

void MainWindow::XMLtoCode(QTextStream &text, QDomNode &node, int num_of_spaces)
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
                    }
                    else if (domElement.tagName() == "FCActionWidget") text << space + line + ";" << endl;
                    else if (domElement.tagName() == "FCInputWidget") text << space + "cin >> " + line_stream(line.toStdString(), true) + ";" << endl;
                    else if (domElement.tagName() == "FCOutputWidget") text << space + "cout << " + line_stream(line.toStdString(), false) + ";" << endl;
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
                QDomNode tempNode = domNode.nextSibling();
                if (!tempNode.firstChild().isNull()) {
                    text << space + "} else";
                    if (openFromNewLine) text << endl << space + "{" << endl;
                    else text << " {" << endl;
                }

            }
        }
        domNode = domNode.nextSibling();
    }
}

void MainWindow::closeEvent(QCloseEvent *) //Обработчик закрытия приложения
{
    //if (theoryBrowser != NULL) theoryBrowser->close();
    /*
    if (maybeSave()) { //Если нужно сохранять,
        writeSettings(); //то сохранить и
        event->accept(); //подтвердить событие закрытия
    } else {
        event->ignore(); //иначе отменить обработку
    }
    */
}

void MainWindow::newFile() //Создаёт новый файл
{
    delete wdg;
    makeFlowChart();
    //textEdit->clear();
    setCurrentFile("");
    /*
    if (maybeSave()) {

    }
    */
}

void MainWindow::open() //Открывает существующий файл
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open FlowChart"), "", "*.xml");
    if (!fileName.isEmpty())
        loadFile(fileName);
    /*
    if (maybeSave()) {

    }
    */
}

bool MainWindow::save() //Обработка пункта меню "Сохранить"
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs() //Обработка пункта меню "Сохранить как"
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save FlowChart"), "FlowChart", "*.xml");
    if (fileName.isEmpty())
        return false;
    QString sEnding = QString::fromUtf8(fileName.toStdString().substr(fileName.length() - 4, 4).c_str());
    if (sEnding.contains(".xml")) return saveFile(fileName);
    else return saveFile(fileName + ".xml");

}

void MainWindow::savePicture()
{
    QString sFilter;
    QString Name = QFileDialog::getSaveFileName(0, tr("Save FlowChart"),
        QDir::currentPath(), "*.png", &sFilter);
    if (!Name.isEmpty()) {
        QPixmap pix = wdg->grab();
        QString sEnding = QString::fromUtf8(Name.toStdString().substr(Name.length() - 4, 4).c_str());
        //qDebug()<<sEnding;
        if (sEnding.contains(".png")) pix.save(Name);
        else pix.save(Name + ".png");
    }
}

bool MainWindow::saveFile(const QString &fileName) //Непосредственно сохранение файла
{
    QFile file(fileName); //Свяжем файловую переменную с именем файла

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("%1").arg(myName),
                             tr("Не могу сохранить файл %1\nПричина: %2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }



    QTextStream out(&file); //...и будем сохранять через поток...
    //...и, если надо, управлять курсором (менять на "песочные часы")
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QDomDocument doc(strippedName(fileName));

    QDomElement domElement = getBlocks(wdg, doc);
    doc.appendChild(domElement);
    out << doc.toString();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл сохранён"), 2000);
     //Покажем сообщение в строке статуса на 2 секунды
    return true;
}


void MainWindow::about() //Выводит окно "О программе"
{

   QMessageBox::about(this, tr("О программе"),
            tr("Данная программа представляет собой тренажер по работе с блок-схемами алгоритмов. \n"
       "Версия программного продукта: %2 \n"
       "Разработчик: %1").arg(myName, programVersion));

}

void MainWindow::documentWasModified() //Ставит метку модификации главного окна
{
    //setWindowModified(textEdit->document()->isModified());
}

void MainWindow::translate()
{
    openFromNewLine = ui->checkBox_newLine->isChecked();
    toCode();
}

void MainWindow::showCodeWidget()
{
    isToShowCode = !isToShowCode;
    if (isToShowCode){
        ui->widget_3->show();
        //ui->textEdit->show();
        ui->code_viewer_widget->show();
        ui->show_hide_button->setIcon(QIcon(":/resources/icon_arrow_right_1.png"));
    }
    else {
        ui->widget_3->hide();
        //ui->textEdit->hide();
        ui->code_viewer_widget->hide();
        ui->show_hide_button->setIcon(QIcon(":/resources/icon_arrow_left_1.png"));
    }
}

void MainWindow::saveSourceCode()
{
    QString sFilter;
    QString Name = QFileDialog::getSaveFileName(0, tr("Save Source Code"),
        QDir::currentPath(), "*.txt", &sFilter);
    if (!Name.isEmpty()) {
        QString text = ui->textEdit->toPlainText();
        QString sEnding = QString::fromLocal8Bit(Name.toStdString().substr(Name.length() - 4, 4).c_str());
        //qDebug()<<sEnding;
        std::ofstream out;
        if (sEnding.contains(".txt")) out.open(Name.toStdString().c_str());
        else out.open((Name + ".txt").toStdString().c_str());
        out << text.toStdString();
        out.close();
    }
}

void MainWindow::changeFontSize(int size)
{
    ui->textEdit->setFont(QFont("Courier", size));
}

void MainWindow::getTested()
{
    test = new TestWindow();
    connect(test, SIGNAL(closed()), SLOT(endTest()));
    test->show();
    test->startTest();
    this->hide();
}

void MainWindow::endTest()
{
    this->show();
    delete test;
}

void MainWindow::startExercise()
{
    exercise = new ExerciseWindow();
    connect(exercise, SIGNAL(closed()), SLOT(endExercise()));
    exercise->show();
    exercise->generateSourceCode();
    this->hide();
}

void MainWindow::endExercise()
{
    this->show();
    delete exercise;
}

void MainWindow::showTheory()
{
    if (theoryBrowser == NULL){
        theoryBrowser = new QTextBrowser();

        QString url = QApplication::applicationDirPath()+"/Theory.html";
        QFile theoryFile(url);
        if (theoryFile.open(QIODevice::ReadOnly)){
            html = theoryFile.readAll();
            theoryBrowser->setHtml(html);
        }
        theoryBrowser->show();
    }
    else {
        theoryBrowser->close();
        theoryBrowser->show();
    }

}

void MainWindow::build()
{
    translate();
    QProcess process;
    QString appPath = QApplication::applicationDirPath();
    appPath.replace("/", "\\");
    /*
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("TMPDIR", appPath+"\\temp"); // Add an environment variable
    env.insert("PATH", env.value("Path") + ";" + appPath+"\\mingw32\\bin");
    process.setProcessEnvironment(env);*/
    //QString url = QApplication::applicationDirPath() + "/mingw32/bin/g++";
    QString url = QApplication::applicationDirPath();


    QString fileName = windowTitle();
    qDebug()<<fileName;
    if (fileName.toStdString().substr(fileName.length()-4, 4) == ".xml"){
        qDebug()<<"that";
        fileName = QString::fromStdString(fileName.toStdString().substr(0, fileName.length()-4));
    }
    qDebug()<<fileName;
    programName = fileName + ".exe";
    QString string_include = "#include <iostream>\n#include <windows.h>";
    if (ui->checkBoxMath->isChecked()) string_include += "\n#include <cmath>";
    QString string_using = "using namespace std;";
    QString string_algorithm = ui->textEdit->toPlainText();
    QString string_functionName = fileName + "();";
    QString string_mainFunction = "int main(){\n\tSetConsoleCP(CP_UTF8);\n\tSetConsoleOutputCP(CP_UTF8);\n\t"
            +string_functionName+"\n\tsystem(\"Pause\");\n\treturn 0;\n}";

    fileName += ".cpp";
    QString logFileName = "compileroutput.txt";
    QString compilerParameters = "-static -static-libgcc -finput-charset=UTF-8 -g -Wall -o "
            + programName + " "
            + fileName + " 2> "
            + logFileName;
    QString forBatEncoding = "chcp 65001";
    QString forBatPath = "SET PATH=%PATH%;"
            + appPath + "\\mingw32\\bin\\";
    QString forBatBuild = "g++ " + compilerParameters;
    QString batFileName = "build.bat";

    std::ofstream outBat;
    outBat.open(batFileName.toStdString().c_str());
    outBat << forBatEncoding.toStdString() << std::endl <<
              forBatPath.toStdString() << std::endl << forBatBuild.toStdString();
    outBat.close();
    /*QFile out;
    out.setFileName(fileName);
    out.write((string_include + "\n" +
              string_using + "\n" +
              string_algorithm + "\n" +
              string_mainFunction).toUtf8());
    out.close();*/

    std::ofstream output;
    output.open(fileName.toStdString().c_str());
    output << string_include.toStdString() << std::endl <<
              string_using.toStdString() << std::endl <<
              string_algorithm.toStdString() << std::endl <<
              string_mainFunction.toStdString();
    output.close();


    //process.startDetached("cmd.exe /C" + appPath + "\\build.bat");
    process.execute("cmd.exe /C " + appPath + "\\build.bat");
    process.waitForFinished(-1);
    //Sleep(3000);
    ui->textBrowser->clear();
    std::ifstream input;
    input.open(logFileName.toStdString().c_str());
    if (input.is_open()){
        std::string line;
        while (!input.eof()) {
            std::getline(input, line);
            ui->textBrowser->append(QString::fromStdString(line));
        }
        input.close();
    }

}

void MainWindow::buildAndRun()
{
    build();
    if (ui->textBrowser->toPlainText().length() == 0){
        QProcess process;
        process.startDetached(QApplication::applicationDirPath() + "/" + programName);
    }
}
/*
void MainWindow::changeFunctionName(QString name)
{
    QString sEnding = QString::fromStdString(name.toStdString().substr(name.length() - 4, 4).c_str());
    if (sEnding.contains(".xml"))
        name = QString::fromStdString(name.toStdString().substr(0, name.length()-4).c_str());
    completer->setName(name);
}*/
/*
void MainWindow::translateBack()
{
    qDebug()<<"LOL";
    QString text = ui->textEdit->toPlainText();
    QString line = "";
    QString name = "";

    QVector <QString> vector;
    for(int i = 0; i < text.length(); i++)
    {
        if(text[i] != '\n')
        {
            line+= text[i];
        }
        else
        {
            vector.push_back(line);
            line = "";
        }

    }
    for(int i = 0; i < vector.size(); i++)
    {
        qDebug() << vector[i];
    }
    name = vector[0];
    int n1 = 0, n2 = 0;
    n1 = name.toStdString().find_first_of(" ") + 1;
    n2 = name.toStdString().find_first_of("(");
    n2 -= n1;
    name = QString::fromStdString(name.toStdString().substr(n1, n2));
    QDomDocument doc(name);
    QDomElement domElement;

    domElement = doc.createElement(line);
    domElement = codeToXML(text, doc, domElement, vector);
    doc.appendChild(domElement);

    loadFlowChart(wdg, domElement, true);
    //setWindowTitle(name);

}
*/
void MainWindow::loadFile(const QString &fileName) //Открытие файла
{
    QFile file(fileName);
    /*
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("%1").arg(myName),
                             tr("Не могу открыть файл %1\nПричина: %2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
*/
    //QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QDomDocument doc;
    doc.setContent(&file);
    QDomElement element = doc.documentElement();
    loadFlowChart(wdg, element, true);
    file.close();
   // textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл открыт"), 2000);
}
