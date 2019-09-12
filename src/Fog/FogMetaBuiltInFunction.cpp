#include <Fog/FogIncludeAll.h>

#include <strstream>
















 
FOGMETAFUNCTION_CLASS(FogStd_Ambiguous)
FogStd_Ambiguous::FogStd_Ambiguous(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::bool_type(), IS_ENCAPSULATED, "ambiguous")
{
    add(FogMetaType::expression_type(), IS_ENCAPSULATED, *PrimIdHandle("a_name"));  //   Formal name must be illegal
}
bool FogStd_Ambiguous::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    const FogName *aName = callContext.get_name_argument(0);
    if (!aName)
        return false;
    FogMetaSlotFinding metaSlotFinding;
    aName->find_slots_in(callContext.calling_context(), metaSlotFinding);
    returnValue.assign(FogNumber::make_logical(metaSlotFinding.is_ambiguous()));
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_Date)
FogStd_Date::FogStd_Date(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::string_type(), IS_ENCAPSULATED, "date")
{}
bool FogStd_Date::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    const char *dateString = callContext.global_scope().time().date().str();
    returnValue.assign(FogTokenType::make_narrow_string(dateString, dateString ? strlen(dateString) : 0));
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_Defined)
FogStd_Defined::FogStd_Defined(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::bool_type(), IS_ENCAPSULATED, "defined")
{
    add(FogMetaType::expression_type(), IS_ENCAPSULATED, *PrimIdHandle("a_name"));  //   Formal name must be illegal
}
bool FogStd_Defined::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    const FogName *aName = callContext.get_name_argument(0);
    if (!aName)
        return false;
    FogMetaSlotFinding metaSlotFinding;
    aName->find_slots_in(callContext.calling_context(), metaSlotFinding);
    returnValue.assign(FogNumber::make_logical(metaSlotFinding.is_found()));
    return true;
}
 
FOGMETAFUNCTION_CLASS(FogStd_Diagnostic)
FogStd_Diagnostic::FogStd_Diagnostic(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::void_type(), IS_ENCAPSULATED, "diagnostic")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("an_identifier"));
}
bool FogStd_Diagnostic::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
    {
        returnValue.reset();
        return false;
    }
    else
    {
        DIAMSGZ(anId->str());
        returnValue = FogFailure::make();
        return true;
    }
}

FOGMETAFUNCTION_CLASS(FogStd_Error)
FogStd_Error::FogStd_Error(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::void_type(), IS_ENCAPSULATED, "error")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("an_identifier"));
}
bool FogStd_Error::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
    {
        returnValue.reset();
        return false;
    }
    else
    {
        ERRMSGZ(anId->str());
        returnValue = FogFailure::make();
        return true;
    }
}

FOGMETAFUNCTION_CLASS(FogStd_File)
FogStd_File::FogStd_File(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::string_type(), IS_ENCAPSULATED, "file")
{}
bool FogStd_File::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    const FogRoot& globalScope = callContext.global_scope();
    const FogReader& theReader = globalScope.reader();
    const FogSourceFile *sourceFile = theReader.source_file();
    const char *fileName = sourceFile ? sourceFile->full_file_name().str() : 0;
    returnValue.assign(FogTokenType::make_narrow_string(fileName, fileName ? strlen(fileName) : 0));
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_GetCpp)
FogStd_GetCpp::FogStd_GetCpp(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::string_type(), IS_ENCAPSULATED, "getcpp")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("a_string"));
}
bool FogStd_GetCpp::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    returnValue.reset();
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
        return false;
    const FogReader& aReader = callContext.dynamic_token().global_scope().reader();
    const FogMacro *aMacro = aReader.is_defined(*anId);
    FogTokens *theTokens = (FogTokens *)(aMacro ? &aMacro->body() : 0);     //  .bugbug cast
    PrimIdHandle aString;
    if (theTokens && theTokens->get_string(aString, callContext))
        returnValue.assign(FogTokenType::make_narrow_string(aString.str(), aString->length()));
    else
        returnValue.assign(FogTokenType::make_narrow_string("0", 1));
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_GetEnv)
FogStd_GetEnv::FogStd_GetEnv(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::string_type(), IS_ENCAPSULATED, "getenv")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("aString"));
}
bool FogStd_GetEnv::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    returnValue.reset();
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
        return false;
    const char *envText = getenv(anId.str());
    returnValue.assign(FogTokenType::make_narrow_string(envText, envText ? strlen(envText) : 0));
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_Parse)
FogStd_Parse::FogStd_Parse(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::token_type(), IS_ENCAPSULATED, "parse")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("anIdentifier"));
}
bool FogStd_Parse::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
    {
        returnValue.reset();
        return false;
    }
    FogRoot& globalScope = callContext.global_scope();
    FogReader& theReader = globalScope.reader();
    FogTokensRef tokenList(new FogTokens, FogTokensRef::ADOPT);
    if (!theReader.parse(*tokenList, anId->str()))
    {
        ERRMSG("Failed to parse \"" << *anId << "\"");
        returnValue.reset();
        return false;
    }
    returnValue = *tokenList;
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_ParseTokens)
FogStd_ParseTokens::FogStd_ParseTokens(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::token_type(), IS_ENCAPSULATED, "parse_tokens")
{
    add(FogMetaType::token_type(), IS_ENCAPSULATED, *PrimIdHandle("anIdentifier"));
}
bool FogStd_ParseTokens::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    const FogToken *theToken = callContext.get_token_argument(0);
    if (!theToken)
    {
        returnValue.reset();
        return false;
    }
    FogRoot& globalScope = callContext.global_scope();
    FogReader& theReader = globalScope.reader();
    if (!theReader.parse_tokens(returnValue, *theToken))
    {
        ERRMSG("Failed to parse_tokens " << viz(*theToken));
        returnValue.reset();
        return false;
    }
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_Time)
FogStd_Time::FogStd_Time(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::string_type(), IS_ENCAPSULATED, "time")
{}
bool FogStd_Time::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    const char *timeString = callContext.global_scope().time().time().str();
    returnValue.assign(FogTokenType::make_narrow_string(timeString, timeString ? strlen(timeString) : 0));
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_Tokenize)
FogStd_Tokenize::FogStd_Tokenize(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::token_type(), IS_ENCAPSULATED, "tokenize")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("anIdentifier"));
}
bool FogStd_Tokenize::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
    {
        returnValue.reset();
        return false;
    }
    FogRoot& globalScope = callContext.global_scope();
    FogReader& theReader = globalScope.reader();
    FogTokensRef tokenList(new FogTokens, FogTokensRef::ADOPT);
    if (!theReader.tokenize(*tokenList, anId->str()))
    {
        ERRMSG("Failed to tokenize \"" << *anId << "\"");
        returnValue.reset();
        return false;
    }
    returnValue = *tokenList;
    return true;
}

FOGMETAFUNCTION_CLASS(FogStd_Warning)
FogStd_Warning::FogStd_Warning(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::void_type(), IS_ENCAPSULATED, "warning")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("anIdentifier"));
}
bool FogStd_Warning::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
    {
        returnValue.reset();
        return false;
    }
    else
    {
        WRNMSGZ(anId->str());
        returnValue = FogFailure::make();
        return true;
    }
}

FOGMETAFUNCTION_CLASS(FogStd_VarFn)
FogStd_VarFn::FogStd_VarFn(FogToken& staticMetaScope)
:
    Super(staticMetaScope, IS_STATIC, FogMetaType::string_type(), IS_ENCAPSULATED, "var_fn")
{
    add(FogMetaType::string_type(), IS_ENCAPSULATED, *PrimIdHandle("a_string"));
}

bool FogStd_VarFn::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const
{
    returnValue.reset();
    PrimIdHandle anId;
    if (!callContext.get_string_argument(anId, 0))
        return false;
    
    const char* orig_var = anId.str();
    int orig_var_len = strlen(orig_var);
    
    int new_count = 0;
    std::strstream str;
    bool up = true;
    for(int i = 0; i < orig_var_len; i++) {
		int chr = orig_var[i];
		
		if (chr == '_') {
			up = true;
			continue;
		}
		
		if (up)
			chr = toupper(chr);
		else
			chr = tolower(chr);
		up = false;
		
		str.put(chr);
		new_count++;
    }
    if (new_count == 0) {
        new_count++;
        str.put('_');
    }
    
    const char* result = str.str();
    returnValue.assign(FogTokenType::make_narrow_string(result, new_count));
    return true;
}





void FogMetaBuiltInFunction::install(FogScope& stdScope)
{
    stdScope.adopt_slot(new FogStd_Ambiguous(stdScope));
    stdScope.adopt_slot(new FogStd_Date(stdScope));
    stdScope.adopt_slot(new FogStd_Defined(stdScope));
    stdScope.adopt_slot(new FogStd_Diagnostic(stdScope));
    stdScope.adopt_slot(new FogStd_Error(stdScope));
    stdScope.adopt_slot(new FogStd_File(stdScope));
    stdScope.adopt_slot(new FogStd_GetCpp(stdScope));
    stdScope.adopt_slot(new FogStd_GetEnv(stdScope));
    stdScope.adopt_slot(new FogStd_Parse(stdScope));
    stdScope.adopt_slot(new FogStd_ParseTokens(stdScope));
    stdScope.adopt_slot(new FogStd_Time(stdScope));
    stdScope.adopt_slot(new FogStd_Tokenize(stdScope));
    stdScope.adopt_slot(new FogStd_Warning(stdScope));
    stdScope.adopt_slot(new FogStd_VarFn(stdScope));
}
