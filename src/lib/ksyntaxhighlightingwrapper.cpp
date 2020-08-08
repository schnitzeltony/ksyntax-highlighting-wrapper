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
        m_highlighter->setDefinition(m_currentDefinition);
        m_highlighter->setTheme(m_currentTheme);

    }
    return highlighterChanged;
}

bool KSyntaxHighlightingWrapperPrivate::setDefinition(KSyntaxHighlighting::Definition def)
{
    bool changed = false;
    if(def.isValid()) {
        if(def != m_currentDefinition) {
            m_currentDefinition = def;
            changed = true;
            if(m_highlighter) {
                m_highlighter->setDefinition(def);
            }

        }
    }
    return changed;
}

KSyntaxHighlighting::Definition KSyntaxHighlightingWrapperPrivate::definition()
{
    return m_currentDefinition;
}

bool KSyntaxHighlightingWrapperPrivate::setTheme(KSyntaxHighlighting::Theme theme)
{
    bool changed = false;
    if(theme.isValid()) {
        if(theme.name() != m_currentTheme.name()) {
            m_currentTheme = theme;
            changed = true;
            if(m_highlighter) {
                m_highlighter->setTheme(theme);
            }
        }
    }
    return changed;
}

KSyntaxHighlighting::Theme KSyntaxHighlightingWrapperPrivate::theme()
{
    return m_currentTheme;
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
    return d->definition().name();
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
    return d->theme().name();
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
    return d->theme().translatedName();
}

int KSyntaxHighlightingWrapper::themeNumber()
{
    Q_D(KSyntaxHighlightingWrapper);
    int themeNum = -1;
    QVector<KSyntaxHighlighting::Theme> themes = d->m_repository.themes();
    for(int iTheme=0; iTheme<themes.size(); iTheme++) {
        if(themes[iTheme].name() == d->theme().name()) {
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
