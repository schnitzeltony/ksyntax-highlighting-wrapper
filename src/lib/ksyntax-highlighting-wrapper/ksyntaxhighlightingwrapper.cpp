#include <ksyntaxhighlightingwrapper.h>
#include <ksyntaxhighlightingwrapper_p.h>
#include <QQuickTextDocument>
#include <QQmlEngine>
#include <theme.h>

QT_BEGIN_NAMESPACE

static KSyntaxHighlighting::Repository m_repository;

/////////////////////////////////////////////////////////////////////////////////
// private
KSyntaxHighlightingWrapperPrivate::KSyntaxHighlightingWrapperPrivate(KSyntaxHighlightingWrapper* pPublic) :
    m_highlighter(nullptr),
    m_quickTextDocument(nullptr),
    m_textDocument(nullptr),
    m_caseSensitive(false),
    m_wholeWords(false),
    m_regExpr(false),
    m_highlightColor("yellow"),
    q_ptr(pPublic)
{
}

KSyntaxHighlightingWrapperPrivate::~KSyntaxHighlightingWrapperPrivate()
{
}

bool KSyntaxHighlightingWrapperPrivate::setTextDocument(QTextDocument *textDocument)
{
    bool highlighterChanged = false;
    if(m_textDocument != textDocument) {
        m_textDocument = textDocument;
        if(m_highlighter) {
            delete m_highlighter;
            m_highlighter = nullptr;
            highlighterChanged = true;
        }
        if(textDocument) {
            m_highlighter = new KSyntaxHighlighterEx(textDocument, this);
            highlighterChanged = true;
            m_highlighter->setTheme(m_currentTheme);
            m_highlighter->setDefinition(m_currentDefinition);
            m_highlighter->newSearch();
        }
    }
    return highlighterChanged;
}

QTextDocument *KSyntaxHighlightingWrapperPrivate::textDocument() const
{
    return m_textDocument;
}

bool KSyntaxHighlightingWrapperPrivate::setQmlTextDocument(QQuickTextDocument *qmlTextDocument)
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

QQuickTextDocument *KSyntaxHighlightingWrapperPrivate::qmlTextDocument() const
{
    return m_quickTextDocument;
}

bool KSyntaxHighlightingWrapperPrivate::setDefinition(KSyntaxHighlighting::Definition def)
{
    bool changed = false;
    if(def.isValid() || def == KSyntaxHighlighting::Definition() /* None is ok */) {
        if(def != m_currentDefinition) {
            m_currentDefinition = def;
            changed = true;
            if(m_highlighter) {
                // Although ksyntaxhighlighter calls rehighlight, the document
                // is not re-rendered. So choose the brute force way: Unload
                // and reload highlighter
                QTextDocument *currDocument = m_textDocument;
                setTextDocument(nullptr);
                setTextDocument(currDocument);
                //m_highlighter->setDefinition(def);
            }
        }
    }
    return changed;
}

KSyntaxHighlighting::Definition KSyntaxHighlightingWrapperPrivate::definition()
{
    return m_currentDefinition;
}

bool KSyntaxHighlightingWrapperPrivate::setDefinitionForFileName(const QString &fileName)
{
    const auto def = m_repository.definitionForFileName(fileName);
    return setDefinition(def);
}

bool KSyntaxHighlightingWrapperPrivate::setDefinitionForMimeType(const QString &mimeType)
{
    const auto def = m_repository.definitionForMimeType(mimeType);
    return setDefinition(def);
}

const QString KSyntaxHighlightingWrapperPrivate::definitionName()
{
    return definition().name();
}

bool KSyntaxHighlightingWrapperPrivate::setDefinitionName(const QString &definitionName)
{
    const auto def = m_repository.definitionForName(definitionName);
    return setDefinition(def);
}

const QStringList KSyntaxHighlightingWrapperPrivate::definitionNames() const
{
    QStringList definitionNames;
    for(auto definition : m_repository.definitions()) {
        definitionNames.append(definition.name());
    }
    return definitionNames;
}

int KSyntaxHighlightingWrapperPrivate::definitionNumber()
{
    return definitionNames().indexOf(definitionName());
}

bool KSyntaxHighlightingWrapperPrivate::setDefinitionNumber(const int definitionNumber)
{
    bool changed = false;
    if(definitionNumber >= 0) {
        QStringList names = definitionNames();
        if(definitionNumber < names.count()) {
            changed = setDefinitionName(names[definitionNumber]);
        }
    }
    return changed;
}

bool KSyntaxHighlightingWrapperPrivate::setTheme(KSyntaxHighlighting::Theme theme)
{
    bool changed = false;
    if(theme.isValid()) {
        if(theme.name() != m_currentTheme.name()) {
            m_currentTheme = theme;
            changed = true;
            if(m_highlighter) {
                // ksyntaxhighlighter does not call rehighlight due to it's
                // design. So choose the sam brute force way as setDefinition
                // does: Unload and reload highlighter
                QTextDocument *currDocument = m_textDocument;
                setTextDocument(nullptr);
                setTextDocument(currDocument);
            }
        }
    }
    return changed;
}

const QString KSyntaxHighlightingWrapperPrivate::themeName()
{
    return theme().name();
}

KSyntaxHighlighting::Theme KSyntaxHighlightingWrapperPrivate::theme()
{
    return m_currentTheme;
}

bool KSyntaxHighlightingWrapperPrivate::setThemeName(const QString &themeName)
{
    const auto theme = m_repository.theme(themeName);
    return setTheme(theme);
}

int KSyntaxHighlightingWrapperPrivate::themeNumber()
{
    int themeNum = -1;
    QVector<KSyntaxHighlighting::Theme> themes = m_repository.themes();
    for(int iTheme=0; iTheme<themes.size(); iTheme++) {
        if(themes[iTheme].name() == theme().name()) {
            themeNum = iTheme;
            break;
        }
    }
    return themeNum;
}

bool KSyntaxHighlightingWrapperPrivate::setThemeNumber(const int themeNumber)
{
    bool changed = false;
    if(themeNumber >= 0) {
        QVector<KSyntaxHighlighting::Theme> themes = m_repository.themes();
        if(themeNumber < themes.count()) {
            changed = setTheme(themes[themeNumber]);
        }
    }
    return changed ;
}

const QStringList KSyntaxHighlightingWrapperPrivate::themeNames() const
{
    QStringList themeNames;
    for(auto theme : m_repository.themes()) {
        themeNames.append(theme.name());
    }
    return themeNames;
}

const QStringList KSyntaxHighlightingWrapperPrivate::themeNamesTranslated() const
{
    QStringList themeNamesTranslated;
    for(auto theme : m_repository.themes()) {
        themeNamesTranslated.append(theme.translatedName());
    }
    return themeNamesTranslated;
}

QString KSyntaxHighlightingWrapperPrivate::search() const
{
    return m_search;
}

bool KSyntaxHighlightingWrapperPrivate::setSearch(const QString &search)
{
    bool bChanged = false;
    if(m_search != search) {
        m_search = search;
        bChanged = true;
        if(m_highlighter) {
            m_highlighter->newSearch();
        }
    }
    return bChanged;
}

bool KSyntaxHighlightingWrapperPrivate::caseSensitive() const
{
    return m_caseSensitive;
}

bool KSyntaxHighlightingWrapperPrivate::setCaseSensitive(const bool caseSensitive)
{
    bool bChanged = false;
    if(m_caseSensitive != caseSensitive) {
        m_caseSensitive = caseSensitive;
        bChanged = true;
        if(m_highlighter) {
            m_highlighter->newSearch();
        }
    }
    return bChanged;
}

bool KSyntaxHighlightingWrapperPrivate::wholeWords() const
{
    return m_wholeWords;
}

bool KSyntaxHighlightingWrapperPrivate::setWholeWords(const bool wholeWords)
{
    bool bChanged = false;
    if(m_wholeWords != wholeWords) {
        m_wholeWords = wholeWords;
        bChanged = true;
        if(m_highlighter) {
            m_highlighter->newSearch();
        }
    }
    return bChanged;
}

bool KSyntaxHighlightingWrapperPrivate::regExpr() const
{
    return m_regExpr;
}

bool KSyntaxHighlightingWrapperPrivate::setRegExpr(const bool regExpr)
{
    bool bChanged = false;
    if(m_regExpr != regExpr) {
        bChanged = true;
        if(m_highlighter) {
            m_highlighter->newSearch();
        }
    }
    return bChanged;
}

QString KSyntaxHighlightingWrapperPrivate::replace() const
{
    return m_replace;
}

bool KSyntaxHighlightingWrapperPrivate::setReplace(const QString &replace)
{
    bool bChanged = false;
    if(m_replace != replace) {
        m_replace = replace;
        bChanged = true;
    }
    return bChanged;
}

const QColor KSyntaxHighlightingWrapperPrivate::highlightColor()
{
    return m_highlightColor;
}

bool KSyntaxHighlightingWrapperPrivate::setHighlightColor(const QColor highlightColor)
{
    bool colorChanged = false;
    if(highlightColor != m_highlightColor) {
        m_highlightColor = highlightColor;
        colorChanged = true;
        if(m_highlighter) {
            m_highlighter->newSearch();
        }
    }
    return colorChanged;
}

/////////////////////////////////////////////////////////////////////////////////
// public
KSyntaxHighlightingWrapper::KSyntaxHighlightingWrapper(QObject *parent) :
    QObject(parent),
    d_ptr(new KSyntaxHighlightingWrapperPrivate(this))
{
}

KSyntaxHighlightingWrapper::KSyntaxHighlightingWrapper(KSyntaxHighlightingWrapperPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
}

KSyntaxHighlightingWrapper::~KSyntaxHighlightingWrapper()
{
}

void KSyntaxHighlightingWrapper::registerQml()
{
    qmlRegisterType<KSyntaxHighlightingWrapper>("KSyntaxHighlighting", 1, 0, "KSyntaxHighlighting");
}

void KSyntaxHighlightingWrapper::setTextDocument(QTextDocument *textDocument)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setTextDocument(textDocument)) {
        emit documentChanged();
    }
}

QTextDocument *KSyntaxHighlightingWrapper::textDocument() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->textDocument();
}

void KSyntaxHighlightingWrapper::setQmlTextDocument(QQuickTextDocument *qmlTextDocument)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setQmlTextDocument(qmlTextDocument)) {
        emit documentChanged();
    }
}

QQuickTextDocument *KSyntaxHighlightingWrapper::qmlTextDocument() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->qmlTextDocument();
}

void KSyntaxHighlightingWrapper::setDefinitionForFileName(const QString &fileName)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setDefinitionForFileName(fileName)) {
        emit definitionChanged();
    }
}

void KSyntaxHighlightingWrapper::setDefinitionForMimeType(const QString &mimeType)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setDefinitionForMimeType(mimeType)) {
        emit definitionChanged();
    }
}

void KSyntaxHighlightingWrapper::findPrevious()
{
    // TODO
}

void KSyntaxHighlightingWrapper::findNext()
{
    // TODO
}


 const QString KSyntaxHighlightingWrapper::definitionName()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->definitionName();
}

void KSyntaxHighlightingWrapper::setDefinitionName(const QString &definitionName)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setDefinitionName(definitionName)) {
        emit definitionChanged();
    }
}

const int KSyntaxHighlightingWrapper::definitionNumber()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->definitionNumber();
}

void KSyntaxHighlightingWrapper::setDefinitionNumber(const int definitionNumber)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setDefinitionNumber(definitionNumber)) {
        emit definitionChanged();
    }
}

const QStringList KSyntaxHighlightingWrapper::definitionNames() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->definitionNames();
}

const QString KSyntaxHighlightingWrapper::themeName()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->themeName();
}

void KSyntaxHighlightingWrapper::setThemeName(const QString &themeName)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setThemeName(themeName)) {
        emit themeChanged();
    }
}

const QString KSyntaxHighlightingWrapper::themeNameTranslated()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->theme().translatedName();
}

int KSyntaxHighlightingWrapper::themeNumber()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->themeNumber();
}

void KSyntaxHighlightingWrapper::setThemeNumber(const int themeNumber)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setThemeNumber(themeNumber)) {
        emit themeChanged();
    }
}

const QStringList KSyntaxHighlightingWrapper::themeNames() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->themeNames();
}

const QStringList KSyntaxHighlightingWrapper::themeNamesTranslated() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->themeNamesTranslated();
}

QString KSyntaxHighlightingWrapper::search() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->search();
}

void KSyntaxHighlightingWrapper::setSearch(const QString &search)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setSearch(search)) {
        emit searchParamChanged();
    }
}

bool KSyntaxHighlightingWrapper::caseSensitive() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->caseSensitive();
}

void KSyntaxHighlightingWrapper::setCaseSensitive(const bool caseSensitive)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setCaseSensitive(caseSensitive)) {
        emit searchParamChanged();
    }
}

bool KSyntaxHighlightingWrapper::wholeWords() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->wholeWords();
}

void KSyntaxHighlightingWrapper::setWholeWords(const bool wholeWords)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setWholeWords(wholeWords)) {
        emit searchParamChanged();
    }
}

bool KSyntaxHighlightingWrapper::regExpr() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->regExpr();
}

void KSyntaxHighlightingWrapper::setRegExpr(const bool regExpr)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setRegExpr(regExpr)) {
        emit searchParamChanged();
    }
}

QString KSyntaxHighlightingWrapper::replace() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->replace();
}

void KSyntaxHighlightingWrapper::setReplace(const QString &replace)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setReplace(replace)) {
        emit replaceChanged();
    }
}

const QColor KSyntaxHighlightingWrapper::highlightColor()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->highlightColor();
}

void KSyntaxHighlightingWrapper::setHighlightColor(const QColor highlightColor)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->setHighlightColor(highlightColor)) {
        emit highlightColorChanged();
    }
}

QT_END_NAMESPACE
