#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogAsmSpecifier, Super)
//  PRIMREF_NULL_CLAS(FogAsmSpecifier)
//      FOGTOKEN_NULL_LEAF
//  };
//  PRIMREF_DERIVED_NULL_IMPL(FogAsmSpecifier)
//  PRIMREFS_IMPL(FogAsmSpecifier)
FOGTOKEN_LEAF_IMPL(FogAsmSpecifier)
//TMPL_HACK_FIX_DO(FogAsmSpecifier)

FogAsmSpecifier::FogAsmSpecifier() {}
FogAsmSpecifier::~FogAsmSpecifier() {}

FogAsmSpecifier::FogAsmSpecifier(FogString& asmText)
	:
	_asm_text(asmText)
{}

bool FogAsmSpecifier::compile_parsed(FogParseContext& parseContext) {
	ERRMSG("ROUNDTUIT -- missing code for FogAsmSpecifier::compile_parsed.");
	//      parseContext.dynamic_entity().set_implementation_file(file_id().make(parseContext));
	return true;
}

bool FogAsmSpecifier::emit(FogEmitContext& emitContext) const {
	emitContext.emit_hash_line(line());
	emitContext.emit_space_and_text("asm (");
	_asm_text->emit(emitContext);
	emitContext.emit_space_and_text(");");
	return true;
}

FogEntity* FogAsmSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const {
	ERRMSG("ROUNDTUIT -- missing code for FogAsmSpecifier::make_name_entity.");
	return 0;
}

FogEntity* FogAsmSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }

const FogMetaType& FogAsmSpecifier::meta_type() const { return FogMetaType::asm_definition_type(); }

char FogAsmSpecifier::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, "asm (");
	tailChar = _asm_text->print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, ");");
	return tailChar;
}

std::ostream& FogAsmSpecifier::print_viz(std::ostream& s) const {
	s << "\"asm-definition: ";
	print_this(s);
	return s << '\"';
}

bool FogAsmSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.assign(FogSemantics::IS_ASM_DEFINITION);
	return true;
}
