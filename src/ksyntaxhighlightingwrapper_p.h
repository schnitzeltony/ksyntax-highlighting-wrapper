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
    KSyntaxHighlightingWrapperPrivate(KSyntaxHighlightingWrapper* pPublic);
    virtual ~KSyntaxHighlightingWrapperPrivate();

    static KSyntaxHighlighting::Repository m_repository;
    KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;

    KSyntaxHighlightingWrapper *q_ptr;
};


#endif // KSYNTAX_HIGHLIGHTING_WRAPPER_P_H
