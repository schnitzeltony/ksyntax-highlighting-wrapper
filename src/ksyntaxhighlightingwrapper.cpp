#include <ksyntaxhighlightingwrapper.h>
#include <ksyntaxhighlightingwrapper_p.h>
#include <QQuickTextDocument>
#include <theme.h>

QT_BEGIN_NAMESPACE

KSyntaxHighlighting::Repository KSyntaxHighlightingWrapperPrivate::m_repository;

// private
KSyntaxHighlightingWrapperPrivate::KSyntaxHighlightingWrapperPrivate(KSyntaxHighlightingWrapper* pPublic) :
    m_highlighter(nullptr),
    q_ptr(pPublic)
{
}

KSyntaxHighlightingWrapperPrivate::~KSyntaxHighlightingWrapperPrivate()
{
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

void KSyntaxHighlightingWrapper::setTextDocument(QObject *textDocument)
{
    Q_D(KSyntaxHighlightingWrapper);
    QTextDocument* textDocumentCast = qobject_cast<QTextDocument *>(textDocument);
    if(d->m_highlighter && textDocumentCast != d->m_highlighter->document()) {
        if(d->m_highlighter->definition().isValid()) {
            emit definitionChanged();
        }
        if(d->m_highlighter->theme().isValid()) {
            emit themeChanged();
        }
        delete d->m_highlighter;
        d->m_highlighter = nullptr;
    }
    if(textDocumentCast) {
        d->m_highlighter = new KSyntaxHighlighting::SyntaxHighlighter(textDocumentCast);
    }
}

void KSyntaxHighlightingWrapper::setQmlTextDocument(QObject *qmlTextDocument)
{
    QQuickTextDocument* qmlTextDocumentCast = qobject_cast<QQuickTextDocument *>(qmlTextDocument);
    if(qmlTextDocumentCast) {
        setTextDocument(qmlTextDocumentCast->textDocument());
    }
}

void KSyntaxHighlightingWrapper::definitionForFileName(const QString &fileName)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto def = d->m_repository.definitionForFileName(fileName);
    if(d->m_highlighter && def.isValid()) {
        d->m_highlighter->setDefinition(def);
        emit definitionChanged();
    }
}

void KSyntaxHighlightingWrapper::definitionForMimeType(const QString &mimeType)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto def = d->m_repository.definitionForMimeType(mimeType);
    if(d->m_highlighter && def.isValid()) {
        d->m_highlighter->setDefinition(def);
        emit definitionChanged();
    }
}

const QString KSyntaxHighlightingWrapper::definitionName() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QString strName;
    if(d->m_highlighter) {
        strName = d->m_highlighter->definition().name();
    }
    return strName;
}

void KSyntaxHighlightingWrapper::setDefinitionName(const QString &definitionName)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto def = d->m_repository.definitionForName(definitionName);
    if(d->m_highlighter && def.isValid()) {
        d->m_highlighter->setDefinition(def);
        emit definitionChanged();
    }
}

const QString KSyntaxHighlightingWrapper::themeName() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QString strName;
    if(d->m_highlighter) {
        strName = d->m_highlighter->theme().name();
    }
    return strName;
}

void KSyntaxHighlightingWrapper::setThemeName(const QString &themeName)
{
    Q_D(KSyntaxHighlightingWrapper);
    const auto theme = d->m_repository.theme(themeName);
    if(d->m_highlighter && theme.isValid()) {
        d->m_highlighter->setTheme(theme);
        emit themeChanged();
    }
}

const QString KSyntaxHighlightingWrapper::themeNameTranslated() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QString strName;
    if(d->m_highlighter) {
        strName = d->m_highlighter->theme().translatedName();
    }
    return strName;
}

int KSyntaxHighlightingWrapper::themeNumber() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    int themeNum = -1;
    if(d->m_highlighter) {
        QVector<KSyntaxHighlighting::Theme> themes = d->m_repository.themes();
        for(int iTheme=0; iTheme<themes.size(); iTheme++) {
            if(themes[iTheme].name() == d->m_highlighter->theme().name()) {
                themeNum = iTheme;
                break;
            }
        }
    }
    return themeNum;
}

void KSyntaxHighlightingWrapper::setThemeNumber(const int themeNumber)
{
    Q_D(KSyntaxHighlightingWrapper);
    if(d->m_highlighter) {
        QVector<KSyntaxHighlighting::Theme> themes = d->m_repository.themes();
        if(themeNumber < themes.count()) {
            d->m_highlighter->setTheme(themes[themeNumber]);
            emit themeChanged();
        }
    }
}

const QStringList KSyntaxHighlightingWrapper::themeNames() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QStringList themeNames;
    if(d->m_highlighter) {
        for(auto theme : d->m_repository.themes()) {
            themeNames.append(theme.name());
        }
    }
    return themeNames;
}

const QStringList KSyntaxHighlightingWrapper::themeNamesTranslated() const
{
    Q_D(const KSyntaxHighlightingWrapper);
    QStringList themeNames;
    if(d->m_highlighter) {
        for(auto theme : d->m_repository.themes()) {
            themeNames.append(theme.translatedName());
        }
    }
    return themeNames;
}

QT_END_NAMESPACE
