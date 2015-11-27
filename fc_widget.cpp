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

FCBeginWidget::FCBeginWidget(QWidget *parent): FCWidget(parent)
{
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    setText("Начало");
}

FCBeginWidget::~FCBeginWidget()
{

}

void FCBeginWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawRoundedRect(QRect(0, 0, 100, 50), 15, 15);
    painter.drawText(QPoint(30, 20), getText());
    painter.end();
}


FCEndWidget::FCEndWidget(QWidget *parent): FCWidget(parent)
{
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
    setText("Конец");
}

FCEndWidget::~FCEndWidget()
{

}

void FCEndWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawRoundedRect(QRect(0, 0, 100, 50), 15, 15);
    painter.drawText(QPoint(30, 20), getText());
    painter.end();
}

FCActionWidget::FCActionWidget(QWidget *parent, QString text): FCWidget(parent)
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
    //connect(this, SIGNAL(modified()), (FCWidget*)parentWidget(), SLOT(ChildModified()));
}

FCActionWidget::~FCActionWidget()
{
    /*
    if (parentWidget()->inherits("FCWidget")) {
        FCWidget *wid = (FCWidget*)parentWidget();
        wid->ChildModified();
    }
    */
}

void FCActionWidget::paintEvent(QPaintEvent *)
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

FCLine::FCLine(QWidget *parent): FCWidget(parent)
{
    ForceArrow = false;
    FCWidget *A = (FCWidget*)parentWidget();
    FCWidget *B = (FCWidget*)A->parentWidget();
    QString name = B->metaObject()->className();
    if (name == "FCPostfixCycleWidget") ForceArrow = true;
    setLine(true);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    isUnderCursor = false;
    //setMouseTracking(true);
}

FCLine::~FCLine()
{

}

bool FCLine::isArrow()
{
    bool arrow = false;
    if (ForceArrow) return true;

    int index = this->parentWidget()->layout()->indexOf(this);
    if (this->parentWidget()->layout()->count()>index+1) {
        FCWidget *wid = (FCWidget*)this->parentWidget()->layout()->itemAt(index+1)->widget();
        arrow = !(wid->isLineBlock());
    }
    return arrow;
}

void FCLine::paintEvent(QPaintEvent *)
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

void FCLine::enterEvent(QEvent *)
{ 
    isUnderCursor = true;
    this->repaint();
}

void FCLine::leaveEvent(QEvent *)
{  
    isUnderCursor = false;
    this->repaint();
}

void FCLine::mousePressEvent(QMouseEvent *)
{

    emit clicked(this);
}

void FCLine::lineclicked(FCLine *object)
{
    QVBoxLayout *layout;
    FCWidget *widget = (FCWidget*)object->parentWidget();
    layout = (QVBoxLayout*)object->parentWidget()->layout();
    int index = layout->indexOf(object) + 1;
    widget->createBlock(index);
    /*
    if (ObjectToFill == action) layout->insertWidget(layout->indexOf(object)+1, new FCActionWidget(widget));
    else if (ObjectToFill == input) layout->insertWidget(layout->indexOf(object)+1, new FCInputWidget(widget));
    else if (ObjectToFill == output) layout->insertWidget(layout->indexOf(object)+1, new FCOutputWidget(widget));
    else if (ObjectToFill == divar) layout->insertWidget(layout->indexOf(object)+1, new FCDivarWidget(widget));
    else if (ObjectToFill == prefix) layout->insertWidget(layout->indexOf(object)+1, new FCPrefixCycleWidget(widget));
    else if (ObjectToFill == postfix) layout->insertWidget(layout->indexOf(object)+1, new FCPostfixCycleWidget(widget));
    else if (ObjectToFill == parameter) layout->insertWidget(layout->indexOf(object)+1, new FCParameterCycleWidget(widget));
    if (ObjectToFill == action || ObjectToFill == input || ObjectToFill == output ||
             ObjectToFill == divar || ObjectToFill == prefix || ObjectToFill == postfix || ObjectToFill == parameter)
    {
        FCLine *line = new FCLine(widget);
        layout->insertWidget(layout->indexOf(object) + 2, line);
        connect(line, SIGNAL(clicked(FCLine*)), line, SLOT(lineclicked(FCLine*)));
    }
    //SetNewSize(widget);
    widget->UpdateSize();
    ToCenter(widget);
    */
}


FCInputWidget::FCInputWidget(QWidget *parent, QString text): FCWidget(parent)
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

FCInputWidget::~FCInputWidget()
{

}

void FCInputWidget::paintEvent(QPaintEvent *)
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


FCOutputWidget::FCOutputWidget(QWidget *parent, QString text): FCWidget(parent)
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

FCOutputWidget::~FCOutputWidget()
{

}

void FCOutputWidget::paintEvent(QPaintEvent *)
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


FCDivarWidget::FCDivarWidget(QWidget *parent, QString text): FCWidget(parent)
{
    setLine(false);
    setToDelete(false);
    setToDialog(false);

    Divar = new FCDecisionWidget(this, text);
    Yes = new FCWidget(this);
    No = new FCWidget(this);
    UPLayout = new QHBoxLayout();
    HLayout = new QHBoxLayout();
    VLayout = new QVBoxLayout();
    lline = new FCLeftLine(this);
    rline = new FCRightLine(this);
    bline = new FCBottomLine(this);
    decision = new FCColumn(this);
    doing = new FCColumn(this);
    LLayout = new QVBoxLayout();
    RLayout = new QVBoxLayout();

    FCLine *line1 = new FCLine(No);
    FCLine *line2 = new FCLine(Yes);
    FCLine *line3 = new FCLine(No);
    FCLine *line4 = new FCLine(Yes);
    FCWidget *centralWidget = new FCWidget(this);
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
    LLayout->insertWidget(1, new FCActionWidget(No));
    LLayout->insertWidget(2, line3);
    RLayout->insertWidget(0, line2);
    RLayout->insertWidget(1, new FCActionWidget(Yes));
    RLayout->insertWidget(2, line4);

    HLayout->itemAt(1)->setAlignment(Qt::AlignCenter);
    HLayout->itemAt(0)->setAlignment(Qt::AlignCenter);

    VLayout->setSpacing(0);
    UPLayout->setSpacing(0);
    HLayout->setSpacing(0);
    LLayout->setSpacing(0);
    RLayout->setSpacing(0);

    connect(line1, SIGNAL(clicked(FCLine*)), line1, SLOT(lineclicked(FCLine*)));
    connect(line2, SIGNAL(clicked(FCLine*)), line2, SLOT(lineclicked(FCLine*)));
    connect(line3, SIGNAL(clicked(FCLine*)), line3, SLOT(lineclicked(FCLine*)));
    connect(line4, SIGNAL(clicked(FCLine*)), line4, SLOT(lineclicked(FCLine*)));
 }

FCDivarWidget::~FCDivarWidget()
{

}

void FCDivarWidget::UpdateSize()
{
    //qDebug()<<doing->height();
    resize(doing->width(), 100+doing->height());
    if (parentWidget()->inherits("FCWidget")) ((FCWidget*)parentWidget())->UpdateSize();

    int margin = 0;
    margin = No->GetMaxWidth()/2;
    FCLeftLine *left = (FCLeftLine*)decision->layout()->itemAt(0)->widget();
    left->margin = margin;
    bline->marginLeft = margin;
    margin = Yes->GetMaxWidth()/2;
    FCRightLine *right = (FCRightLine*)decision->layout()->itemAt(2)->widget();
    right->margin = margin;
    bline->marginRight = margin;
    left->repaint();
    right->repaint();
    bline->repaint();
    //emit(modified());
}

int FCDivarWidget::GetMaxWidth()
{
    return No->GetMaxWidth()+100+Yes->GetMaxWidth();
}

QString FCDivarWidget::getText()
{
    return Divar->getText();
}

FCWidget *FCDivarWidget::getLeftLine()
{
    return No;
}

FCWidget *FCDivarWidget::getRightLine()
{
    return Yes;
}

void FCDivarWidget::paintEvent(QPaintEvent *)
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

void FCDivarWidget::mousePressEvent(QMouseEvent *)
{

}


FCDecisionWidget::FCDecisionWidget(QWidget *parent, QString text, bool isPrefix): FCWidget(parent)
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

FCDecisionWidget::~FCDecisionWidget()
{

}

void FCDecisionWidget::paintEvent(QPaintEvent *)
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

void FCDecisionWidget::block_clicked()
{
    FCWidget* parent = (FCWidget*)parentWidget()->parentWidget();
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
        FCWidget *widget = (FCWidget*)this->parentWidget()->parentWidget()->parentWidget();
        if (isPrefix) {
            widget = (FCWidget*)widget->parentWidget();
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


void FCWidget::mousePressEvent(QMouseEvent *)
{
    emit(clicked());
}

void FCWidget::setText(QString str)
{
    text = str;
}

void FCWidget::setDefaultText(QString str)
{
    defaultText = str;
}

void FCWidget::setLine(bool expression)
{
    isLine = expression;
}

void FCWidget::setToDelete(bool expression)
{
    isCheckedToDelete = expression;
}

void FCWidget::setToDialog(bool expression)
{
    isCheckedToDialog = expression;
}

bool FCWidget::isLineBlock()
{
    return isLine;
}

bool FCWidget::isToDelete()
{
    return isCheckedToDelete;
}

bool FCWidget::isToDialog()
{
    return isCheckedToDialog;
}

FCWidget::FCWidget(QWidget *parent): QWidget(parent)
{
    setLine(false);
    setToDelete(false);
    /*
    if (parentWidget()->inherits("FCWidget"))
        connect(this, SIGNAL(modified()), (FCWidget*)parentWidget(), SLOT(ChildModified()));
        */
}

FCWidget::~FCWidget()
{
    /*
    if (parentWidget()->inherits("FCWidget")) {
        FCWidget *wid = (FCWidget*)parentWidget();
        wid->ChildModified();
    }
    */
}

void FCWidget::UpdateSize()
{
    //qDebug()<<"modified";
    //if (!parentWidget()->inherits("FCWidget")) //emit(modified());
    //qDebug()<<"modified again";
    int w = 0, h = 0;
    QVBoxLayout* layout = (QVBoxLayout*)this->layout();
    int i = 0;
    while (i < layout->count()) {
        FCWidget *wid = (FCWidget*)layout->itemAt(i)->widget();
        i++;
        if (wid->width() > w) w = wid->width();
        h += wid->height();
    }
    if (!parentWidget()->inherits("FCWidget")){
        h+=18;
        w+=18;
    }

    //if (parentWidget()->inherits("FCWidget")) w+=50;
    resize(w, h);
    setFixedHeight(h);
    //qDebug()<<"Block height = "<<height();
    if (parentWidget()->inherits("FCWidget")) ((FCWidget*)parentWidget())->UpdateSize();

}

int FCWidget::GetMaxWidth()
{
    int w = 0;
    QVBoxLayout* layout = (QVBoxLayout*)this->layout();
    if (layout == NULL) return width();
    else {
        if (layout->count() == 0) return width();
        else {
            int i = 0;
            while (i < layout->count()) {
                FCWidget *wid = (FCWidget*)layout->itemAt(i)->widget();
                i++;
                if (wid->GetMaxWidth() > w) w = wid->GetMaxWidth();
            }
            return w;
        }
    }
}

std::string FCWidget::returnTag()
{
    return metaObject()->className();
}

QString FCWidget::getText()
{
    return text;
}

QString FCWidget::getDefaultText()
{
    return defaultText;
}

void FCWidget::createBlock(int index)
{
    FCWidget *block;
    if (ObjectToFill == action) block = new FCActionWidget(this);
    else if (ObjectToFill == input) block = new FCInputWidget(this);
    else if (ObjectToFill == output) block = new FCOutputWidget(this);
    else if (ObjectToFill == divar) block = new FCDivarWidget(this);
    else if (ObjectToFill == prefix) block = new FCPrefixCycleWidget(this);
    else if (ObjectToFill == postfix) block = new FCPostfixCycleWidget(this);
    else if (ObjectToFill == parameter) block = new FCParameterCycleWidget(this);
    addBlock(index, block);
}

void FCWidget::addBlock(int index, FCWidget *widget, bool withLine)
{
    QVBoxLayout *layout = (QVBoxLayout*)this->layout();
    layout->insertWidget(index, widget);
    if (withLine) {
        FCLine *line = new FCLine(this);
        layout->insertWidget(index + 1, line);
        connect(line, SIGNAL(clicked(FCLine*)), line, SLOT(lineclicked(FCLine*)));
    }


    //SetNewSize(widget);
    this->UpdateSize();
    ToCenter(this);
    //emit(modified());
}

/*
void FCWidget::ChildModified()
{
    if (parentWidget()->inherits("FCWidget")) {
        FCWidget *wid = (FCWidget*)parentWidget();
        wid->ChildModified();
    }
    else //emit(modified());
}
*/

void FCWidget::ClickedAction()
{
    ObjectToFill = action;
}

void FCWidget::ClickedInput()
{
    ObjectToFill = input;
}

void FCWidget::ClickedOutput()
{
    ObjectToFill = output;
}

void FCWidget::ClickedDivar()
{
    ObjectToFill = divar;
}

void FCWidget::ClickedPrefix()
{
    ObjectToFill = prefix;
}

void FCWidget::ClickedPostfix()
{
    ObjectToFill = postfix;
}

void FCWidget::ClickedParameter()
{
    ObjectToFill = parameter;
}

void FCWidget::HideTexts(bool toHide)
{
    hideTexts = toHide;
    repaint();
}

void FCWidget::DeleteBlock()
{
    isCheckedToDelete = true;
}

void FCWidget::block_clicked()
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
        FCWidget *widget = (FCWidget*)this->parentWidget();
        int index = widget->layout()->indexOf(this);
        delete widget->layout()->itemAt(index+1)->widget();
        delete this;
        widget->UpdateSize();
        //widget->repaint();
    }
    //ChildModified();
}


FCLeftLine::FCLeftLine(QWidget *parent, bool bottom): FCWidget(parent)
{
    isBottom = bottom;
    setLine(true);
    margin = 50;
    resize(100, 50);
    //setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
}

void FCLeftLine::paintEvent(QPaintEvent *)
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


FCRightLine::FCRightLine(QWidget *parent, bool bottom, bool back, bool arrow): FCWidget(parent)
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

void FCRightLine::paintEvent(QPaintEvent *)
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


FCBottomLine::FCBottomLine(QWidget *parent, bool left): QWidget(parent)
{
    isLeftSide = left;
    marginLeft = 50;
    marginRight = 50;
    resize(300, 50);
    //setFixedWidth(300);
    setFixedHeight(50);
    setContentsMargins(0,0,0,0);
}

FCBottomLine::~FCBottomLine(){}

void FCBottomLine::paintEvent(QPaintEvent *)
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


FCColumn::FCColumn(QWidget *parent): FCWidget(parent)
{

}

void FCColumn::UpdateSize()
{
    int w = 0, h = 0;
    QVBoxLayout* layout = (QVBoxLayout*)this->layout();
    int i = 0;
    while (i < layout->count()) {
        FCWidget *wid = (FCWidget*)layout->itemAt(i)->widget();
        i++;
        if (wid->height() > h) h = wid->height();
        w += wid->width();
    }
    resize(w, h);
    //qDebug()<<"Column height = "<<height();
    if (parentWidget()->inherits("FCWidget")) ((FCWidget*)parentWidget())->UpdateSize();
    //emit(modified());
}

FCPrefixCycleWidget::FCPrefixCycleWidget(QWidget *parent, QString text): FCWidget(parent)
{
    setLine(true);
    setToDelete(false);
    setToDialog(false);

    Cycle = new FCDecisionWidget(this, text, true);
    Yes = new FCWidget(this);
    No = new FCWidget(this);
    UPLayout = new QVBoxLayout();
    UPLayout1 = new QHBoxLayout();
    UPLayout2 = new QHBoxLayout();
    HLayout = new QHBoxLayout();
    VLayout = new QVBoxLayout();
    lline = new FCLeftLine(this);
    rline = new FCRightLine(this, false, true);
    lbline = new FCLeftLine(this, true);
    rbline = new FCRightLine(this, true);
    bline = new FCBottomLine(this, true);
    decision = new FCColumn(this);
    doing = new FCColumn(this);
    Layout = new QVBoxLayout();
    cross = new FCCrossLine(this, true);
    top1 = new FCWidget(this);
    top2 = new FCWidget(this);
    top1left = new FCWidget(this);
    top2right = new FCWidget(this);

    FCLine *line1 = new FCLine(this);
    FCLine *line2 = new FCLine(this);

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
    Layout->insertWidget(1, new FCActionWidget(this));
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

    connect(line1, SIGNAL(clicked(FCLine*)), line1, SLOT(lineclicked(FCLine*)));
    connect(line2, SIGNAL(clicked(FCLine*)), line2, SLOT(lineclicked(FCLine*)));

}

FCPrefixCycleWidget::~FCPrefixCycleWidget()
{

}

void FCPrefixCycleWidget::paintEvent(QPaintEvent *)
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

void FCPrefixCycleWidget::UpdateSize()
{
    //qDebug()<<"cycle update size "<<Yes->GetMaxWidth();
    lbline->resize(100, 50);
    lbline->setFixedWidth(100);

    int w = Yes->GetMaxWidth() + 200;
    int h = doing->height() + 150;
    resize(w, h);

    decision->resize(w, 50);
    decision->setFixedWidth(w);

    if (parentWidget()->inherits("FCWidget")) ((FCWidget*)parentWidget())->UpdateSize();


    lbline->resize(Yes->GetMaxWidth(), 50);
    lbline->setFixedWidth(Yes->GetMaxWidth());
    lbline->margin = lbline->width()/2;
    //emit(modified());
}

int FCPrefixCycleWidget::GetMaxWidth()
{
    return Yes->GetMaxWidth() + 200;
}

QString FCPrefixCycleWidget::getText()
{
    return Cycle->getText();
}

FCWidget *FCPrefixCycleWidget::getCycleBody()
{
    return Yes;
}


FCCrossLine::FCCrossLine(QWidget *parent, bool SetArrow): FCWidget(parent)
{
    isArrow = SetArrow;
    setLine(true);
    resize(100, 50);
    setFixedWidth(100);
    setFixedHeight(50);
    setContentsMargins(0, 0, 0, 0);
}

FCCrossLine::~FCCrossLine()
{

}

void FCCrossLine::paintEvent(QPaintEvent *)
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


FCParameterWidget::FCParameterWidget(QWidget *parent, QString text): FCWidget(parent)
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

FCParameterWidget::~FCParameterWidget()
{

}

void FCParameterWidget::paintEvent(QPaintEvent *)
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

void FCParameterWidget::block_clicked()
{
    FCWidget* parent = (FCWidget*)parentWidget()->parentWidget();
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
        FCWidget *widget = (FCWidget*)this->parentWidget()->parentWidget()->parentWidget();
        int index = widget->layout()->indexOf(this->parentWidget()->parentWidget());
        delete widget->layout()->itemAt(index+1)->widget();
        delete widget->layout()->itemAt(index)->widget();
        widget->UpdateSize();
        //widget->repaint();
    }
}


FCParameterCycleWidget::FCParameterCycleWidget(QWidget *parent, QString text): FCWidget(parent)
{
    setLine(false);
    setToDelete(false);
    setToDialog(false);

    Cycle = new FCParameterWidget(this, text);
    Yes = new FCWidget(this);
    No = new FCWidget(this);
    UPLayout = new QHBoxLayout();
    HLayout = new QHBoxLayout();
    VLayout = new QVBoxLayout();
    lline = new FCLeftLine(this);
    rline = new FCRightLine(this, false, true, true);
    lbline = new FCLeftLine(this, true);
    rbline = new FCRightLine(this, true);
    bline = new FCBottomLine(this, true);
    decision = new FCColumn(this);
    doing = new FCColumn(this);
    Layout = new QVBoxLayout();


    FCLine *line1 = new FCLine(Yes);
    FCLine *line2 = new FCLine(Yes);


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
    Layout->insertWidget(1, new FCActionWidget(Yes));
    Layout->insertWidget(2, line2);
    Layout->insertWidget(3, lbline);

    VLayout->itemAt(0)->setAlignment(Qt::AlignHCenter);
    HLayout->itemAt(2)->setAlignment(Qt::AlignBottom);

    VLayout->setSpacing(0);
    UPLayout->setSpacing(0);
    HLayout->setSpacing(0);
    Layout->setSpacing(0);

    connect(line1, SIGNAL(clicked(FCLine*)), line1, SLOT(lineclicked(FCLine*)));
    connect(line2, SIGNAL(clicked(FCLine*)), line2, SLOT(lineclicked(FCLine*)));

}

FCParameterCycleWidget::~FCParameterCycleWidget()
{

}

void FCParameterCycleWidget::paintEvent(QPaintEvent *)
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

void FCParameterCycleWidget::UpdateSize()
{
    //qDebug()<<"cycle update size "<<Yes->GetMaxWidth();
    lbline->resize(100, 50);
    lbline->setFixedWidth(100);

    int w = Yes->GetMaxWidth() + 200;
    int h = doing->height() + 100;
    resize(w, h);

    decision->resize(w, 50);
    decision->setFixedWidth(w);

    if (parentWidget()->inherits("FCWidget")) ((FCWidget*)parentWidget())->UpdateSize();


    lbline->resize(Yes->GetMaxWidth(), 50);
    lbline->setFixedWidth(Yes->GetMaxWidth());
    lbline->margin = lbline->width()/2;
    //emit(modified());
}

int FCParameterCycleWidget::GetMaxWidth()
{
    return Yes->GetMaxWidth() + 200;
}

QString FCParameterCycleWidget::getText()
{
    return Cycle->getText();
}

FCWidget *FCParameterCycleWidget::getCycleBody()
{
    return Yes;
}


FCPostfixCycleWidget::FCPostfixCycleWidget(QWidget *parent, QString text): FCWidget(parent)
{
    setLine(true);
    setToDelete(false);
    setToDialog(false);

    UPLayout = new QHBoxLayout;
    HLayout = new QHBoxLayout ;
    VLayout = new QVBoxLayout;
    Layout = new QVBoxLayout;
    decision = new FCColumn(this);
    enter = new FCColumn(this);
    cross = new FCCrossLine(this);
    Cycle = new FCDecisionWidget(this, text);
    Center = new FCWidget(this);
    LeftSideUp = new FCWidget(this);
    LeftSideDown = new FCWidget(this);
    rline = new FCRightLine(this, false, true, false);
    rbline = new FCRightLine(this, true);

    FCLine *line1 = new FCLine(Center);
    FCLine *line2 = new FCLine(Center);

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
    Layout->insertWidget(1, new FCActionWidget(Center));
    Layout->insertWidget(2, line2);

    enter->setLayout(UPLayout);
    Center->setLayout(Layout);
    decision->setLayout(HLayout);
    setLayout(VLayout);

    VLayout->itemAt(1)->setAlignment(Qt::AlignHCenter);

    connect(line1, SIGNAL(clicked(FCLine*)), line1, SLOT(lineclicked(FCLine*)));
    connect(line2, SIGNAL(clicked(FCLine*)), line2, SLOT(lineclicked(FCLine*)));
}

FCPostfixCycleWidget::~FCPostfixCycleWidget()
{

}

void FCPostfixCycleWidget::paintEvent(QPaintEvent *)
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

void FCPostfixCycleWidget::UpdateSize()
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
    if (parentWidget()->inherits("FCWidget")) ((FCWidget*)parentWidget())->UpdateSize();
    //emit(modified());
}

int FCPostfixCycleWidget::GetMaxWidth()
{
    return Center->GetMaxWidth() + 200;
}

QString FCPostfixCycleWidget::getText()
{
    return Cycle->getText();
}

FCWidget *FCPostfixCycleWidget::getCycleBody()
{
    return Center;
}
