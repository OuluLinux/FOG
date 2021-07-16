#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogTag, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogTag)
PRIMREFS_IMPL(FogTag)
TMPL_HACK_FIX_DO(FogTag)

FogTag *FogTag_Flyweights::_flyweights[FogTagEnums::ARRAY_SIZE] = { 0 };
bool FogTag_Flyweights::_initialised = initialise();
    
bool FogTag_Flyweights::initialise()
{
    static const FogTag_Initializer flyweightInits[] =
    {
        { "built_in", FogTokenType::init(), FogTokenType::init(), FogTokenType::init(), &FogMetaType::built_in_type,
                &FogMetaType::built_in_type_specifier_type, FogTag::BUILT_IN },
        { "namespace/file", FogTokenType::init(), FogTokenType::init(), FogTokenType::init(), &FogMetaType::filespace_type,
                &FogMetaType::filespace_specifier_type, FogTag::FILESPACE },
        { "linkage", FogTokenType::init(), FogTokenType::init(), FogTokenType::init(), &FogMetaType::linkage_type,
                &FogMetaType::linkage_specification_type, FogTag::LINKAGE },
        { "auto", FogTokenType::init(), FogTokenType::init(), FogTokenType::init(), &FogMetaType::meta_class_type,
                &FogMetaType::meta_class_specifier_type, FogTag::META }, //  .bugbug cf AUTO ?
        { "scope", FogTokenType::init(), FogTokenType::init(), FogTokenType::init(), &FogMetaType::scope_type,
                &FogMetaType::class_specifier_type, FogTag::SCOPE },
        { "/*INVALID-access*/", FogTokenType::init(), FogTokenType::init(), FogTokenType::init(), &FogMetaType::mutable_null,
                &FogMetaType::mutable_null, FogTag::INVALID },
        { 0, FogTokenType::init(), FogTokenType::init(), FogTokenType::init(), &FogMetaType::mutable_null,
                &FogMetaType::mutable_null, FogTag::INVALID }
    };
    static FogTagRef flyWeights[FogTagEnums::ARRAY_SIZE];
    for (const FogTag_Initializer *p = flyweightInits; p->_name; ++p)
    {
        flyWeights[p->_tag_enum].adopt(new FogTag(*p));
        _flyweights[p->_tag_enum] = flyWeights[p->_tag_enum].pointer();
    }
    _flyweights[FogTag::CLASS] = &FogTokenType::get_tag(FogTokenType::CLASS);
    _flyweights[FogTag::ENUM] = &FogTokenType::get_tag(FogTokenType::ENUM);
    _flyweights[FogTag::NAMESPACE] = &FogTokenType::get_tag(FogTokenType::NAMESPACE);
    _flyweights[FogTag::STRUCT] = &FogTokenType::get_tag(FogTokenType::STRUCT);
    _flyweights[FogTag::TYPENAME] = &FogTokenType::get_tag(FogTokenType::TYPENAME);
    _flyweights[FogTag::UNION] = &FogTokenType::get_tag(FogTokenType::UNION);
    return true;
}

FogTag::FogTag(const FogTag_Initializer& inIt)
:
    Super(inIt._name, inIt._pp_enum, inIt._enum),
    _tag(inIt._tag_enum),
    _entity_meta_type(inIt._entity_meta_type ? inIt._entity_meta_type() : FogMetaType::mutable_null()),
    _specifier_meta_type(inIt._specifier_meta_type ? inIt._specifier_meta_type() : FogMetaType::mutable_null())
{}

FogTag::FogTag()
:
    Super(0, FogTokenType::init(), FogTokenType::init()),
    _tag(INVALID),
    _entity_meta_type(FogMetaType::immutable_null()),
    _specifier_meta_type(FogMetaType::immutable_null())
{}

const FogTag& FogTag::merge_tag(const FogTag& aTag) const
{
    if (!is_determined() || (_tag == aTag._tag))
        return aTag;
    if (!aTag.is_determined())
        return *this;
    if ((_tag == CLASS) && (aTag._tag == STRUCT))
        return class_tag();
    if ((_tag == STRUCT) && (aTag._tag == CLASS))
        return class_tag();
    ERRMSG("BUG - should not merge incompatible tags \"" << *this << "\" and \"" << aTag << '\"');
    return invalid_tag();
}

const FogMetaType& FogTag::meta_type() const
{
    switch (_tag.value())
    {
        case CLASS:
        case STRUCT:
        case UNION:
            return FogMetaType::class_key_type();
        case TYPENAME:
        case ENUM:
        case NAMESPACE:
//        case TYPEDEF:
            return FogMetaType::meta_type_type();
        default:
            return Super::meta_type();
    }
}

FogMetaType *FogTag::meta_type_name() const
{
    switch (_tag.value())
    {
        case CLASS: return &FogMetaType::class_type();
        case ENUM: return &FogMetaType::enum_type();
        case LINKAGE: return &FogMetaType::linkage_type();
        case NAMESPACE:
			return &FogMetaType::namespace_type();
        case SCOPE: return &FogMetaType::scope_type();
//          case TYPEDEF: return &FogMetaType::class_type();
        case TYPENAME: return &FogMetaType::typename_type();
        default: return 0;
    }
}

const FogMerge& FogTag::needs_merge_tag(const FogTag& aTag) const
{
    if (_tag == aTag._tag)
        return FogMerge::both_valid();
    if (!aTag.is_determined())
        return FogMerge::right_invalid();
    if (!is_determined())
        return FogMerge::left_invalid();
    if ((_tag == CLASS) && (aTag._tag == STRUCT))
        return FogMerge::right_invalid();
    if ((_tag == STRUCT) && (aTag._tag == CLASS))
        return FogMerge::left_invalid();
    return FogMerge::set_incompatible(FogMerge::incompatible_tag());
}

bool FogTag::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    theSemantics.reset();
    switch (_tag.value())
    {
        case CLASS:
            theSemantics.set(FogSemantics::IS_CLASS_TYPE_PARAMETER);
            theSemantics.set(FogSemantics::IS_CLASS_KEY);
            theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
            break;
        case TYPENAME:
            theSemantics.set(FogSemantics::IS_TYPENAME_TYPE_PARAMETER);
            theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
            break;
        case ENUM:
        case NAMESPACE:
            theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
            break;
//        case TYPEDEF:
//            theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
//            theSemantics.set(FogSemantics::IS_UNTYPED_DECL_SPECIFIER_SEQ);
//            break;
        case STRUCT:
        case UNION:
            theSemantics.set(FogSemantics::IS_CLASS_KEY);
            theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
            break;
    }
    return true;
}
