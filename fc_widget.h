#ifndef FC_WIDGET_H
#define FC_WIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>


//void SetNewSize(QWidget *widget);

class Block_Widget: public QWidget
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
    Block_Widget(QWidget *parent = 0);
    ~Block_Widget();
    virtual void UpdateSize();
    virtual int GetMaxWidth();
    virtual std::string returnTag();
    virtual QString getText();
    virtual QString getDefaultText();
    void createBlock(int index);
    void addBlock(int index, Block_Widget *widget, bool withLine = true);
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

class MultyColumn: public Block_Widget
{
public:
    MultyColumn(QWidget *parent = 0);
    ~MultyColumn(){}
    void UpdateSize();
};

class LeftLine: public Block_Widget
{
    Q_OBJECT
public:
    int margin;
    LeftLine(QWidget *parent = 0, bool bottom = false);
    ~LeftLine(){}
private:
    bool isBottom;
    void paintEvent(QPaintEvent *);
};

class RightLine: public Block_Widget
{
    Q_OBJECT
public:
    int margin;
    RightLine(QWidget *parent = 0, bool bottom = false, bool back = false, bool arrow = false);
    ~RightLine(){}
private:
    bool isBottom;
    bool isBack;
    bool isArrow;
    void paintEvent(QPaintEvent *);
};

class BottomLine: public QWidget
{
    Q_OBJECT
public:
    int marginLeft;
    int marginRight;
    BottomLine(QWidget *parent = 0, bool left = false);
    ~BottomLine();
private:
    bool isLeftSide;
    void paintEvent(QPaintEvent *);
};

class Line_Widget: public Block_Widget
{
    Q_OBJECT
public:
    Line_Widget(QWidget *parent = 0);
    ~Line_Widget();
    bool isArrow();
private:
    bool ForceArrow;
    bool isUnderCursor;
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);

signals:
    void clicked(Line_Widget* object);
public slots:
    void lineclicked(Line_Widget* object);
};

class CrossLine_Widget: public Block_Widget
{
    Q_OBJECT
public:
    CrossLine_Widget(QWidget *parent = 0, bool SetArrow = false);
    ~CrossLine_Widget();
private:
    bool isArrow;
    void paintEvent(QPaintEvent *);
};

class BlockBegin_Widget: public Block_Widget
{
    Q_OBJECT
public:
    BlockBegin_Widget(QWidget *parent = 0);
    ~BlockBegin_Widget();
private:
    void paintEvent(QPaintEvent *);
};

class BlockEnd_Widget: public Block_Widget
{
    Q_OBJECT
public:
    BlockEnd_Widget(QWidget *parent = 0);
    ~BlockEnd_Widget();
private:
    void paintEvent(QPaintEvent *);
};

class BlockAction_Widget: public Block_Widget
{
    Q_OBJECT
public:
    BlockAction_Widget(QWidget *parent = 0, QString text = "Действие");
    ~BlockAction_Widget();
private:
    void paintEvent(QPaintEvent *);
};

class BlockInput_Widget: public Block_Widget
{
    Q_OBJECT
public:
    BlockInput_Widget(QWidget *parent = 0, QString text = "Ввод");
    ~BlockInput_Widget();
private:
    void paintEvent(QPaintEvent *);
};

class BlockOutput_Widget: public Block_Widget
{
    Q_OBJECT
public:
    BlockOutput_Widget(QWidget *parent = 0, QString text = "Вывод");
    ~BlockOutput_Widget();
private:
    void paintEvent(QPaintEvent *);
};

class Decision_Widget: public Block_Widget
{
    Q_OBJECT
public:
    Decision_Widget(QWidget *parent = 0, QString text = "Решение", bool isPrefix = false);
    ~Decision_Widget();
private:
    void paintEvent(QPaintEvent *);
    bool isPrefix;
public slots:
    void block_clicked();
};

class Parameter_Widget: public Block_Widget
{
    Q_OBJECT
public:
    Parameter_Widget(QWidget *parent = 0, QString text = "Параметр");
    ~Parameter_Widget();
private:
    void paintEvent(QPaintEvent *);
public slots:
    void block_clicked();
};

class Divar_Widget: public Block_Widget
{
    Q_OBJECT
public:
    Divar_Widget(QWidget *parent = 0, QString text = "Решение");
    ~Divar_Widget();
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    Block_Widget *getLeftLine();
    Block_Widget *getRightLine();

private:
    QHBoxLayout *UPLayout;
    QHBoxLayout *HLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *LLayout;
    QVBoxLayout *RLayout;
    MultyColumn *decision;
    MultyColumn *doing;
    Block_Widget *Divar;
    Block_Widget *Yes;
    Block_Widget *No;
    LeftLine *lline;
    RightLine *rline;
    BottomLine *bline;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};

class Prefix_Cycle: public Block_Widget
{
    Q_OBJECT
public:
    Prefix_Cycle(QWidget *parent = 0, QString text = "Решение");
    ~Prefix_Cycle();
    void paintEvent(QPaintEvent *);
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    Block_Widget *getCycleBody();
private:
    QHBoxLayout *UPLayout1;
    QHBoxLayout *UPLayout2;
    QHBoxLayout *HLayout;
    QVBoxLayout *UPLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *Layout;
    MultyColumn *decision;
    MultyColumn *doing;
    Decision_Widget *Cycle;
    Block_Widget *Yes;
    Block_Widget *No;
    Block_Widget *top1;
    Block_Widget *top2;
    Block_Widget *top1left;
    Block_Widget *top2right;
    CrossLine_Widget *cross;
    LeftLine *lline;
    RightLine *rline;
    BottomLine *bline;
    LeftLine *lbline;
    RightLine *rbline;
};

class Postfix_Cycle: public Block_Widget
{
    Q_OBJECT
public:
    Postfix_Cycle(QWidget *parent = 0, QString text = "Решение");
    ~Postfix_Cycle();
    void paintEvent(QPaintEvent *);
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    Block_Widget *getCycleBody();
private:
    QHBoxLayout *UPLayout;
    QHBoxLayout *HLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *Layout;
    MultyColumn *decision;
    MultyColumn *enter;
    CrossLine_Widget *cross;
    Decision_Widget *Cycle;
    Block_Widget *Center;
    Block_Widget *LeftSideUp;
    Block_Widget *LeftSideDown;
    RightLine *rline;
    RightLine *rbline;
};

class Parameter_Cycle: public Block_Widget
{
    Q_OBJECT
public:
    Parameter_Cycle(QWidget *parent = 0, QString text = "Параметр");
    ~Parameter_Cycle();
    void paintEvent(QPaintEvent *);
    void UpdateSize();
    int GetMaxWidth();
    QString getText();
    Block_Widget *getCycleBody();
private:
    QHBoxLayout *UPLayout;
    QHBoxLayout *HLayout;
    QVBoxLayout *VLayout;
    QVBoxLayout *Layout;
    MultyColumn *decision;
    MultyColumn *doing;
    Block_Widget *Cycle;
    Block_Widget *Yes;
    Block_Widget *No;
    LeftLine *lline;
    RightLine *rline;
    BottomLine *bline;
    LeftLine *lbline;
    RightLine *rbline;
};


#endif // FC_WIDGET_H
