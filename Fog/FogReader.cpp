#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogReader, Super)

FogReader::FogReader(FogRoot& aRoot)
:
    _global(aRoot),
    _utility_mode(FogUtility::emit_utility()),
    _source_file(0),
    _scope(&aRoot),
    _depth(0)
{
    static FogTokensRef zeroTokens(new FogTokens, FogTokensRef::ADOPT);
    if (zeroTokens && !zeroTokens->tally())
        zeroTokens->add(FogTokenType::zero());
    _macros.adopt(new FogMacro(*PrimIdHandle("__cplusplus"), 0, *zeroTokens));
    _macros.adopt(new FogMacro(*PrimIdHandle("__STDC__"), 0, *zeroTokens));
    for (PrimTextIterator p(Fog::define_strings()); p; p++)
    {
        const char *pStart = p->str();
        const char *pEqual = strchr(pStart, '=');
        PrimOstrstream s;
        s << "#define ";
        if (pEqual)
        {
            s.write(pStart, pEqual - pStart);
            s << ' ';
            s << (pEqual+1);
        }
        else
            s << pStart;
        FogTokensRef tokenList(new FogTokens, FogTokensRef::ADOPT);
        if (!tokenize(*tokenList, s.str()))
            ERRMSG("Failed to tokenize \"" << s.str() << "\" from \"-D" << pStart << "\"");;
    }
}

FogReader::~FogReader() {}

void FogReader::define(const PrimId& macroId, const PrimIdList *formalList, FogTokens& someTokens)
{
    const FogMacro *oldMacro = _macros.find(macroId);
    if (!oldMacro)
        _macros.adopt(new FogMacro(macroId, formalList, someTokens));
    else if (!oldMacro->is_compatible_with(formalList, someTokens))
        ERRMSGZ("Illegal macro redefinition.");
}

const FogSourceFile *FogReader::find_source_file(const PrimId& fileName,
        const FogSourceFileType& sourceType)
{
    const FogSourceFile *sourceFile = global().file_manager().find_source_file(fileName, sourceType);
    if (sourceFile)
        add_source(sourceFile->unique_id());
    return sourceFile;
}

const FogSourceFile *FogReader::make_source_file(const PrimId& fileName)
{
    const FogSourceFile *foundFile = find_source_file(fileName, FogSourceFileType::invalid());
    if (!foundFile)
        foundFile = global().file_manager().make_source_file(fileName, FogSourceFileType::invalid());
    return foundFile;
}

bool FogReader::parse(FogTokens& returnTokens, const char *aString)
{
    WRNMSG("Experimental implementation of FogReader::parse()");
    std::istrstream s(aString);
    FogLexerCpp aLexer(*this, s, FogSourceFile::immutable_null());
    FogTokenContext tokenContext;
    while (aLexer.get(tokenContext))
    {
        FogToken& theToken = tokenContext.token();
        if (!theToken.is_line())
            returnTokens.add(theToken);
    }
    return true;
}

bool FogReader::parse_tokens(FogTokenRef& returnToken, const FogToken& aToken)
{
    returnToken.reset();
    ERRMSG("BUG -- no implementation of FogReader::parse_tokens()");
    return false;
}

std::ostream& FogReader::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_sources.tally())
    {
        s << indent(aDepth) << "sources\n";
        _sources.print_depth(s, aDepth+1);
    }
    if (_macros.tally())
        _macros.print_depth(s, aDepth);
    return s;
}

std::ostream& FogReader::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_sources.tally())
    {
        s << indent(aDepth) << "sources\n";
        _sources.print_members(s, aDepth+1);
    }
    if (_macros.tally())
        _macros.print_members(s, aDepth);
    return s;
}

void FogReader::print_token(const char *aState, const FogToken *aValue) const
{
    PrimOstrstream s;
//    if (yy_debug())
//        s << "\t\t\t\t\t\t";
    s << indent(_depth);
    s << (aState ? aState : "<ZERO>");
    if (aValue)
    {
        s << " >> ";
        aValue->print_diagnostic(s, 0);
    }
    DIAMSGZ(s.str());
}

void FogReader::set_new_scope(FogScope& aScope)
{
    CONDMSG(Fog::debug_scope(), 
        "Scope of " << viz(_global) << " changing from " << viz(*_scope) << " to " << viz(aScope));
    _scope = &aScope;
    aScope.set_braced();                    //   Terminates consideration as a forward declaration.
    if (!aScope.is_name_space())            //   Don't mess root scope around.
    {
        if (_source_file && _source_file->source_type().is_hash())
        {
            FogTargetFile *targetFile = _global.file_manager().make_target_file(*_source_file);
            if (targetFile)
                aScope.set_interface_file(targetFile);
        }
        aScope.set_max_utility(utility_mode());
    }
}

void FogReader::set_new_utility_mode(const FogUtility& utilityMode)
{
    CONDMSG(Fog::debug_utility(), 
        "Utility mode of " << viz(_global) << " changing from " << utility_mode().str() << " to " << utilityMode.str());
    _utility_mode = utilityMode;
}

bool FogReader::tokenize(FogTokens& returnTokens, const char *aString)
{
    std::istrstream s(aString);
    FogLexerCpp aLexer(*this, s, FogSourceFile::immutable_null());
    FogTokenContext tokenContext;
    while (aLexer.get(tokenContext))
    {
        FogToken& theToken = tokenContext.token();
        if (!theToken.is_line())
            returnTokens.add(theToken);
    }
    return true;
}

void FogReader::undefine(const PrimId& anId)
{
    _macros.remove(anId);
    /*   No error message required if not defined. */
}
