#ifndef QWORDCOMPLETER_H
#define QWORDCOMPLETER_H

#include <QCompleter>

class QWordCompleter : public QCompleter
{
    Q_OBJECT
public:
    QWordCompleter(const QStringList& completions, QObject *parent = 0, QString name = "untitled");

    // QObject interface
public:
    virtual bool eventFilter(QObject *o, QEvent *e);
    int getMinCompletionPrefixLength() const;
    void setMinCompletionPrefixLength(int value);
    void setName(QString name);

public slots:
    void replaceCurrentWord(QString text);

private:
    int minCompletionPrefixLength = 0;
    QString name;
};

#endif // QWORDCOMPLETER_H
