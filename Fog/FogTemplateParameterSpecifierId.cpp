#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogTemplateParameterSpecifierId, Super)
FOGTOKEN_ACTUAL_IMPL(FogTemplateParameterSpecifierId)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogTemplateParameterSpecifierId)
PRIMREFS_IMPL(FogTemplateParameterSpecifierId)
PRIMMAPOFREFS_IMPL(FogTemplateParameterSpecifierId)
TMPL_HACK_FIX_DO(FogTemplateParameterSpecifierId)

FogTemplateParameterSpecifierId::FogTemplateParameterSpecifierId()
	:
	_form(VALUE)
{}

FogTemplateParameterSpecifierId::FogTemplateParameterSpecifierId(const PrimId& anId, Form aForm)
	:
	Super(anId),
	_form(aForm)
{}

void FogTemplateParameterSpecifierId::create_usage(FogUsageContext& usageContext) const { return; }

const FogTemplateParameterSpecifierId* FogTemplateParameterSpecifierId::deduce_template_parameter_type
(FogName& actualTypeName, FogNameRef& deducedType) const {
	deducedType = actualTypeName;
	return this;
}

bool FogTemplateParameterSpecifierId::emit(FogEmitContext& emitContext) const
{ return emitContext.emit_identifier(id()); }

void FogTemplateParameterSpecifierId::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const {
	//   Only invoked as get_type_in from ObjectSpecifier::create_usage.
	//      ERRMSG("INVESTIGATE - find_entities_in " << viz(*this) << " in " << viz(inScope));
	//      const FogTemplateParameterSpecifier *tpSpecifier = inScope.find_template_parameter_specifier(*this);
	//      FogTemplateParameter *tParameter = tpSpecifier ? inScope.dynamic_scope().find_template_parameter(*tpSpecifier) : 0;
	switch (theFinding.strategy()) {
	//          case FIND_ENTITY:
	//              if (tParameter && tParameter->is_entity())
	//                  theFinding.add(*tParameter);
	//              else
	//              {
	//                  ERRMSG("INVESTIGATE - failed to locate " << viz(*this) << " as an entity in " << viz(inScope));
	//                  theFinding.add(FogEntity::mutable_null());
	//              }
	//              break;
	//          case FIND_NAME:
	//              if (_form == VALUE)
	//              {
	//                  if (tParameter && tParameter->is_entity())
	//                      theFinding.add(*tParameter);
	//                  else
	//                  {
	//                      ERRMSG("INVESTIGATE - failed to locate " << viz(*this) << " as an entity in " << viz(inScope));
	//                      theFinding.add(FogEntity::mutable_null());
	//                  }
	//              }
	//              break;
	case FIND_TYPE:
		if (_form != VALUE) {
			//                  if (tParameter && tParameter->is_entity())
			//                      theFinding.add(*tParameter);
			//                  else
			//                  {
			//                      ERRMSG("INVESTIGATE - failed to locate " << viz(*this) << " as an entity in " << viz(inScope));
			theFinding.add(FogTypeName::mutable_null());
			//                  }
		}
		
		break;
		
	default:
		ERRMSG("BUG - should not FogTemplateParameterSpecifierId::find_entities_in for unknown find strategy.");
		break;
	}
}

FogTemplateParameterSpecifierId::Form FogTemplateParameterSpecifierId::form(const FogTemplateParameterSpecifier& aTps) {
	if (aTps.is_type_parameter_specifier())
		return TYPE;
	else if (aTps.is_type_parameter_specifier())
		return VALUE;
	else
		return TEMPLATED;
}

FogTemplateParameterSpecifierId* FogTemplateParameterSpecifierId::is_template_parameter_specifier_id() { return this; }

FogTemplateParameterSpecifierId& FogTemplateParameterSpecifierId::make(const PrimId& anId,
        const FogTemplateParameterSpecifier& templateParameterSpecifier) {
	const Form theForm = form(templateParameterSpecifier);
	static FogTemplateParameterSpecifierIdMapOfRef identifierMaps[3];
	FogTemplateParameterSpecifierIdMapOfRef& identifierMap = identifierMaps[theForm];
	FogTemplateParameterSpecifierId* anIdentifier = identifierMap.find(anId);
	
	if (!anIdentifier) {
		anIdentifier = new FogTemplateParameterSpecifierId(anId, theForm);
		
		if (anIdentifier)
			identifierMap.adopt(anIdentifier);
	}
	
	if (anIdentifier)
		return *anIdentifier;
	else
		return FogTemplateParameterSpecifierId::mutable_null();
}

FogEntity* FogTemplateParameterSpecifierId::make_type_entity(FogMakeEntityContext& makeEntityContext) const {
	return &FogEntity::mutable_null();      //   satisfy required successful status return.
}

const FogMetaType& FogTemplateParameterSpecifierId::meta_type() const {
	switch (_form) {
	case VALUE:
		return FogMetaType::value_parameter_specifier_type();
		
	case TYPE:
		return FogMetaType::type_parameter_specifier_type();
		
	case TEMPLATED:
		return FogMetaType::templated_parameter_specifier_type();
	}
	
	return FogMetaType::void_type();         //   Visual C++ wants further coverage!
}

std::ostream& FogTemplateParameterSpecifierId::print_viz(std::ostream& s) const {
	switch (_form) {
	case VALUE:
		s << "\"value-parameter-name: " << id() << '\"'; break;
		
	case TYPE:
		s << "\"type-parameter-name: " << id() << '\"'; break;
		
	case TEMPLATED:
		s << "\"templated-parameter-name: " << id() << '\"'; break;
	}
	
	return s;
}

bool FogTemplateParameterSpecifierId::resolve_semantics(FogSemanticsContext& theSemantics) const {
	switch (_form) {
	case VALUE:
		theSemantics.assign(FogSemantics::IS_TEMPLATE_VALUE_NAME); break;
		
	case TYPE:
		theSemantics.assign(FogSemantics::IS_TEMPLATE_TYPE_NAME); break;
		
	case TEMPLATED:
		theSemantics.assign(FogSemantics::IS_TEMPLATE_TYPE_NAME); break;
	}
	
	return true;
}
