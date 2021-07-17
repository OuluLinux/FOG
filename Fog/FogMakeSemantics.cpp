#include <Fog/FogIncludeAll.h>


















TYPEINFO_SINGLE(FogMakeSemantics, Super)

FogMakeSemantics::FogMakeSemantics(const FogSemanticsContext& theSemantics,
                                   const FogDeclSpecifierValue& declSpecifiers, Context aContext)
	:
	_semantics(semantics(theSemantics, declSpecifiers, aContext, theSemantics.is_resolve_type_strongly()))
{}

FogMakeSemantics::Semantics FogMakeSemantics::semantics(const FogSemantics& theSemantics,
        const FogDeclSpecifierValue& declSpecifiers, Context aContext, bool isDiagnosed) {
	switch (aContext) {
	case DECLARATION:                               //   From FogExpr::compile_parsed
		if (declSpecifiers.is_friend()) {
			if (theSemantics.is_function_declaration())
				return MAKE_FRIEND_FUNCTION;
				
			if (theSemantics.is_type_declaration())
				return MAKE_FRIEND_TYPE;
				
			if (isDiagnosed)
				DIAMSG("Defaulting to VARIABLE semantics for undiagnosed " << theSemantics);
				
			return MAKE_VARIABLE;
		}
		
		if (declSpecifiers.is_friend() && theSemantics.is_function_declaration())
			return MAKE_FRIEND_FUNCTION;
			
		if (theSemantics.is_auto_ctor_definition())
			return MAKE_META_CONSTRUCTOR;
			
		if (theSemantics.is_auto_dtor_definition())
			return MAKE_META_DESTRUCTOR;
			
		if (theSemantics.is_auto_function_definition())
			return MAKE_META_FUNCTION;
			
		if (theSemantics.is_auto_variable_declaration())
			return MAKE_META_VARIABLE;
			
		if (declSpecifiers.is_auto()) {
			if (isDiagnosed)
				DIAMSG("Unexpected auto for " << theSemantics);
				
			if (theSemantics.is_meta_ctor_definition())
				return MAKE_META_CONSTRUCTOR;
				
			if (theSemantics.is_meta_dtor_definition())
				return MAKE_META_DESTRUCTOR;
				
			if (theSemantics.is_meta_function_definition())
				return MAKE_META_FUNCTION;
				
			if (theSemantics.is_meta_variable_declaration())
				return MAKE_META_VARIABLE;
				
			if (isDiagnosed)
				DIAMSG("Defaulting to META_VARIABLE semantics for undiagnosed " << theSemantics);
				
			return MAKE_META_VARIABLE;
		}
		
		if (theSemantics.is_typedef_declaration())
			return MAKE_TYPEDEF;
			
		if (theSemantics.is_function_declaration())
			return MAKE_FUNCTION;
			
		if (theSemantics.is_type_declaration())
			return MAKE_TYPE;
			
		if (theSemantics.is_init_declaration())
			return MAKE_VARIABLE;
			
		if (isDiagnosed)
			DIAMSG("Defaulting to VARIABLE semantics for undiagnosed " << theSemantics);
			
		return MAKE_VARIABLE;
		
	case META_DECLARATION:                               //   From FogExpr::compile_parsed
		if (theSemantics.is_auto_ctor_definition() || theSemantics.is_meta_ctor_definition())
			return MAKE_META_CONSTRUCTOR;
			
		if (theSemantics.is_auto_dtor_definition() || theSemantics.is_meta_dtor_definition())
			return MAKE_META_DESTRUCTOR;
			
		if (theSemantics.is_auto_function_definition() || theSemantics.is_meta_function_definition())
			return MAKE_META_FUNCTION;
			
		if (theSemantics.is_auto_variable_declaration() || theSemantics.is_meta_variable_declaration())
			return MAKE_META_VARIABLE;
			
		if (isDiagnosed)
			DIAMSG("Defaulting to META_VARIABLE semantics for undiagnosed " << theSemantics);
			
		return MAKE_META_VARIABLE;
		
	case DECLARATION_LIST:                               //   From FogListOfExpr::compile_parsed
		if (theSemantics.is_typedef_declaration())
			return MAKE_TYPEDEF;
			
		if (theSemantics.is_function_declaration())
			return MAKE_FUNCTION;
			
		return MAKE_VARIABLE;
		
	case SUBSPACE:                                      //   From FogObjectSpecifier::compile_subspace
	
		//            if (theSemantics.is_auto_ctor_definition() || theSemantics.is_meta_ctor_definition())
		//                return MAKE_META_CONSTRUCTOR;
		//            if (theSemantics.is_auto_dtor_definition() || theSemantics.is_meta_dtor_definition())
		//                return MAKE_META_DESTRUCTOR;
		if (theSemantics.is_auto_function_definition() || theSemantics.is_meta_function_definition())
			return MAKE_META_FUNCTION;
			
		if (theSemantics.is_auto_variable_declaration() || theSemantics.is_meta_variable_declaration())
			return MAKE_META_VARIABLE;
			
		if (theSemantics.is_function_body())
			return MAKE_FUNCTION_BODY;
			
		if (theSemantics.is_file_placement_specifier()
		        || theSemantics.is_file_dependency_specifier()
		        || theSemantics.is_object_declaration())
			return MAKE_SUBSPACE;
			
		if (isDiagnosed)
			DIAMSG("Defaulting to SUBSPACE semantics for undiagnosed " << theSemantics);
			
		return MAKE_SUBSPACE;
		
	case TEMPLATE_DECLARATION:                          //   FogRawTemplate::compile_parsed
		if (theSemantics.is_type_parameter())
			return MAKE_TYPE_PARAMETER;
			
		return MAKE_VALUE_PARAMETER;
	}
	
	return MAKE_INVALID;
}

bool FogMakeSemantics::make_specifier(FogBaseMakeSpecifierContext& aContext, FogRaw& aRaw) const {
	FogName* aName = aRaw.is_name();
	
	switch (_semantics) {
	case MAKE_FRIEND_FUNCTION:
	case MAKE_FUNCTION:
		if (aName)
			return aContext.adopt_specifier(new FogFunctionSpecifier(aContext, *aName));
			
		break;
		
	case MAKE_META_CONSTRUCTOR:
	case MAKE_META_DESTRUCTOR:
	case MAKE_META_FUNCTION:
		if (aName)
			return aContext.adopt_specifier(new FogMetaFunctionSpecifier(aContext, *aName));
			
		break;
		
	case MAKE_META_PARAMETER: {
		FogObjectSpecifier* metaSpecifier = 0;
		FogSpecifier* aSpecifier = aRaw.is_specifier();
		FogMetaType* metaTypeName = aSpecifier ? aSpecifier->tag().meta_type_name() : 0;
		
		if (metaTypeName) {
			metaSpecifier = new FogMetaParameterSpecifier(aContext, aSpecifier->name());
			
			if (metaSpecifier)
				metaSpecifier->set_type(*metaTypeName);
		}
		else if (aName)
			metaSpecifier = new FogMetaParameterSpecifier(aContext, *aName);
		else
			break;
			
		return aContext.adopt_specifier(metaSpecifier);
	}
	
	case MAKE_META_VARIABLE:
		if (aName)
			return aContext.adopt_specifier(new FogMetaVariableSpecifier(aContext, *aName));
			
		break;
		
	case MAKE_PARAMETER:
		if (aName)
			return aContext.adopt_specifier(new FogParameterSpecifier(aContext, *aName));
			
		break;
		
	case MAKE_TEMPLATED_PARAMETER: {
		FogSpecifier* aSpecifier = aRaw.is_specifier();
		
		if (aSpecifier)
			return aContext.adopt_specifier(new FogTemplatedParameterSpecifier(aContext, *aSpecifier));
			
		ERRMSG("BUG -- should have made templated-parameter from a specifier");
		return false;
	}
	
	case MAKE_FUNCTION_BODY: {
		FogSpecifier* aSpecifier = aRaw.is_specifier();
		
		//            FogObjectSpecifier *objectSpecifier = aSpecifier ? aSpecifier->is_object_specifier() : 0;
		if (aSpecifier) {
			aSpecifier->share();
			return aContext.adopt_specifier(aSpecifier);
		}
		
		ERRMSG("BUG -- expected specifier context for function-body element");
		return false;
	}
	
	case MAKE_SUBSPACE: {
		ERRMSG("INVESTIGATE -- use of MAKE_SUBSPACE for " << viz(aRaw));
		FogSpecifier* aSpecifier = aRaw.is_specifier();
		
		//            FogObjectSpecifier *objectSpecifier = aSpecifier ? aSpecifier->is_object_specifier() : 0;
		if (aSpecifier) {
			aSpecifier->share();
			return aContext.adopt_specifier(aSpecifier);
		}
		
		ERRMSG("BUG -- expected specifier context for subspace element");
		return false;
	}
	
	case MAKE_FRIEND_TYPE:
	case MAKE_TYPE: {
		FogSpecifier* aSpecifier = aRaw.is_specifier();
		
		if (aSpecifier) {
			aSpecifier->share();
			return aContext.adopt_specifier(aSpecifier);
		}
		
		ERRMSG("BUG -- should have made type from a specifier");
		return false;
	}
	
	case MAKE_TYPEDEF:
		return aContext.adopt_specifier(new FogTypedefSpecifier(aContext, *aName));
		
	case MAKE_TYPE_PARAMETER: {
		FogSpecifier* aSpecifier = aRaw.is_specifier();
		FogScopeSpecifier* scopeSpecifier = aSpecifier ? aSpecifier->is_scope_specifier() : 0;
		
		if (scopeSpecifier)
			return aContext.adopt_specifier(new FogTypeParameterSpecifier(aContext, *scopeSpecifier));
			
		ERRMSG("BUG -- should have made template-type-parameter from an elaborated-type-specifier");
		return false;
	}
	
	//        case MAKE_USING: return new FogUsingSpecifier(aContext, *aName);
	case MAKE_VALUE_PARAMETER:
		if (aName)
			return aContext.adopt_specifier(new FogValueParameterSpecifier(aContext, *aName));
			
		break;
		
	default:
		ERRMSG("INVESTIGATE -- using drop through to variable in FogMakeSemantics::new_specifier");
		
	case MAKE_VARIABLE:
		if (aName)
			return aContext.adopt_specifier(new FogVariableSpecifier(aContext, *aName));
			
		break;
	}
	
	ERRMSG("BUG -- cannot make-specifier from " << viz(aRaw));
	return false;
}

#define FOGMAKESEMANTICS_NAME_BIT(Name, Text) Text
std::ostream& FogMakeSemantics::print_this(std::ostream& s) const {
	static const char* const aTable[] = {
		FOGMAKESEMANTICS_DECLARATIONS(FOGMAKESEMANTICS_NAME_BIT)
	};
	return s << aTable[_semantics];
}
#undef FOGMAKESEMANTICS_NAME_BIT
