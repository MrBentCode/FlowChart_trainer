#include "qwordcompleter.h"
#include <QEvent>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QTextEdit>
#include <QTextCursor>

QWordCompleter::QWordCompleter(const QStringList &completions, QObject *parent):
    QCompleter(completions, parent)
{
}

bool QWordCompleter::eventFilter(QObject *o, QEvent *e)
{
    if(widget()->inherits("QTextEdit") && e->type() == QEvent::KeyPress){
        QKeyEvent* ke = static_cast<QKeyEvent*>(e);
        switch (ke->key()) {
        case Qt::Key_Space:
            if(ke->modifiers().testFlag(Qt::ControlModifier)){
                QTextEdit* textEdit = qobject_cast<QTextEdit*>(widget());
                QTextCursor textCursor = textEdit->textCursor();
                textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
                if(textCursor.selectedText().length() >= minCompletionPrefixLength){
                    setCompletionPrefix(textCursor.selectedText());
                    QRect rect = QRect(textEdit->cursorRect().bottomLeft(), QSize(100, 5));
                    complete(rect);
                }
                return true;
            }
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Tab:
            if(popup()->isVisible()){
                popup()->hide();
                if(popup()->currentIndex().isValid()){
                    emit activated(popup()->currentIndex());
                    emit activated(popup()->currentIndex().data(completionRole()).toString());
                }
                return true;
            }
        default:
            break;
        }
    }
    return QCompleter::eventFilter(o, e);

}
int QWordCompleter::getMinCompletionPrefixLength() const
{
    return minCompletionPrefixLength;
}

void QWordCompleter::setMinCompletionPrefixLength(int value)
{
    minCompletionPrefixLength = value;
}

void QWordCompleter::replaceCurrentWord(QString text)
{
    QTextEdit* textEdit = qobject_cast<QTextEdit*>(widget());
    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition(QTextCursor::StartOfWord);
    textCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    textCursor.insertText(text);
    textEdit->setTextCursor(textCursor);
}

