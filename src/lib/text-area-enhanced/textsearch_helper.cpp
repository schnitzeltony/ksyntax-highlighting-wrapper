#include "textsearch_helper.h"
#include "textsearch_helper_p.h"
#include <QQmlEngine>
#include <QQuickTextDocument>

/////////////////////////////////////////////////////////////////////////////////
// private

TextSearchHelperPrivate::TextSearchHelperPrivate(TextSearchHelper *pPublic) :
    m_quickTextDocument(nullptr),
    m_textDocument(nullptr),
    m_caseSensitive(false),
    m_wholeWords(false),
    m_highlightColor("yellow"),
    m_highlighter(new TextSearchHighlighter(this)),
    q_ptr(pPublic)
{
}

TextSearchHelperPrivate::~TextSearchHelperPrivate()
{
    delete m_highlighter;
}

bool TextSearchHelperPrivate::setTextDocument(QTextDocument *textDocument)
{
    bool textDocumentChanged = false;
    if(m_textDocument != textDocument) {
        m_textDocument = textDocument;
        m_highlighter->setDocument(textDocument);
        // TODO: rehighlight?
        textDocumentChanged = true;
    }
    return textDocumentChanged;
}

QTextDocument *TextSearchHelperPrivate::textDocument() const
{
    return m_textDocument;
}

bool TextSearchHelperPrivate::setQmlTextDocument(QQuickTextDocument *qmlTextDocument)
{
    bool documentChanged = false;
    if(qmlTextDocument != m_quickTextDocument) {
       m_quickTextDocument = qmlTextDocument;
       documentChanged = true;
       if(qmlTextDocument) {
           setTextDocument(qmlTextDocument->textDocument());
       }
       else {
           setTextDocument(nullptr);
       }
    }
    return documentChanged;
}

QQuickTextDocument *TextSearchHelperPrivate::qmlTextDocument() const
{
    return m_quickTextDocument;
}

QString TextSearchHelperPrivate::search() const
{
    return m_search;
}

bool TextSearchHelperPrivate::setSearch(const QString &search)
{
    bool bChanged = false;
    if(m_search != search) {
        m_search = search;
        m_highlighter->rehighlight();
        bChanged = true;
    }
    return bChanged;
}

bool TextSearchHelperPrivate::caseSensitive() const
{
    return m_caseSensitive;
}

bool TextSearchHelperPrivate::setCaseSensitive(const bool caseSensitive)
{
    bool bChanged = false;
    if(m_caseSensitive != caseSensitive) {
        m_caseSensitive = caseSensitive;
        if(!m_search.isEmpty()) {
            m_highlighter->rehighlight();
        }
        bChanged = true;
    }
    return bChanged;
}

bool TextSearchHelperPrivate::wholeWords() const
{
    return m_wholeWords;
}

bool TextSearchHelperPrivate::setWholeWords(const bool wholeWords)
{
    bool bChanged = false;
    if(m_wholeWords != wholeWords) {
        m_wholeWords = wholeWords;
        if(!m_search.isEmpty()) {
            m_highlighter->rehighlight();
        }
        bChanged = true;
    }
    return bChanged;
}

QString TextSearchHelperPrivate::replace() const
{
    return m_replace;
}

bool TextSearchHelperPrivate::setReplace(const QString &replace)
{
    bool bChanged = false;
    if(m_replace != replace) {
        m_replace = replace;
        bChanged = true;
    }
    return bChanged;
}

const QColor TextSearchHelperPrivate::highlightColor()
{
    return m_highlightColor;
}

bool TextSearchHelperPrivate::setHighlightColor(const QColor highlightColor)
{
    bool colorChanged = false;
    if(highlightColor != m_highlightColor) {
        m_highlightColor = highlightColor;
        colorChanged = true;
        if(!m_search.isEmpty()) {
            m_highlighter->rehighlight();
        }
    }
    return colorChanged;
}

/////////////////////////////////////////////////////////////////////////////////
// public

TextSearchHelper::TextSearchHelper(QObject *parent) :
    QObject(parent),
    d_ptr(new TextSearchHelperPrivate(this))
{
}

TextSearchHelper::~TextSearchHelper()
{
}

void TextSearchHelper::registerQml()
{
    qmlRegisterType<TextSearchHelper>("TextSearchHelper", 1, 0, "TextSearchHelper");
}

void TextSearchHelper::findPrevious()
{
    // TODO
}

void TextSearchHelper::findNext()
{
    // TODO
}

void TextSearchHelper::setTextDocument(QTextDocument *textDocument)
{
    Q_D(TextSearchHelper);
    if(d->setTextDocument(textDocument)) {
        emit documentChanged();
    }
}

QTextDocument *TextSearchHelper::textDocument() const
{
    Q_D(const TextSearchHelper);
    return d->textDocument();
}

void TextSearchHelper::setQmlTextDocument(QQuickTextDocument *qmlTextDocument)
{
    Q_D(TextSearchHelper);
    if(d->setQmlTextDocument(qmlTextDocument)) {
        emit documentChanged();
    }
}

QQuickTextDocument *TextSearchHelper::qmlTextDocument() const
{
    Q_D(const TextSearchHelper);
    return d->qmlTextDocument();
}

QString TextSearchHelper::search() const
{
    Q_D(const TextSearchHelper);
    return d->search();
}

void TextSearchHelper::setSearch(const QString &search)
{
    Q_D(TextSearchHelper);
    if(d->setSearch(search)) {
        emit searchParamChanged();
    }
}

bool TextSearchHelper::caseSensitive() const
{
    Q_D(const TextSearchHelper);
    return d->caseSensitive();
}

void TextSearchHelper::setCaseSensitive(const bool caseSensitive)
{
    Q_D(TextSearchHelper);
    if(d->setCaseSensitive(caseSensitive)) {
        emit searchParamChanged();
    }
}

bool TextSearchHelper::wholeWords() const
{
    Q_D(const TextSearchHelper);
    return d->wholeWords();
}

void TextSearchHelper::setWholeWords(const bool wholeWords)
{
    Q_D(TextSearchHelper);
    if(d->setWholeWords(wholeWords)) {
        emit searchParamChanged();
    }
}

QString TextSearchHelper::replace() const
{
    Q_D(const TextSearchHelper);
    return d->replace();
}

void TextSearchHelper::setReplace(const QString &replace)
{
    Q_D(TextSearchHelper);
    if(d->setReplace(replace)) {
        emit replaceChanged();
    }
}

const QColor TextSearchHelper::highlightColor()
{
    Q_D(TextSearchHelper);
    return d->highlightColor();
}

void TextSearchHelper::setHighlightColor(const QColor highlightColor)
{
    Q_D(TextSearchHelper);
    if(d->setHighlightColor(highlightColor)) {
        emit highlightColorChanged();
    }

}
