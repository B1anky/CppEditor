#include "Highlighter.h"
#include "TextEditor.h"

Highlighter::Highlighter(QPlainTextEdit* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor(107, 216, 255));
    keywordFormat.setFontItalic(true);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bclass\\b"),     QStringLiteral("\\bconst\\b"),    QStringLiteral("\\benum\\b"),
        QStringLiteral("\\bexplicit\\b"),  QStringLiteral("\\bfriend\\b"),   QStringLiteral("\\binline\\b"),
        QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"), QStringLiteral("\\bprivate\\b"),
        QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),   QStringLiteral("\\bsignals\\b"),
        QStringLiteral("\\bslots\\b"),     QStringLiteral("\\bstatic\\b"),   QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"),  QStringLiteral("\\btypedef\\b"),  QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"),     QStringLiteral("\\bvirtual\\b"),  QStringLiteral("\\bvolatile\\b"),
        QStringLiteral("\\bif\\b"),        QStringLiteral("\\bthis\\b")
    };
    for (const QString& pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    primitivesFormat.setForeground(QColor(38, 127, 181));
    const QString primitivesPatterns[] = {
        QStringLiteral("\\bchar\\b"),     QStringLiteral("\\bdouble\\b"), QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"),     QStringLiteral("\\bshort\\b"),  QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvoid\\b"),   QStringLiteral("\\bbool\\b"),
        QStringLiteral("\\bfloat\\b"),    QStringLiteral("\\bwchar_t\\b")
    };
    for (const QString& pattern : primitivesPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = primitivesFormat;
        highlightingRules.append(rule);
    }

    //This following the double colon format overwrites the color format of the :: that this sets, this is on purpose and order for the rules matter
    namespaceFormat.setForeground(QColor(92, 176, 242));
    rule.pattern = QRegularExpression(QStringLiteral("\\b\\w*::\\w*\\b"));
    rule.format = namespaceFormat;
    highlightingRules.append(rule);

    //Overrides the color formatting for the double colon in the case of a namespace
    doubleColonFormat.setForeground(QColor(255, 255, 255));
    rule.pattern = QRegularExpression(QStringLiteral("\\b::\\b"));
    rule.format = doubleColonFormat;
    highlightingRules.append(rule);

    preprocessorFormat.setForeground(QColor(92, 176, 242));
    preprocessorFormat.setFontWeight(QFont::Bold);
    const QString preprocessorPatterns[] = {
        QStringLiteral("\\bif\\b"), QStringLiteral("\\belif\\b"), QStringLiteral("\\belse\\b"),
        QStringLiteral("\\bendif\\b"), QStringLiteral("\\bifdef\\b"), QStringLiteral("\\bifndef\\b"),
        QStringLiteral("\\bundef\\b"), QStringLiteral("\\binclude\\b"), QStringLiteral("\\bline\\b"),
        QStringLiteral("\\berror\\b"), QStringLiteral("\\bpragma\\b"), QStringLiteral("\\b__has_include\\b"),
        QStringLiteral("\\b__has_cpp_attribute\\b")
    };
    for (const QString& pattern : preprocessorPatterns) {
        rule.pattern = QRegularExpression("[#]"+pattern);
        rule.format = preprocessorFormat;
        highlightingRules.append(rule);
    }
/*
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);
*/

    //Handles digit constant coloring
    numbersFormat.setForeground(QColor(110, 226, 255));
    rule.pattern = QRegularExpression(QStringLiteral("\\b\\d*\\b"));
    rule.format = numbersFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(QColor(110, 226, 255));
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(153, 254, 254));
    rule.pattern = QRegularExpression(QStringLiteral("\\b[ ][A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(QColor(50, 67, 87));
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(50, 67, 87));

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
