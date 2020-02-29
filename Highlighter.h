#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QSyntaxHighlighter>

class TextEditor;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(TextEditor* parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat namespaceFormat;
    QTextCharFormat doubleColonFormat;
    QTextCharFormat numbersFormat;
    QTextCharFormat primitivesFormat;
    QTextCharFormat preprocessorFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};
#endif // HIGHLIGHTER_H
