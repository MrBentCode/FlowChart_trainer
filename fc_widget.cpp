#include "fc_widget.h"
#include <QPainter>
//#include <QDebug>
#include "inputdialog.h"

enum ObjToFill {
    action, input, output, divar, prefix, postfix, parameter
} ObjectToFill;

bool hideTexts = false;

void ToCenter(QWidget *widget)
{
    QLayout *layout = widget->layout();
    int i = layout->count() - 1;
    while (i >= 0) {
        layout->itemAt(i)->setAlignment(Qt::AlignHCenter);
        i--;
    }
}

BlockBegin_Widget::BlockBegin_Widget(QWidget *parent): Block_Widget(parent)
{
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    setText("Начало");
}

BlockBegin_Widget::~BlockBegin_Widget()
{

}

void BlockBegin_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawRoundedRect(QRect(0, 0, 100, 50), 15, 15);
    painter.drawText(QPoint(30, 20), getText());
    painter.end();
}


BlockEnd_Widget::BlockEnd_Widget(QWidget *parent): Block_Widget(parent)
{
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    setText("Конец");
}

BlockEnd_Widget::~BlockEnd_Widget()
{

}

void BlockEnd_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawRoundedRect(QRect(0, 0, 100, 50), 15, 15);
    painter.drawText(QPoint(30, 20), getText());
    painter.end();
}

BlockAction_Widget::BlockAction_Widget(QWidget *parent, QString text): Block_Widget(parent)
{
    setDefaultText("Действие");
    setLine(false);
    setToDelete(false);
    setText(text);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    connect(this, SIGNAL(clicked()), SLOT(block_clicked()));
    //connect(this, SIGNAL(modified()), (Block_Widget*)parentWidget(), SLOT(ChildModified()));
}

BlockAction_Widget::~BlockAction_Widget()
{
    /*
    if (parentWidget()->inherits("Block_Widget")) {
        Block_Widget *wid = (Block_Widget*)parentWidget();
        wid->ChildModified();
    }
    */
}

void BlockAction_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawRect(0, 0, 100, 50);
    int text_length = getText().length();
    int mleft = 20;
    if (text_length > 8) mleft -= 5;
    if (text_length > 12) mleft -= 10;
    if (hideTexts) painter.drawText(QPoint(mleft, 20), getDefaultText());
    else painter.drawText(QPoint(mleft, 20), getText());
    painter.end();
}

Line_Widget::Line_Widget(QWidget *parent): Block_Widget(parent)
{
    ForceArrow = false;
    Block_Widget *A = (Block_Widget*)parentWidget();
    Block_Widget *B = (Block_Widget*)A->parentWidget();
    QString name = B->metaObject()->className();
    if (name == "Postfix_Cycle") ForceArrow = true;
    setLine(true);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    isUnderCursor = false;
    //setMouseTracking(true);
}

Line_Widget::~Line_Widget()
{

}

bool Line_Widget::isArrow()
{
    bool arrow = false;
    if (ForceArrow) return true;

    int index = this->parentWidget()->layout()->indexOf(this);
    if (this->parentWidget()->layout()->count()>index+1) {
        Block_Widget *wid = (Block_Widget*)this->parentWidget()->layout()->itemAt(index+1)->widget();
        arrow = !(wid->isLineBlock());
    }
    return arrow;
}

void Line_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::red);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(50, 0), QPoint(50, 50));
    if (isArrow()) {
        painter.drawLine(QPoint(50, 50), QPoint(45, 45));
        painter.drawLine(QPoint(50, 50), QPoint(55, 45));
    }
    if (isUnderCursor) painter.drawEllipse(QPoint(50, 25), 5, 5);
    painter.end();
}

void Line_Widget::enterEvent(QEvent *)
{ 
    isUnderCursor = true;
    this->repaint();
}

void Line_Widget::leaveEvent(QEvent *)
{  
    isUnderCursor = false;
    this->repaint();
}

void Line_Widget::mousePressEvent(QMouseEvent *)
{

    emit clicked(this);
}

void Line_Widget::lineclicked(Line_Widget *object)
{
    QVBoxLayout *layout;
    Block_Widget *widget = (Block_Widget*)object->parentWidget();
    layout = (QVBoxLayout*)object->parentWidget()->layout();
    int index = layout->indexOf(object) + 1;
    widget->createBlock(index);
    /*
    if (ObjectToFill == action) layout->insertWidget(layout->indexOf(object)+1, new BlockAction_Widget(widget));
    else if (ObjectToFill == input) layout->insertWidget(layout->indexOf(object)+1, new BlockInput_Widget(widget));
    else if (ObjectToFill == output) layout->insertWidget(layout->indexOf(object)+1, new BlockOutput_Widget(widget));
    else if (ObjectToFill == divar) layout->insertWidget(layout->indexOf(object)+1, new Divar_Widget(widget));
    else if (ObjectToFill == prefix) layout->insertWidget(layout->indexOf(object)+1, new Prefix_Cycle(widget));
    else if (ObjectToFill == postfix) layout->insertWidget(layout->indexOf(object)+1, new Postfix_Cycle(widget));
    else if (ObjectToFill == parameter) layout->insertWidget(layout->indexOf(object)+1, new Parameter_Cycle(widget));
    if (ObjectToFill == action || ObjectToFill == input || ObjectToFill == output ||
             ObjectToFill == divar || ObjectToFill == prefix || ObjectToFill == postfix || ObjectToFill == parameter)
    {
        Line_Widget *line = new Line_Widget(widget);
        layout->insertWidget(layout->indexOf(object) + 2, line);
        connect(line, SIGNAL(clicked(Line_Widget*)), line, SLOT(lineclicked(Line_Widget*)));
    }
    //SetNewSize(widget);
    widget->UpdateSize();
    ToCenter(widget);
    */
}


BlockInput_Widget::BlockInput_Widget(QWidget *parent, QString text): Block_Widget(parent)
{
    setDefaultText("Ввод");
    setText(text);
    setLine(false);
    setToDelete(false);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    connect(this, SIGNAL(clicked()), SLOT(block_clicked()));
}

BlockInput_Widget::~BlockInput_Widget()
{

}

void BlockInput_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(this->rect()), Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(0, 50), QPoint(10, 0));
    painter.drawLine(QPoint(90, 50), QPoint(100, 0));
    painter.drawLine(QPoint(10, 0), QPoint(100, 0));
    painter.drawLine(QPoint(0, 50), QPoint(90, 50));
    int text_length = getText().length();
    int mleft = 20;
    if (text_length > 8) mleft -= 5;
    if (text_length > 12) mleft -= 10;
    if (hideTexts) painter.drawText(QPoint(mleft, 20), getDefaultText());
    else painter.drawText(QPoint(mleft, 20), getDefaultText() + " " + getText());
    painter.end();
}


BlockOutput_Widget::BlockOutput_Widget(QWidget *parent, QString text): Block_Widget(parent)
{
    setDefaultText("Вывод");
    setText(text);
    setLine(false);
    setToDelete(false);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    connect(this, SIGNAL(clicked()), SLOT(block_clicked()));
}

BlockOutput_Widget::~BlockOutput_Widget()
{

}

void BlockOutput_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(this->rect()), Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(0, 50), QPoint(10, 0));
    painter.drawLine(QPoint(90, 50), QPoint(100, 0));
    painter.drawLine(QPoint(10, 0), QPoint(100, 0));
    painter.drawLine(QPoint(0, 50), QPoint(90, 50));
    int text_length = getText().length();
    int mleft = 20;
    if (text_length > 8) mleft -= 5;
    if (text_length > 12) mleft -= 10;
    if (hideTexts) painter.drawText(QPoint(mleft, 20), getDefaultText());
    else painter.drawText(QPoint(mleft, 20), getDefaultText() + " " + getText());
    painter.end();
}


Divar_Widget::Divar_Widget(QWidget *parent, QString text): Block_Widget(parent)
{
    setLine(false);
    setToDelete(false);
    setToDialog(false);

    Divar = new Decision_Widget(this, text);
    Yes = new Block_Widget(this);
    No = new Block_Widget(this);
    UPLayout = new QHBoxLayout();
    HLayout = new QHBoxLayout();
    VLayout = new QVBoxLayout();
    lline = new LeftLine(this);
    rline = new RightLine(this);
    bline = new BottomLine(this);
    decision = new MultyColumn(this);
    doing = new MultyColumn(this);
    LLayout = new QVBoxLayout();
    RLayout = new QVBoxLayout();

    Line_Widget *line1 = new Line_Widget(No);
    Line_Widget *line2 = new Line_Widget(Yes);
    Line_Widget *line3 = new Line_Widget(No);
    Line_Widget *line4 = new Line_Widget(Yes);
    Block_Widget *centralWidget = new Block_Widget(this);
    centralWidget->setFixedWidth(100);
    centralWidget->setContentsMargins(0, 0, 0, 0);

    Yes->resize(150, 50);
    No->resize(150, 50);
    decision->resize(300, 50);
    doing->resize(300, 50);
    resize(300, 250);
    No->setFixedHeight(150);
    Yes->setFixedHeight(150);

    VLayout->setContentsMargins(0, 0, 0, 0);
    UPLayout->setContentsMargins(0, 0, 0, 0);
    HLayout->setContentsMargins(0, 0, 0 ,0);
    LLayout->setContentsMargins(0, 0, 0, 0);
    RLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    decision->setLayout(UPLayout);
    doing->setLayout(HLayout);
    No->setLayout(LLayout);
    Yes->setLayout(RLayout);
    setLayout(VLayout);

    UPLayout->addWidget(lline);
    UPLayout->addWidget(Divar);
    UPLayout->addWidget(rline);
    VLayout->addWidget(decision);
    VLayout->addWidget(doing);
    VLayout->addWidget(bline);
    HLayout->addWidget(No);
    HLayout->addWidget(centralWidget);
    HLayout->addWidget(Yes);

    LLayout->insertWidget(0, line1);
    LLayout->insertWidget(1, new BlockAction_Widget(No));
    LLayout->insertWidget(2, line3);
    RLayout->insertWidget(0, line2);
    RLayout->insertWidget(1, new BlockAction_Widget(Yes));
    RLayout->insertWidget(2, line4);

    HLayout->itemAt(1)->setAlignment(Qt::AlignCenter);
    HLayout->itemAt(0)->setAlignment(Qt::AlignCenter);

    VLayout->setSpacing(0);
    UPLayout->setSpacing(0);
    HLayout->setSpacing(0);
    LLayout->setSpacing(0);
    RLayout->setSpacing(0);

    connect(line1, SIGNAL(clicked(Line_Widget*)), line1, SLOT(lineclicked(Line_Widget*)));
    connect(line2, SIGNAL(clicked(Line_Widget*)), line2, SLOT(lineclicked(Line_Widget*)));
    connect(line3, SIGNAL(clicked(Line_Widget*)), line3, SLOT(lineclicked(Line_Widget*)));
    connect(line4, SIGNAL(clicked(Line_Widget*)), line4, SLOT(lineclicked(Line_Widget*)));
 }

Divar_Widget::~Divar_Widget()
{

}

void Divar_Widget::UpdateSize()
{
    //qDebug()<<doing->height();
    resize(doing->width(), 100+doing->height());
    if (parentWidget()->inherits("Block_Widget")) ((Block_Widget*)parentWidget())->UpdateSize();

    int margin = 0;
    margin = No->GetMaxWidth()/2;
    LeftLine *left = (LeftLine*)decision->layout()->itemAt(0)->widget();
    left->margin = margin;
    bline->marginLeft = margin;
    margin = Yes->GetMaxWidth()/2;
    RightLine *right = (RightLine*)decision->layout()->itemAt(2)->widget();
    right->margin = margin;
    bline->marginRight = margin;
    left->repaint();
    right->repaint();
    bline->repaint();
    //emit(modified());
}

int Divar_Widget::GetMaxWidth()
{
    return No->GetMaxWidth()+100+Yes->GetMaxWidth();
}

QString Divar_Widget::getText()
{
    return Divar->getText();
}

Block_Widget *Divar_Widget::getLeftLine()
{
    return No;
}

Block_Widget *Divar_Widget::getRightLine()
{
    return Yes;
}

void Divar_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(this->rect()), Qt::white);
    if (isToDialog()) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(QRect(this->rect()));
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 3));
    int mleft = No->GetMaxWidth()/2;
    int mright = Yes->GetMaxWidth()/2;
    painter.drawLine(QPoint(mleft, 50), QPoint(mleft, height()-50));
    painter.drawLine(QPoint(width()-mright, 50), QPoint(width()-mright, height()-50));
    painter.end();
}

void Divar_Widget::mousePressEvent(QMouseEvent *)
{

}


Decision_Widget::Decision_Widget(QWidget *parent, QString text, bool isPrefix): Block_Widget(parent)
{
    this->isPrefix = isPrefix;
    setDefaultText("Решение");
    setText(text);
    setLine(false);
    setToDelete(false);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    connect(this, SIGNAL(clicked()), SLOT(block_clicked()));
}

Decision_Widget::~Decision_Widget()
{

}

void Decision_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(0, 25), QPoint(50, 0));
    painter.drawLine(QPoint(50, 0), QPoint(100, 25));
    painter.drawLine(QPoint(0, 25), QPoint(50, 50));
    painter.drawLine(QPoint(50, 50), QPoint(100, 25));
    int text_length = getText().length();
    int mleft = 20;
    if (text_length > 8) mleft -= 5;
    if (text_length > 12) mleft -= 5;
    if (hideTexts) painter.drawText(QPoint(mleft, 25), getDefaultText());
    else painter.drawText(QPoint(mleft, 25), getText());
    painter.end();
}

void Decision_Widget::block_clicked()
{
    Block_Widget* parent = (Block_Widget*)parentWidget()->parentWidget();
    parent->setToDialog(true);
    parent->repaint();
    InputDialog* pInputDialog = new InputDialog(getText());
    connect(pInputDialog->pcmdDelete, SIGNAL(clicked()), SLOT(DeleteBlock()));
    pInputDialog->show();
    if (pInputDialog->exec() == QDialog::Accepted) {
        setText(pInputDialog->getLine());
    }
    delete pInputDialog;
    repaint();
    parent->setToDialog(false);
    parent->repaint();

    if (isToDelete()) {
        int index = 0;
        Block_Widget *widget = (Block_Widget*)this->parentWidget()->parentWidget()->parentWidget();
        if (isPrefix) {
            widget = (Block_Widget*)widget->parentWidget();
            index = widget->layout()->indexOf(this->parentWidget()->parentWidget()->parentWidget());
        }
        else index = widget->layout()->indexOf(this->parentWidget()->parentWidget());

        delete widget->layout()->itemAt(index+1)->widget();
        delete widget->layout()->itemAt(index)->widget();
        widget->UpdateSize();
        //widget->repaint();
    }
    //emit(modified());

}


void Block_Widget::mousePressEvent(QMouseEvent *)
{
    emit(clicked());
}

void Block_Widget::setText(QString str)
{
    text = str;
}

void Block_Widget::setDefaultText(QString str)
{
    defaultText = str;
}

void Block_Widget::setLine(bool expression)
{
    isLine = expression;
}

void Block_Widget::setToDelete(bool expression)
{
    isCheckedToDelete = expression;
}

void Block_Widget::setToDialog(bool expression)
{
    isCheckedToDialog = expression;
}

bool Block_Widget::isLineBlock()
{
    return isLine;
}

bool Block_Widget::isToDelete()
{
    return isCheckedToDelete;
}

bool Block_Widget::isToDialog()
{
    return isCheckedToDialog;
}

Block_Widget::Block_Widget(QWidget *parent): QWidget(parent)
{
    setLine(false);
    setToDelete(false);
    /*
    if (parentWidget()->inherits("Block_Widget"))
        connect(this, SIGNAL(modified()), (Block_Widget*)parentWidget(), SLOT(ChildModified()));
        */
}

Block_Widget::~Block_Widget()
{
    /*
    if (parentWidget()->inherits("Block_Widget")) {
        Block_Widget *wid = (Block_Widget*)parentWidget();
        wid->ChildModified();
    }
    */
}

void Block_Widget::UpdateSize()
{
    //qDebug()<<"modified";
    //if (!parentWidget()->inherits("Block_Widget")) //emit(modified());
    //qDebug()<<"modified again";
    int w = 0, h = 0;
    QVBoxLayout* layout = (QVBoxLayout*)this->layout();
    int i = 0;
    while (i < layout->count()) {
        Block_Widget *wid = (Block_Widget*)layout->itemAt(i)->widget();
        i++;
        if (wid->width() > w) w = wid->width();
        h += wid->height();
    }
    if (!parentWidget()->inherits("Block_Widget")){
        h+=18;
        w+=18;
    }

    //if (parentWidget()->inherits("Block_Widget")) w+=50;
    resize(w, h);
    setFixedHeight(h);
    //qDebug()<<"Block height = "<<height();
    if (parentWidget()->inherits("Block_Widget")) ((Block_Widget*)parentWidget())->UpdateSize();

}

int Block_Widget::GetMaxWidth()
{
    int w = 0;
    QVBoxLayout* layout = (QVBoxLayout*)this->layout();
    if (layout == NULL) return width();
    else {
        if (layout->count() == 0) return width();
        else {
            int i = 0;
            while (i < layout->count()) {
                Block_Widget *wid = (Block_Widget*)layout->itemAt(i)->widget();
                i++;
                if (wid->GetMaxWidth() > w) w = wid->GetMaxWidth();
            }
            return w;
        }
    }
}

std::string Block_Widget::returnTag()
{
    return metaObject()->className();
}

QString Block_Widget::getText()
{
    return text;
}

QString Block_Widget::getDefaultText()
{
    return defaultText;
}

void Block_Widget::createBlock(int index)
{
    Block_Widget *block;
    if (ObjectToFill == action) block = new BlockAction_Widget(this);
    else if (ObjectToFill == input) block = new BlockInput_Widget(this);
    else if (ObjectToFill == output) block = new BlockOutput_Widget(this);
    else if (ObjectToFill == divar) block = new Divar_Widget(this);
    else if (ObjectToFill == prefix) block = new Prefix_Cycle(this);
    else if (ObjectToFill == postfix) block = new Postfix_Cycle(this);
    else if (ObjectToFill == parameter) block = new Parameter_Cycle(this);
    addBlock(index, block);
}

void Block_Widget::addBlock(int index, Block_Widget *widget, bool withLine)
{
    QVBoxLayout *layout = (QVBoxLayout*)this->layout();
    layout->insertWidget(index, widget);
    if (withLine) {
        Line_Widget *line = new Line_Widget(this);
        layout->insertWidget(index + 1, line);
        connect(line, SIGNAL(clicked(Line_Widget*)), line, SLOT(lineclicked(Line_Widget*)));
    }


    //SetNewSize(widget);
    this->UpdateSize();
    ToCenter(this);
    //emit(modified());
}

/*
void Block_Widget::ChildModified()
{
    if (parentWidget()->inherits("Block_Widget")) {
        Block_Widget *wid = (Block_Widget*)parentWidget();
        wid->ChildModified();
    }
    else //emit(modified());
}
*/

void Block_Widget::ClickedAction()
{
    ObjectToFill = action;
}

void Block_Widget::ClickedInput()
{
    ObjectToFill = input;
}

void Block_Widget::ClickedOutput()
{
    ObjectToFill = output;
}

void Block_Widget::ClickedDivar()
{
    ObjectToFill = divar;
}

void Block_Widget::ClickedPrefix()
{
    ObjectToFill = prefix;
}

void Block_Widget::ClickedPostfix()
{
    ObjectToFill = postfix;
}

void Block_Widget::ClickedParameter()
{
    ObjectToFill = parameter;
}

void Block_Widget::HideTexts(bool toHide)
{
    hideTexts = toHide;
    repaint();
}

void Block_Widget::DeleteBlock()
{
    isCheckedToDelete = true;
}

void Block_Widget::block_clicked()
{
    InputDialog* pInputDialog = new InputDialog(text);
    connect(pInputDialog->pcmdDelete, SIGNAL(clicked()), SLOT(DeleteBlock()));
    pInputDialog->show();
    if (pInputDialog->exec() == QDialog::Accepted) {
        text = pInputDialog->getLine();
    }
    delete pInputDialog;
    repaint();
    //qDebug()<<parentWidget()->metaObject()->className();
    if (isCheckedToDelete) {
        Block_Widget *widget = (Block_Widget*)this->parentWidget();
        int index = widget->layout()->indexOf(this);
        delete widget->layout()->itemAt(index+1)->widget();
        delete this;
        widget->UpdateSize();
        //widget->repaint();
    }
    //ChildModified();
}


LeftLine::LeftLine(QWidget *parent, bool bottom): Block_Widget(parent)
{
    isBottom = bottom;
    setLine(true);
    margin = 50;
    resize(100, 50);
    //setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
}

void LeftLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(margin, 25), QPoint(width(), 25));
    if (isBottom) {
        painter.drawLine(QPoint(margin, 0), QPoint(margin, 25));
    }
    else {
        painter.drawLine(QPoint(margin, 25), QPoint(margin, 50));
        painter.drawText(QPoint(width()-20, 20), "No");
    }
    painter.end();
}


RightLine::RightLine(QWidget *parent, bool bottom, bool back, bool arrow): Block_Widget(parent)
{
    isBottom = bottom;
    isBack = back;
    isArrow = arrow;
    margin = 50;
    resize(100, 50);
    //setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
}

void RightLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(0, 25), QPoint(width()-margin, 25));
    if (isBottom) {
        painter.drawLine(QPoint(width()-margin, 0), QPoint(width()-margin, 25));

    }
    else {
        painter.drawLine(QPoint(width()-margin, 25), QPoint(width()-margin, 50));
        if (isBack && isArrow) {
            painter.drawLine(QPoint(0, 25), QPoint(5, 20));
            painter.drawLine(QPoint(0, 25), QPoint(5, 30));
        }
        else if (!isBack) painter.drawText(QPoint(20, 20), "Yes");

    }
    painter.end();
}


BottomLine::BottomLine(QWidget *parent, bool left): QWidget(parent)
{
    isLeftSide = left;
    marginLeft = 50;
    marginRight = 50;
    resize(300, 50);
    //setFixedWidth(300);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
}

BottomLine::~BottomLine(){}

void BottomLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::white);
    painter.drawLine(QPoint(marginLeft, 0), QPoint(marginLeft, 25));
    if (!isLeftSide) {
        painter.drawLine(QPoint(width()-marginRight, 0), QPoint(width()-marginRight, 25));
        painter.drawLine(QPoint(marginLeft, 25), QPoint(width()-marginRight, 25));
    }
    else painter.drawLine(QPoint(marginLeft, 25), QPoint(width()/2, 25));
    painter.drawLine(QPoint(width()/2, 25), QPoint(width()/2, 50));
    painter.end();
}


MultyColumn::MultyColumn(QWidget *parent): Block_Widget(parent)
{

}

void MultyColumn::UpdateSize()
{
    int w = 0, h = 0;
    QVBoxLayout* layout = (QVBoxLayout*)this->layout();
    int i = 0;
    while (i < layout->count()) {
        Block_Widget *wid = (Block_Widget*)layout->itemAt(i)->widget();
        i++;
        if (wid->height() > h) h = wid->height();
        w += wid->width();
    }
    resize(w, h);
    //qDebug()<<"Column height = "<<height();
    if (parentWidget()->inherits("Block_Widget")) ((Block_Widget*)parentWidget())->UpdateSize();
    //emit(modified());
}

Prefix_Cycle::Prefix_Cycle(QWidget *parent, QString text): Block_Widget(parent)
{
    setLine(true);
    setToDelete(false);
    setToDialog(false);

    Cycle = new Decision_Widget(this, text, true);
    Yes = new Block_Widget(this);
    No = new Block_Widget(this);
    UPLayout = new QVBoxLayout();
    UPLayout1 = new QHBoxLayout();
    UPLayout2 = new QHBoxLayout();
    HLayout = new QHBoxLayout();
    VLayout = new QVBoxLayout();
    lline = new LeftLine(this);
    rline = new RightLine(this, false, true);
    lbline = new LeftLine(this, true);
    rbline = new RightLine(this, true);
    bline = new BottomLine(this, true);
    decision = new MultyColumn(this);
    doing = new MultyColumn(this);
    Layout = new QVBoxLayout();
    cross = new CrossLine_Widget(this, true);
    top1 = new Block_Widget(this);
    top2 = new Block_Widget(this);
    top1left = new Block_Widget(this);
    top2right = new Block_Widget(this);

    Line_Widget *line1 = new Line_Widget(this);
    Line_Widget *line2 = new Line_Widget(this);

    top1->resize(300, 50);
    top2->resize(300, 50);
    top1left->resize(100, 50);
    top2right->resize(100, 50);
    top1left->setFixedHeight(50);
    top2right->setFixedHeight(50);
    Yes->resize(100, 200);
    No->resize(100, 50);
    No->setFixedSize(100, 50);
    rbline->resize(100, 50);
    rbline->setFixedSize(100, 50);
    lbline->setFixedHeight(50);
    decision->resize(300, 100);
    decision->setFixedWidth(300);
    doing->resize(300, 50);
    resize(300, 350);

    VLayout->setContentsMargins(0, 0, 0, 0);
    UPLayout1->setContentsMargins(0, 0, 0, 0);
    UPLayout2->setContentsMargins(0, 0, 0, 0);
    UPLayout->setContentsMargins(0, 0, 0, 0);
    HLayout->setContentsMargins(0, 0, 0 ,0);
    Layout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    decision->setLayout(UPLayout);
    doing->setLayout(HLayout);
    Yes->setLayout(Layout);
    top1->setLayout(UPLayout1);
    top2->setLayout(UPLayout2);
    setLayout(VLayout);

    UPLayout1->addWidget(top1left);
    UPLayout1->addWidget(cross);
    UPLayout1->addWidget(rline);
    UPLayout2->addWidget(lline);
    UPLayout2->addWidget(Cycle);
    UPLayout2->addWidget(top2right);
    UPLayout->addWidget(top1);
    UPLayout->addWidget(top2);
    HLayout->addWidget(No);
    HLayout->addWidget(Yes);
    HLayout->addWidget(rbline);
    VLayout->addWidget(decision);
    VLayout->addWidget(doing);
    VLayout->addWidget(bline);
    Layout->insertWidget(0, line1);
    Layout->insertWidget(1, new BlockAction_Widget(this));
    Layout->insertWidget(2, line2);
    Layout->insertWidget(3, lbline);

    VLayout->itemAt(0)->setAlignment(Qt::AlignHCenter);
    HLayout->itemAt(2)->setAlignment(Qt::AlignBottom);

    VLayout->setSpacing(0);
    UPLayout->setSpacing(0);
    UPLayout1->setSpacing(0);
    UPLayout2->setSpacing(0);
    HLayout->setSpacing(0);
    Layout->setSpacing(0);

    connect(line1, SIGNAL(clicked(Line_Widget*)), line1, SLOT(lineclicked(Line_Widget*)));
    connect(line2, SIGNAL(clicked(Line_Widget*)), line2, SLOT(lineclicked(Line_Widget*)));

}

Prefix_Cycle::~Prefix_Cycle()
{

}

void Prefix_Cycle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(this->rect()), Qt::white);
    if (isToDialog()) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(QRect(this->rect()));
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(50, 100), QPoint(50, height()-50));
    painter.drawLine(QPoint(width()-50, 50), QPoint(width()-50, height()-100));
    painter.end();
}

void Prefix_Cycle::UpdateSize()
{
    //qDebug()<<"cycle update size "<<Yes->GetMaxWidth();
    lbline->resize(100, 50);
    lbline->setFixedWidth(100);

    int w = Yes->GetMaxWidth() + 200;
    int h = doing->height() + 150;
    resize(w, h);

    decision->resize(w, 50);
    decision->setFixedWidth(w);

    if (parentWidget()->inherits("Block_Widget")) ((Block_Widget*)parentWidget())->UpdateSize();


    lbline->resize(Yes->GetMaxWidth(), 50);
    lbline->setFixedWidth(Yes->GetMaxWidth());
    lbline->margin = lbline->width()/2;
    //emit(modified());
}

int Prefix_Cycle::GetMaxWidth()
{
    return Yes->GetMaxWidth() + 200;
}

QString Prefix_Cycle::getText()
{
    return Cycle->getText();
}

Block_Widget *Prefix_Cycle::getCycleBody()
{
    return Yes;
}


CrossLine_Widget::CrossLine_Widget(QWidget *parent, bool SetArrow): Block_Widget(parent)
{
    isArrow = SetArrow;
    setLine(true);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0, 0, 0, 0);
}

CrossLine_Widget::~CrossLine_Widget()
{

}

void CrossLine_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::red);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(50, 0), QPoint(50, 50));
    painter.drawLine(QPoint(50, 25), QPoint(100, 25));
    painter.drawLine(QPoint(50, 25), QPoint(55, 20));
    painter.drawLine(QPoint(50, 25), QPoint(55, 30));
    if (isArrow) {
        painter.drawLine(QPoint(50, 50), QPoint(45, 45));
        painter.drawLine(QPoint(50, 50), QPoint(55, 45));
    }
    painter.end();
}


Parameter_Widget::Parameter_Widget(QWidget *parent, QString text): Block_Widget(parent)
{
    setDefaultText("Параметр");
    setText(text);
    setLine(false);
    setToDelete(false);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    connect(this, SIGNAL(clicked()), SLOT(block_clicked()));
}

Parameter_Widget::~Parameter_Widget()
{

}

void Parameter_Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(0, 25), QPoint(25, 0));
    painter.drawLine(QPoint(0, 25), QPoint(25, 50));
    painter.drawLine(QPoint(75, 0), QPoint(100, 25));
    painter.drawLine(QPoint(75, 50), QPoint(100, 25));
    painter.drawLine(QPoint(25, 0), QPoint(75, 0));
    painter.drawLine(QPoint(25, 50), QPoint(75, 50));
    int text_length = getText().length();
    int mleft = 20;
    if (text_length > 8) mleft -= 5;
    if (text_length > 12) mleft -= 5;
    if (hideTexts) painter.drawText(QPoint(mleft, 25), getDefaultText());
    else painter.drawText(QPoint(mleft, 25), getText());
    painter.end();
}

void Parameter_Widget::block_clicked()
{
    Block_Widget* parent = (Block_Widget*)parentWidget()->parentWidget();
    parent->setToDialog(true);
    parent->repaint();
    InputDialog* pInputDialog = new InputDialog(getText());
    connect(pInputDialog->pcmdDelete, SIGNAL(clicked()), SLOT(DeleteBlock()));
    pInputDialog->show();
    if (pInputDialog->exec() == QDialog::Accepted) {
        setText(pInputDialog->getLine());
    }
    delete pInputDialog;
    repaint();
    //qDebug()<<parentWidget()->metaObject()->className();
    parent->setToDialog(false);
    parent->repaint();

    if (isToDelete()) {
        Block_Widget *widget = (Block_Widget*)this->parentWidget()->parentWidget()->parentWidget();
        int index = widget->layout()->indexOf(this->parentWidget()->parentWidget());
        delete widget->layout()->itemAt(index+1)->widget();
        delete widget->layout()->itemAt(index)->widget();
        widget->UpdateSize();
        //widget->repaint();
    }
}


Parameter_Cycle::Parameter_Cycle(QWidget *parent, QString text): Block_Widget(parent)
{
    setLine(false);
    setToDelete(false);
    setToDialog(false);

    Cycle = new Parameter_Widget(this, text);
    Yes = new Block_Widget(this);
    No = new Block_Widget(this);
    UPLayout = new QHBoxLayout();
    HLayout = new QHBoxLayout();
    VLayout = new QVBoxLayout();
    lline = new LeftLine(this);
    rline = new RightLine(this, false, true, true);
    lbline = new LeftLine(this, true);
    rbline = new RightLine(this, true);
    bline = new BottomLine(this, true);
    decision = new MultyColumn(this);
    doing = new MultyColumn(this);
    Layout = new QVBoxLayout();


    Line_Widget *line1 = new Line_Widget(Yes);
    Line_Widget *line2 = new Line_Widget(Yes);


    Yes->resize(100, 200);
    No->resize(100, 50);
    No->setFixedSize(100, 50);
    rbline->resize(100, 50);
    rbline->setFixedSize(100, 50);
    lbline->setFixedHeight(50);
    decision->resize(300, 50);
    decision->setFixedWidth(300);
    doing->resize(300, 50);
    resize(300, 300);

    VLayout->setContentsMargins(0, 0, 0, 0);
    UPLayout->setContentsMargins(0, 0, 0, 0);
    HLayout->setContentsMargins(0, 0, 0 ,0);
    Layout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    decision->setLayout(UPLayout);
    doing->setLayout(HLayout);
    Yes->setLayout(Layout);
    setLayout(VLayout);

    UPLayout->addWidget(lline);
    UPLayout->addWidget(Cycle);
    UPLayout->addWidget(rline);
    HLayout->addWidget(No);
    HLayout->addWidget(Yes);
    HLayout->addWidget(rbline);
    VLayout->addWidget(decision);
    VLayout->addWidget(doing);
    VLayout->addWidget(bline);
    Layout->insertWidget(0, line1);
    Layout->insertWidget(1, new BlockAction_Widget(Yes));
    Layout->insertWidget(2, line2);
    Layout->insertWidget(3, lbline);

    VLayout->itemAt(0)->setAlignment(Qt::AlignHCenter);
    HLayout->itemAt(2)->setAlignment(Qt::AlignBottom);

    VLayout->setSpacing(0);
    UPLayout->setSpacing(0);
    HLayout->setSpacing(0);
    Layout->setSpacing(0);

    connect(line1, SIGNAL(clicked(Line_Widget*)), line1, SLOT(lineclicked(Line_Widget*)));
    connect(line2, SIGNAL(clicked(Line_Widget*)), line2, SLOT(lineclicked(Line_Widget*)));

}

Parameter_Cycle::~Parameter_Cycle()
{

}

void Parameter_Cycle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(this->rect()), Qt::white);
    if (isToDialog()) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(QRect(this->rect()));
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(50, 50), QPoint(50, height()-50));
    painter.drawLine(QPoint(width()-50, 50), QPoint(width()-50, height()-100));
    painter.end();
}

void Parameter_Cycle::UpdateSize()
{
    //qDebug()<<"cycle update size "<<Yes->GetMaxWidth();
    lbline->resize(100, 50);
    lbline->setFixedWidth(100);

    int w = Yes->GetMaxWidth() + 200;
    int h = doing->height() + 100;
    resize(w, h);

    decision->resize(w, 50);
    decision->setFixedWidth(w);

    if (parentWidget()->inherits("Block_Widget")) ((Block_Widget*)parentWidget())->UpdateSize();


    lbline->resize(Yes->GetMaxWidth(), 50);
    lbline->setFixedWidth(Yes->GetMaxWidth());
    lbline->margin = lbline->width()/2;
    //emit(modified());
}

int Parameter_Cycle::GetMaxWidth()
{
    return Yes->GetMaxWidth() + 200;
}

QString Parameter_Cycle::getText()
{
    return Cycle->getText();
}

Block_Widget *Parameter_Cycle::getCycleBody()
{
    return Yes;
}


Postfix_Cycle::Postfix_Cycle(QWidget *parent, QString text): Block_Widget(parent)
{
    setLine(true);
    setToDelete(false);
    setToDialog(false);

    UPLayout = new QHBoxLayout;
    HLayout = new QHBoxLayout ;
    VLayout = new QVBoxLayout;
    Layout = new QVBoxLayout;
    decision = new MultyColumn(this);
    enter = new MultyColumn(this);
    cross = new CrossLine_Widget(this);
    Cycle = new Decision_Widget(this, text);
    Center = new Block_Widget(this);
    LeftSideUp = new Block_Widget(this);
    LeftSideDown = new Block_Widget(this);
    rline = new RightLine(this, false, true, false);
    rbline = new RightLine(this, true);

    Line_Widget *line1 = new Line_Widget(Center);
    Line_Widget *line2 = new Line_Widget(Center);

    enter->resize(300, 50);
    enter->setFixedHeight(50);
    decision->resize(300, 50);
    decision->setFixedHeight(50);
    LeftSideDown->setFixedSize(100, 50);
    LeftSideUp->setFixedSize(100, 50);
    rline->setFixedSize(100, 50);
    rbline->setFixedSize(100, 50);
    resize(300, 250);

    UPLayout->setContentsMargins(0, 0, 0, 0);
    HLayout->setContentsMargins(0, 0, 0, 0);
    VLayout->setContentsMargins(0, 0, 0, 0);
    Layout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    UPLayout->setSpacing(0);
    HLayout->setSpacing(0);
    VLayout->setSpacing(0);
    Layout->setSpacing(0);

    UPLayout->addWidget(LeftSideUp);
    UPLayout->addWidget(cross);
    UPLayout->addWidget(rline);

    HLayout->addWidget(LeftSideDown);
    HLayout->addWidget(Cycle);
    HLayout->addWidget(rbline);

    VLayout->addWidget(enter);
    VLayout->addWidget(Center);
    VLayout->addWidget(decision);

    Layout->insertWidget(0, line1);
    Layout->insertWidget(1, new BlockAction_Widget(Center));
    Layout->insertWidget(2, line2);

    enter->setLayout(UPLayout);
    Center->setLayout(Layout);
    decision->setLayout(HLayout);
    setLayout(VLayout);

    VLayout->itemAt(1)->setAlignment(Qt::AlignHCenter);

    connect(line1, SIGNAL(clicked(Line_Widget*)), line1, SLOT(lineclicked(Line_Widget*)));
    connect(line2, SIGNAL(clicked(Line_Widget*)), line2, SLOT(lineclicked(Line_Widget*)));
}

Postfix_Cycle::~Postfix_Cycle()
{

}

void Postfix_Cycle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(this->rect()), Qt::white);
    if (isToDialog()) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(QRect(this->rect()));
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(QPoint(width()-50, 50), QPoint(width()-50, height()-50));
    painter.end();
}

void Postfix_Cycle::UpdateSize()
{
    int maxw = Center->GetMaxWidth();
    int w = maxw + 200;
    int h = Center->height()+100;
    LeftSideUp->setFixedWidth((w - 100)/2);
    LeftSideDown->setFixedWidth((w - 100)/2);
    rline->setFixedWidth((w - 100)/2);
    rbline->setFixedWidth((w - 100)/2);
    enter->resize(w, 50);
    decision->resize(w, 50);
    resize(w, h);
    if (parentWidget()->inherits("Block_Widget")) ((Block_Widget*)parentWidget())->UpdateSize();
    //emit(modified());
}

int Postfix_Cycle::GetMaxWidth()
{
    return Center->GetMaxWidth() + 200;
}

QString Postfix_Cycle::getText()
{
    return Cycle->getText();
}

Block_Widget *Postfix_Cycle::getCycleBody()
{
    return Center;
}
