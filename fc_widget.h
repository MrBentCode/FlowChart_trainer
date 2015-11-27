#ifndef FC_WIDGET_H
#define FC_WIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>


//void SetNewSize(QWidget *widget);

class FCWidget: public QWidget
{
    Q_OBJECT
private:
    virtual void mousePressEvent(QMouseEvent *);
    QString defaultText;
    QString text;
    bool isLine;
    bool isCheckedToDelete;
    bool isCheckedToDialog;
public:
    void setText(QString str);
    void setDefaultText(QString str);
    void setLine(bool expression);
    void setToDelete(bool expression);
    void setToDialog(bool expression);
    bool isLineBlock();
    bool isToDelete();
    bool isToDialog();
    FCWidget(QWidget *parent = 0);
    virtual ~FCWidget();
    virtual void UpdateSize();
    virtual int GetMaxWidth();
    virtual std::string returnTag();
    virtual QString getText();
    virtual QString getDefaultText();
    void createBlock(int index);
    void addBlock(int index, FCWidget *widget, bool withLine = true);
signals:
    void clicked();
    //void modified();
public slots:
    //void ChildModified();
    void ClickedAction();
    void ClickedInput();
    void ClickedOutput();
    void ClickedDivar();
    void ClickedPrefix();
    void ClickedPostfix();
    void ClickedParameter();
    void HideTexts(bool toHide);
    virtual void DeleteBlock();
    virtual void block_clicked();
};

class FCColumn: public FCWidget
{
public:
    FCColumn(QWidget *parent = 0);
    ~FCColumn(){}
    void UpdateSize();
};

class FCLeftLine: public FCWidget
{
    Q_OBJECT
public:
    int margin;
    FCLeftLine(QWidget *parent = 0, bool bottom = false);
    ~FCLeftLine(){}
private:
    bool isBottom;
    void paintEvent(QPaintEvent *);
};

class FCRightLine: public FCWidget
{
    Q_OBJECT
public:
    int margin;
    FCRightLine(QWidget *parent = 0, bool bottom = false, bool back = false, bool arrow = false);
    ~FCRightLine(){}
private:
    bool isBottom;
    bool isBack;
    bool isArrow;
    void paintEvent(QPaintEvent *);
};

class FCBottomLine: public QWidget
{
    Q_OBJECT
public:
    int marginLeft;
    int marginRight;
    FCBottomLine(QWidget *parent = 0, bool left = false);
    ~FCBottomLine();
private:
    bool isLeftSide;
    void paintEvent(QPaintEvent *);
};

class FCLine: public FCWidget
{
    Q_OBJECT
public:
    FCLine(QWidget *parent = 0);
    ~FCLine();
    bool isArrow();
private:
    bool ForceArrow;
    bool isUnderCursor;
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);

signals:
    void clicked(FCLine* object);
public slots:
    void lineclicked(FCLine* object);
};

class FCCrossLine: public FCWidget
{
    Q_OBJECT
public:
    FCCrossLine(QWidget *parent = 0, bool SetArrow = false);
    ~FCCrossLine();
private:
    bool isArrow;
    void paintEvent(QPaintEvent *);
};

class FCBeginWidget: public FCWidget
{
    Q_OBJECT
public:
    FCBeginWidget(QWidget *parent = 0);
    ~FCBeginWidget();
private:
    void paintEvent(QPaintEvent *);
};

class FCEndWidget: public FCWidget
{
    Q_OBJECT
public:
    FCEndWidget(QWidget *parent = 0);
    ~FCEndWidget();
private:
    void paintEvent(QPaintEvent *);
};

class FCActionWidget: public FCWidget
{
    Q_OBJECT
public:
    FCActionWidget(QWidget *parent = 0, QString text = "Действие");
    ~FCActionWidget();
private:
    void paintEvent(QPaintEvent *);
};

class FCInputWidget: public FCWidget
{
    Q_OBJECT
public:
    FCInputWidget(QWidget *parent = 0, QString text = "Ввод");
    ~FCInputWidget();
private:
    void paintEvent(QPaintEvent *);
};

class FCOutputWidget: public FCWidget
{
    Q_OBJECT
public:
    FCOutputWidget(QWidget *parent = 0, QString text = "Вывод");
    ~FCOutputWidget();
private:
    void paintEvent(QPaintEvent *);
};

class FCDecisionWidget: public FCWidget
{
    Q_OBJECT
public:
    FCDecisionWidget(QWidget *parent = 0, QString text = "Решение", bool isPrefix = false);
    ~FCDecisionWidget();
private:
    void paintEvent(QPaintEvent *);
    bool isPrefix;
public slots:
    void block_clicked();
};

class FCParameterWidget: public FCWidget
{
    Q_OBJECT
public:
    FCParameterWidget(QWidget *parent = 0, QString text = "Параметр");
    ~FCParameterWidget();
private:
    void paintEvent(QPaintEvent *);
public slots:
    void block_clicked();
};

class FCDivarWidget: public FCWidget
{
    Q_OBJECT
public:
    FCDivarWidget(QWidget *parent = 0, QString text = "Решение");
    ~FCDivarWidget();
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    FCWidget *getLeftLine();
    FCWidget *getRightLine();

private:
    QHBoxLayout *UPLayout;
    QHBoxLayout *HLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *LLayout;
    QVBoxLayout *RLayout;
    FCColumn *decision;
    FCColumn *doing;
    FCWidget *Divar;
    FCWidget *Yes;
    FCWidget *No;
    FCLeftLine *lline;
    FCRightLine *rline;
    FCBottomLine *bline;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};

class FCPrefixCycleWidget: public FCWidget
{
    Q_OBJECT
public:
    FCPrefixCycleWidget(QWidget *parent = 0, QString text = "Решение");
    ~FCPrefixCycleWidget();
    void paintEvent(QPaintEvent *);
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    FCWidget *getCycleBody();
private:
    QHBoxLayout *UPLayout1;
    QHBoxLayout *UPLayout2;
    QHBoxLayout *HLayout;
    QVBoxLayout *UPLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *Layout;
    FCColumn *decision;
    FCColumn *doing;
    FCDecisionWidget *Cycle;
    FCWidget *Yes;
    FCWidget *No;
    FCWidget *top1;
    FCWidget *top2;
    FCWidget *top1left;
    FCWidget *top2right;
    FCCrossLine *cross;
    FCLeftLine *lline;
    FCRightLine *rline;
    FCBottomLine *bline;
    FCLeftLine *lbline;
    FCRightLine *rbline;
};

class FCPostfixCycleWidget: public FCWidget
{
    Q_OBJECT
public:
    FCPostfixCycleWidget(QWidget *parent = 0, QString text = "Решение");
    ~FCPostfixCycleWidget();
    void paintEvent(QPaintEvent *);
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    FCWidget *getCycleBody();
private:
    QHBoxLayout *UPLayout;
    QHBoxLayout *HLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *Layout;
    FCColumn *decision;
    FCColumn *enter;
    FCCrossLine *cross;
    FCDecisionWidget *Cycle;
    FCWidget *Center;
    FCWidget *LeftSideUp;
    FCWidget *LeftSideDown;
    FCRightLine *rline;
    FCRightLine *rbline;
};

class FCParameterCycleWidget: public FCWidget
{
    Q_OBJECT
public:
    FCParameterCycleWidget(QWidget *parent = 0, QString text = "Параметр");
    ~FCParameterCycleWidget();
    void paintEvent(QPaintEvent *);
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    FCWidget *getCycleBody();
private:
    QHBoxLayout *UPLayout;
    QHBoxLayout *HLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *Layout;
    FCColumn *decision;
    FCColumn *doing;
    FCWidget *Cycle;
    FCWidget *Yes;
    FCWidget *No;
    FCLeftLine *lline;
    FCRightLine *rline;
    FCBottomLine *bline;
    FCLeftLine *lbline;
    FCRightLine *rbline;
};


#endif // FC_WIDGET_H
