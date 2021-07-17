#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogEmitContext, Super)
TYPEINFO_SINGLE(FogBreakableEmitContext, Super)
TYPEINFO_SINGLE(FogDecoratedEmitContext, Super)
TYPEINFO_SINGLE(FogForUseAsEmitContext, Super)
TYPEINFO_SINGLE(FogIndentEmitContext, Super)
TYPEINFO_SINGLE(FogNamespaceEmitContext, Super)
TYPEINFO_SINGLE(FogNestedEmitContext, Super)
TYPEINFO_SINGLE(FogSeparatedEmitContext, Super)
TYPEINFO_SINGLE(FogUnbreakableEmitContext, Super)

TYPEINFO_SINGLE(FogStaticEmitContext, Super)

bool FogEmitContext::do_jump() const {
	return false;
}

bool FogEmitContext::emit_cv(const FogDeclSpecifierValue& declSpecifiers) {
	bool doneSomething = false;
	
	if (declSpecifiers.is_const())
		doneSomething |= emit_space_and_text("const");
		
	if (declSpecifiers.is_volatile())
		doneSomething |= emit_space_and_text("volatile");
		
	return doneSomething;
}

//
//    Emit anId within the current scope so that it refers to aToken.
//
bool FogEmitContext::emit_id(const PrimId& anId, const FogToken& aToken) {
	if (emit_type()) {  //   If type name
		FogStaticScopeContext rootContext(dynamic_scope());
		const FogEntity *anEntity = aToken.find_type_in(rootContext);
		
		if (!anEntity) {
			anEntity = aToken.find_entity_in(rootContext, FIND_SCOPE);
		}
		
		
		// fix exported code compilation problems
		if (!anEntity) {
			anEntity = dynamic_scope().find_entity_in(rootContext, FIND_SCOPE);
		}
		
		
		if (emit_full_types() && anEntity)
			return emit_identifier(anEntity->global_signature_id());
			
		const FogScope *aScope = anEntity ? anEntity->outer_scope() : 0;
		
		if (aScope) {
			emit_scope(*aScope);
			return emit_identifier(anId);
		}
		
		
		ERRMSG("Failed to locate " << viz(aToken) << " as type in " << viz(*this));
	}
	
	emit_scope();
	
	return emit_identifier(anId);
}


//
//    Emit a scope prefix to establish the current scope.
//
void FogEmitContext::emit_scope() {
	if (emit_declarator_scope())
		dynamic_scope().emit_scope(*this, name_space());
	else
		dynamic_scope().emit_scope(*this, dynamic_scope());
}

//
//    Emit a scope prefix to establish theScope.
//
void FogEmitContext::emit_scope(const FogScope& theScope) {
	if (emit_declarator_scope())
		theScope.emit_scope(*this, name_space());
	else
		theScope.emit_scope(*this, dynamic_scope());
}

FogEmitContext *FogEmitContext::is_emit_context() {
	return this;
}

const FogScope& FogEmitContext::name_space() const {
	return dynamic_scope().name_space();
}

void FogEmitContext::set_do_break() {
	ERRMSG("BUG -- should not set_do_break for " << viz(*this));
}

void FogEmitContext::set_do_continue() {
	ERRMSG("BUG -- should not set_do_continue for " << viz(*this));
}

//  ---------------------------------------------------------------------------------------------------------------------

FogBreakableEmitContext::FogBreakableEmitContext(FogEmitContext& emitContext, const FogRaw& aDeclaration)
		:
		Super(emitContext),
//      _declaration(aDeclaration),
		_do_break(false),
		_do_continue(false) {}
		
FogBreakableEmitContext::~FogBreakableEmitContext() {}

//  const FogRaw& FogBreakableEmitContext::declaration() const { return *_declaration; }

bool FogBreakableEmitContext::do_jump() const {
	return _do_break || _do_continue;
}

void FogBreakableEmitContext::set_do_break() {
	_do_break = true;
}

void FogBreakableEmitContext::set_do_continue() {
	_do_continue = true;
}

bool FogDecoratedEmitContext::do_jump() const {
	return _context.do_jump();
}

FogToken& FogDecoratedEmitContext::dynamic_token() {
	return _context.dynamic_token();
}

const FogToken& FogDecoratedEmitContext::dynamic_token() const {
	return _context.as_const().dynamic_token();
}

//  void FogDecoratedEmitContext::emit_bool(bool aNumber) { _context.emit_bool(aNumber); }
void FogDecoratedEmitContext::emit_character(const PrimId& anId, bool isWide) {
	_context.emit_character(anId, isWide);
}

FogStaticEmitContext& FogDecoratedEmitContext::emit_context() {
	return _context.emit_context();
}

bool FogDecoratedEmitContext::emit_hash_line(const FogLine& aLine) {
	return _context.emit_hash_line(aLine);
}

bool FogDecoratedEmitContext::emit_identifier(const PrimId& anId) {
	return _context.emit_identifier(anId);
}

void FogDecoratedEmitContext::emit_number(const FogNumber& aNumber) {
	_context.emit_number(aNumber);
}

void FogDecoratedEmitContext::emit_spaces(size_t numSpaces) {
	_context.emit_spaces(numSpaces);
}

bool FogDecoratedEmitContext::emit_space_and_text(const char *aString) {
	return _context.emit_space_and_text(aString);
}

void FogDecoratedEmitContext::emit_string(const PrimId& anId, bool isWide) {
	_context.emit_string(anId, isWide);
}

void FogDecoratedEmitContext::emit_text(const char *aString) {
	_context.emit_text(aString);
}

bool FogDecoratedEmitContext::find_slots(FogMetaSlotFinder& theFinder) {
	return _context.find_slots(theFinder);
}

const FogTemplateParameterSpecifier *FogDecoratedEmitContext::find_template_parameter_specifier
(const PrimId& anId) {
	return _context.find_template_parameter_specifier(anId);
}

const FogTemplateParameterSpecifier *FogDecoratedEmitContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) {
	return _context.find_template_parameter_specifier(anId);
}

const FogForUseAs& FogDecoratedEmitContext::for_use_as() const {
	return _context.for_use_as();
}

FogEmitContext::InScope FogDecoratedEmitContext::in_scope() const {
	return _context.in_scope();
}

const FogScopeContext *FogDecoratedEmitContext::meta_context() const {
	return _context.meta_context();
}

const FogScope& FogDecoratedEmitContext::name_space() const {
	return _context.name_space();
}

std::ostream& FogDecoratedEmitContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_context.print_deep(s, aDepth);
	return s;
}

std::ostream& FogDecoratedEmitContext::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_context.print_on(s, aDepth);
	return s;
}

FogScopeContext::Resolution FogDecoratedEmitContext::resolution() const {
	return _context.resolution();
}

bool FogDecoratedEmitContext::separate() {
	return _context.separate();
}

const char *FogDecoratedEmitContext::separator() const {
	return _context.separator();
}

size_t FogDecoratedEmitContext::set_depth(size_t numColumns, bool isRelative) {
	return _context.set_depth(numColumns, isRelative);
}

void FogDecoratedEmitContext::set_do_break() {
	_context.set_do_break();
}

void FogDecoratedEmitContext::set_do_continue() {
	_context.set_do_continue();
}

const char *FogDecoratedEmitContext::set_separator(const char *elementSeparator) {
	return _context.set_separator(elementSeparator);
}

const char *FogDecoratedEmitContext::set_terminator(const char *elementTerminator) {
	return _context.set_terminator(elementTerminator);
}

void FogDecoratedEmitContext::start() const {
	_context.start();
}

FogToken& FogDecoratedEmitContext::static_token() {
	return _context.static_token();
}

const FogToken& FogDecoratedEmitContext::static_token() const {
	return _context.as_const().static_token();
}

const FogScope *FogDecoratedEmitContext::stream_scope() const {
	return _context.stream_scope();
}

const FogTemplateParameterSpecifiers *FogDecoratedEmitContext::template_parameters() const {
	return _context.template_parameters();
}

bool FogDecoratedEmitContext::terminate() {
	return _context.terminate();
}

const char *FogDecoratedEmitContext::terminator() const {
	return _context.terminator();
}

//  ----------------------------------------------------------------------------------------------------------------------

FogForUseAsEmitContext::FogForUseAsEmitContext(FogEmitContext&emitContext, const FogForUseAs& forUseAs)
		:
		Super(emitContext),
		_for_use_as(forUseAs) {}
		
const FogForUseAs& FogForUseAsEmitContext::for_use_as() const {
	return _for_use_as;
}

FogIndentEmitContext::FogIndentEmitContext(FogEmitContext& emitContext, int extraIndents, FogEmitContext::Indents)
		:
		Super(emitContext),
		_saved_depth(set_depth(extraIndents * Fog::get_indent_size(), true)) {}
		
FogIndentEmitContext::FogIndentEmitContext(FogEmitContext& emitContext, int extraColumns, FogEmitContext::Columns)
		:
		Super(emitContext),
		_saved_depth(set_depth(extraColumns, true)) {}
		
FogIndentEmitContext::~FogIndentEmitContext() {
	set_depth(_saved_depth, false);
}

//  ----------------------------------------------------------------------------------------------------------------------

FogNamespaceEmitContext::FogNamespaceEmitContext(FogEmitContext& emitContext, const FogScope& nameSpace)
		:
		Super(emitContext),
		_name_space(nameSpace) {}
		
FogNamespaceEmitContext::~FogNamespaceEmitContext() {}

const FogScope& FogNamespaceEmitContext::name_space() const {
	return _name_space;
}

//  ----------------------------------------------------------------------------------------------------------------------

FogNestedEmitContext::FogNestedEmitContext(FogEmitContext& emitContext, FogToken& inToken)
		:
		Super(emitContext),
		_dynamic_token(inToken) {}
		
FogToken& FogNestedEmitContext::dynamic_token() {
	return _dynamic_token;
}

const FogToken& FogNestedEmitContext::dynamic_token() const {
	return _dynamic_token;
}

bool FogNestedEmitContext::find_slots(FogMetaSlotFinder& theFinder) {
	return _dynamic_token.find_slots(theFinder);
}

FogScopeContext::InScope FogNestedEmitContext::in_scope() const {
	return IN_BASE_SCOPE;
}

const FogScope& FogNestedEmitContext::name_space() const {
	return dynamic_scope();
}

//  ----------------------------------------------------------------------------------------------------------------------

FogSeparatedEmitContext::FogSeparatedEmitContext(FogEmitContext& emitContext,
		const char *elementSeparator, const char *elementTerminator)
		:
		Super(emitContext),
		_saved_separator(set_separator(elementSeparator)),
		_saved_terminator(set_terminator(elementTerminator)) {}
		
FogSeparatedEmitContext::~FogSeparatedEmitContext() {
	set_separator(_saved_separator);
	set_terminator(_saved_terminator);
}

FogStaticEmitContext::FogStaticEmitContext(FogStream& s, FogToken& aToken, ForUseAs forUseAs)
		:
		_static_token(aToken),
		_in_scope(IN_ANY_SCOPE),
		_s(s),
		_for_use_as(forUseAs),
		_separator(0),
		_terminator(0) {}
		
FogStaticEmitContext::~FogStaticEmitContext() {}

FogToken& FogStaticEmitContext::dynamic_token() {
	return _static_token;
}

const FogToken& FogStaticEmitContext::dynamic_token() const {
	return _static_token;
}

//  void FogStaticEmitContext::emit_bool(bool aNumber)
//  {
//      _s.emit_space_and_text(aNumber ? "true" : "false");
//  }

void FogStaticEmitContext::emit_character(const PrimId& anId, bool isWide) {
	_s.emit_space_and_text(isWide ? "L'" : "'");
	_s.write(anId.str(), anId.length());
	_s << '\'';
}

FogStaticEmitContext& FogStaticEmitContext::emit_context() {
	return *this;
}

bool FogStaticEmitContext::emit_hash_line(const FogLine& aLine) {
	if (emit_raw())
		return false;
		
	if (!aLine.is_null())
		return _s.emit_hash_line(aLine);
		
	ERRMSG("INVESTIGATE - suppression of null #line for " << viz(*this));
	
	_s.start();
	
	return false;
}

bool FogStaticEmitContext::emit_identifier(const PrimId& anId) {
//         case FogElementType::IDENTIFIER:
//              if (_ws_pending)
//                 _s.emit_space('a');
//             _s << anId.str();
//             break;
	_s.emit_space_and_text(anId.str());
	return anId.length() > 0;
}

void FogStaticEmitContext::emit_number(const FogNumber& aNumber) {
//         case FogElementType::NUMBER:
//                  if (_ws_pending)
//                     _s.emit_space('0');
//                 _s << aNumber;
//             break;
	_s.emit_space('0');
	_s << aNumber;
}

//
//   Emit numSpaces (losing the first if following on from whitespace).
//
void FogStaticEmitContext::emit_spaces(size_t numSpaces) {
	static const char spaceText[] = "                                                                         ";
	
	if (numSpaces < sizeof(spaceText))    {
		_s.emit_space_and_text(spaceText + sizeof(spaceText) - 1 - numSpaces);
		return;
	}
	
	_s.emit_space_and_text(spaceText);
	
	numSpaces -= sizeof(spaceText) - 1;
	
	while (true)    {
		if (numSpaces < sizeof(spaceText))        {
			_s.write(spaceText, numSpaces);
			return;
		}
		
		_s.write(spaceText, sizeof(spaceText) - 1);
		
		numSpaces -= sizeof(spaceText) - 1;
	}
}

bool FogStaticEmitContext::emit_space_and_text(const char *aString) {
	_s.emit_space_and_text(aString);
	return aString && *aString;
}

void FogStaticEmitContext::emit_string(const PrimId& anId, bool isWide) {
    _s.emit_space_and_text(isWide ? "L\"" : "\"");
    _s.write(anId.str(), anId.length());
    _s << '"';
}


void FogStaticEmitContext::emit_text(const char *aString)
{
    _s << aString;
}

bool FogStaticEmitContext::find_slots(FogMetaSlotFinder& theFinder) { return _static_token.find_slots(theFinder); }
const FogForUseAs& FogStaticEmitContext::for_use_as() const { return _for_use_as; }
FogEmitContext::InScope FogStaticEmitContext::in_scope() const { return _in_scope; }

bool FogStaticEmitContext::separate()
{
    bool doneSomething = emit_space_and_text(_separator);
    return terminate() || doneSomething;
}

const char *FogStaticEmitContext::separator() const { return _separator; }

size_t FogStaticEmitContext::set_depth(size_t numColumns, bool isRelative)
{
    if (isRelative)
        return _s.set_depth(_s.depth() + numColumns, FogStream::COLUMNS);
    else
        return _s.set_depth(numColumns, FogStream::COLUMNS);
}

const char *FogStaticEmitContext::set_separator(const char *elementSeparator)
{ 
    const char *oldSeparator = _separator;
    _separator = elementSeparator;
    return oldSeparator;
}

const char *FogStaticEmitContext::set_terminator(const char *elementTerminator)
{ 
    const char *oldTerminator = _terminator;
    _terminator = elementTerminator;
    return oldTerminator;
}

void FogStaticEmitContext::start() const { _s.start(); }
FogToken& FogStaticEmitContext::static_token() { return _static_token; }
const FogToken& FogStaticEmitContext::static_token() const { return _static_token; }
const FogScope *FogStaticEmitContext::stream_scope() const { return _s.emit_scope(); }
bool FogStaticEmitContext::terminate() { return emit_space_and_text(_terminator); }
const char *FogStaticEmitContext::terminator() const { return _terminator; }

FogUnbreakableEmitContext::FogUnbreakableEmitContext(FogEmitContext& emitContext, const FogRaw& aDeclaration)
:
    Super(emitContext)
//    _declaration(aDeclaration),
{}

FogUnbreakableEmitContext::~FogUnbreakableEmitContext() {}