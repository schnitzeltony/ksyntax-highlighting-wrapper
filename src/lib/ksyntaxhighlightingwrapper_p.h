#ifndef KSYNTAX_HIGHLIGHTING_WRAPPER_P_H
#define KSYNTAX_HIGHLIGHTING_WRAPPER_P_H

#include "ksyntaxhighlightingwrapper.h"
#include <repository.h>
#include <syntaxhighlighter.h>
#include <definition.h>
#include <theme.h>

class KSyntaxHighlightingWrapperPrivate
{
    Q_DECLARE_PUBLIC(KSyntaxHighlightingWrapper)
public:
    virtual ~KSyntaxHighlightingWrapperPrivate();
private:
    KSyntaxHighlightingWrapperPrivate(KSyntaxHighlightingWrapper* pPublic);

    bool setTextDocument(QTextDocument *textDocument);
    QTextDocument *textDocument() const;
    bool setQmlTextDocument(QQuickTextDocument *qmlTextDocument);
    QQuickTextDocument *qmlTextDocument() const;

    bool setDefinition(KSyntaxHighlighting::Definition def);
    KSyntaxHighlighting::Definition definition();
    bool setDefinitionForFileName(const QString &fileName);
    bool setDefinitionForMimeType(const QString &mimeType);
    const QString definitionName();
    bool setDefinitionName(const QString& definitionName);
    const QStringList definitionNames() const;
    int definitionNumber();
    bool setDefinitionNumber(const int definitionNumber);

    KSyntaxHighlighting::Theme theme();
    bool setTheme(KSyntaxHighlighting::Theme theme);
    const QString themeName();
    bool setThemeName(const QString& themeName);
    int themeNumber();
    bool setThemeNumber(const int themeNumber);
    const QStringList themeNames() const;
    const QStringList themeNamesTranslated() const;

private:
    KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;

    // We have to keep for getter
    QQuickTextDocument *m_quickTextDocument;

    // m_highlighter has them but m_highlighter can be nullptr
    // so keep these locally. We keep default from KSyntaxHighlighting:
    // without beeing set they are invalid. Maybe we have to change that..
    KSyntaxHighlighting::Definition m_currentDefinition;
    KSyntaxHighlighting::Theme m_currentTheme;  // default is invalid see theme.h

    KSyntaxHighlightingWrapper *q_ptr;
};


#endif // KSYNTAX_HIGHLIGHTING_WRAPPER_P_H
