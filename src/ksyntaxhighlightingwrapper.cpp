#include <ksyntaxhighlightingwrapper.h>
#include <ksyntaxhighlightingwrapper_p.h>
#include <QQuickTextDocument>
#include <theme.h>

QT_BEGIN_NAMESPACE

KSyntaxHighlighting::Repository KSyntaxHighlightingWrapperPrivate::m_repository;

// private
KSyntaxHighlightingWrapperPrivate::KSyntaxHighlightingWrapperPrivate(KSyntaxHighlightingWrapper* pPublic) :
    m_highlighter(nullptr),
    m_quickTextDocument(nullptr),
    q_ptr(pPublic)
{
}

KSyntaxHighlightingWrapperPrivate::~KSyntaxHighlightingWrapperPrivate()
{
}

bool KSyntaxHighlightingWrapperPrivate::setTextDocument(QTextDocument *textDocument)
{
    bool highlighterChanged = false;
    if(m_highlighter) {
        delete m_highlighter;
        m_highlighter = nullptr;
        highlighterChanged = true;
    }
    if(textDocument) {
        m_highlighter = new KSyntaxHighlighting::SyntaxHighlighter(textDocument);
        highlighterChanged = true;
    }
    return highlighterChanged;
}

bool KSyntaxHighlightingWrapperPrivate::setDefinition(KSyntaxHighlighting::Definition def)
{
    if(def.isValid()) {
        if(m_highlighter) {
            if(def != m_highlighter->definition()) {
                m_highlighter->setDefinition(def);
                return true;
            }
        }
        else {
            m_definitionNoHighlighter = def;
        }
    }
    return false;
}

KSyntaxHighlighting::Definition KSyntaxHighlightingWrapperPrivate::currentDefinition()
{
    if(m_highlighter) {
        return m_highlighter->definition();
    }
    else {
        return m_definitionNoHighlighter;
    }
}

bool KSyntaxHighlightingWrapperPrivate::setTheme(KSyntaxHighlighting::Theme theme)
{
    if(theme.isValid()) {
        if(m_highlighter) {
            if(theme.name() != m_highlighter->theme().name()) {
                m_highlighter->setTheme(theme);
                return true;
            }
        }
        else {
            m_themeNoHighlighter = theme;
        }
    }
    return false;
}

KSyntaxHighlighting::Theme KSyntaxHighlightingWrapperPrivate::curentTheme()
{
    if(m_highlighter) {
        return m_highlighter->theme();
    }
    else {
        return m_themeNoHighlighter;
    }
}

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

        // Did we receive setting at times we had no document?
        if(d->m_definitionNoHighlighter.isValid() && d->setDefinition(d->m_definitionNoHighlighter)) {
            emit definitionChanged();
        }
        d->m_definitionNoHighlighter = KSyntaxHighlighting::Definition();

        if(d->m_themeNoHighlighter.isValid() && d->setTheme(d->m_themeNoHighlighter)) {
            emit themeChanged();
        }
        d->m_themeNoHighlighter = KSyntaxHighlighting::Theme();
    }
}

QTextDocument *KSyntaxHighlightingWrapper::textDocument() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    if(d->m_highlighter) {
        return d->m_highlighter->document();
    }
    else {
        return nullptr;
    }
}

void KSyntaxHighlightingWrapper::setQmlTextDocument(QQuickTextDocument *qmlTextDocument)
{
    Q_D(KSyntaxHighlightingWrapper);
    d->m_quickTextDocument = qmlTextDocument;
    if(qmlTextDocument) {
        setTextDocument(qmlTextDocument->textDocument());
    }
}

QQuickTextDocument *KSyntaxHighlightingWrapper::qmlTextDocument() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    return d->m_quickTextDocument;
}

void KSyntaxHighlightingWrapper::definitionForFileName(const QString &fileName)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto def = d->m_repository.definitionForFileName(fileName);
    if(d->setDefinition(def))
        emit definitionChanged();
}

void KSyntaxHighlightingWrapper::definitionForMimeType(const QString &mimeType)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto def = d->m_repository.definitionForMimeType(mimeType);
    if(d->setDefinition(def))
        emit definitionChanged();
}

const QString KSyntaxHighlightingWrapper::definitionName()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->currentDefinition().name();
}

void KSyntaxHighlightingWrapper::setDefinitionName(const QString &definitionName)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto def = d->m_repository.definitionForName(definitionName);
    if(d->setDefinition(def))
        emit definitionChanged();
}

const QStringList KSyntaxHighlightingWrapper::definitionNames() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QStringList definitionNames;
    for(auto definition : d->m_repository.definitions()) {
        definitionNames.append(definition.name());
    }
    return definitionNames;
}

const QString KSyntaxHighlightingWrapper::themeName()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->curentTheme().name();
}

void KSyntaxHighlightingWrapper::setThemeName(const QString &themeName)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto theme = d->m_repository.theme(themeName);
    if(d->setTheme(theme)) {
        emit themeChanged();
    }
}

const QString KSyntaxHighlightingWrapper::themeNameTranslated()
{
    Q_D(KSyntaxHighlightingWrapper);
    return d->curentTheme().translatedName();
}

int KSyntaxHighlightingWrapper::themeNumber()
{
    Q_D(KSyntaxHighlightingWrapper);
    int themeNum = -1;
    QVector<KSyntaxHighlighting::Theme> themes = d->m_repository.themes();
    for(int iTheme=0; iTheme<themes.size(); iTheme++) {
        if(themes[iTheme].name() == d->curentTheme().name()) {
            themeNum = iTheme;
            break;
        }
    }
    return themeNum;
}

void KSyntaxHighlightingWrapper::setThemeNumber(const int themeNumber)
{
    Q_D(KSyntaxHighlightingWrapper);
    QVector<KSyntaxHighlighting::Theme> themes = d->m_repository.themes();
    if(themeNumber < themes.count()) {
        if(d->setTheme(themes[themeNumber])) {
            emit themeChanged();
        }
    }
}

const QStringList KSyntaxHighlightingWrapper::themeNames() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QStringList themeNames;
    for(auto theme : d->m_repository.themes()) {
        themeNames.append(theme.name());
    }
    return themeNames;
}

const QStringList KSyntaxHighlightingWrapper::themeNamesTranslated() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QStringList themeNames;
    for(auto theme : d->m_repository.themes()) {
        themeNames.append(theme.translatedName());
    }
    return themeNames;
}

QT_END_NAMESPACE
