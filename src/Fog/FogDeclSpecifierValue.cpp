#include <Fog/FogIncludeAll.h>









//  
//    Contribute to the deduction of the equivalence tree for a template parameter by updating deducedType to reflect
//    the compatible application of these decl-specifiers and actualDeclSpecifiers to deducedType..Returns true with an
//    update if compatible, false if incompatible.
//  
bool FogDeclSpecifierValue::deduce_template_parameter_type(const FogDeclSpecifierValue& actualDeclSpecifiers,
                                                           FogNameRef& deducedType) const
{
 FogDeclSpecifierValue extraDeclSpecifiers;
    if (is_const() && !actualDeclSpecifiers.is_const())
        return false;
    if (actualDeclSpecifiers.is_const() && !is_const())
        extraDeclSpecifiers |= CONST_MASK;
    if (is_volatile() && !actualDeclSpecifiers.is_volatile())
        return false;
    if (actualDeclSpecifiers.is_volatile() && !is_volatile())
        extraDeclSpecifiers |= VOLATILE_MASK;
    if (extraDeclSpecifiers._mask != 0)
        deducedType.adopt(new FogDeclSpecifiedName(*deducedType, extraDeclSpecifiers));
    return true;
}

bool FogDeclSpecifierValue::emit(FogEmitContext& emitContext) const
{
 bool doneSomething = false;
 if (_mask & TEMPLATE_MASK)
  doneSomething |= emitContext.emit_space_and_text("template ");
 if (_mask & USING_MASK)
  doneSomething |= emitContext.emit_space_and_text("using ");
 if (_mask & PRIVATE_MASK)
  doneSomething |= emitContext.emit_space_and_text("private ");
 else if (_mask & PROTECTED_MASK)
  doneSomething |= emitContext.emit_space_and_text("protected ");
 else if (_mask & PUBLIC_MASK)
  doneSomething |= emitContext.emit_space_and_text("public ");
 if (_mask & AUTO_MASK)
  doneSomething |= emitContext.emit_space_and_text("auto ");
 if (_mask & EXPLICIT_MASK)
  doneSomething |= emitContext.emit_space_and_text("explicit ");
 if (_mask & EXPORT_MASK)
  doneSomething |= emitContext.emit_space_and_text("export ");
 if (_mask & EXTERN_MASK)
  doneSomething |= emitContext.emit_space_and_text("extern ");
 if (_mask & FRIEND_MASK)
  doneSomething |= emitContext.emit_space_and_text("friend ");
 if (_mask & NOT_INLINE_MASK)
  ;
 else if (_mask & INLINE_MASK)
  doneSomething |= emitContext.emit_space_and_text("inline ");
 if (_mask & MUTABLE_MASK)
  doneSomething |= emitContext.emit_space_and_text("mutable ");
 if (_mask & REGISTER_MASK)
  doneSomething |= emitContext.emit_space_and_text("register ");
 if (_mask & STATIC_MASK)
  doneSomething |= emitContext.emit_space_and_text("static ");
 if (_mask & TYPEDEF_MASK)
  doneSomething |= emitContext.emit_space_and_text("typedef ");
 if (_mask & NOT_VIRTUAL_MASK)
  ;
 else if (_mask & VIRTUAL_MASK)
  doneSomething |= emitContext.emit_space_and_text("virtual ");
 else if (_mask & PURE_MASK)
  doneSomething |= emitContext.emit_space_and_text("!virtual/pure ");
 if (_mask & NOT_CONST_MASK)
  ;
 else if (_mask & CONST_MASK)
  doneSomething |= emitContext.emit_space_and_text("const ");
 if (_mask & NOT_VOLATILE_MASK)
  ;
 else if (_mask & VOLATILE_MASK)
  doneSomething |= emitContext.emit_space_and_text("volatile ");
 return doneSomething;
}

#if 0
bool FogDeclSpecifierValue::emit_cv(FogEmitContext& emitContext) const
{
 bool doneSomething = false;
 if (is_const())
  doneSomething |= emitContext.emit_space_and_text("const");
 if (is_volatile())
  doneSomething |= emitContext.emit_space_and_text("volatile");
 return doneSomething;
}
#endif

void FogDeclSpecifierValue::merge_into(FogMergeContext& mergeContext, FogDeclSpecifierValue& declSpecifiers) const
{
 if (declSpecifiers.is_using())    //   Using merges as AND not OR
  declSpecifiers |= _mask;
 else
  declSpecifiers |= Mask(_mask & ~USING_MASK);
}

const FogMerge& FogDeclSpecifierValue::needs_merge(FogMergeContext& mergeContext, const FogDeclSpecifierValue& declSpecifiers) const
{
 if (_mask == declSpecifiers._mask)
  return FogMerge::both_valid();
 FogDeclSpecifierValue mergedDeclSpecifiers(Mask(_mask | declSpecifiers._mask));
 if (_mask == mergedDeclSpecifiers._mask)
  return FogMerge::right_invalid();
 if (declSpecifiers._mask == mergedDeclSpecifiers._mask)
  return FogMerge::left_invalid();
 if (mergedDeclSpecifiers.is_const() && mergedDeclSpecifiers.is_not_const())
  return FogMerge::incompatible_const();
 if (mergedDeclSpecifiers.is_volatile() && mergedDeclSpecifiers.is_not_volatile())
  return FogMerge::incompatible_volatile();
 if (mergedDeclSpecifiers.is_static() && mergedDeclSpecifiers.is_not_static())
  return FogMerge::incompatible_static();
 if ((mergedDeclSpecifiers.is_private() && mergedDeclSpecifiers.is_protected())
  || (mergedDeclSpecifiers.is_private() && mergedDeclSpecifiers.is_public())
  || (mergedDeclSpecifiers.is_protected() && mergedDeclSpecifiers.is_public()))
  return FogMerge::incompatible_access();
 if ((mergedDeclSpecifiers.is_virtual() || mergedDeclSpecifiers.is_pure()) && mergedDeclSpecifiers.is_not_virtual())
  return FogMerge::incompatible_virtual();
 return FogMerge::both_invalid();
}

char FogDeclSpecifierValue::print_named(std::ostream& s, char tailChar) const
{
 if (_mask & TEMPLATE_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "template ");
 if (_mask & USING_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "using ");
 if (_mask & PRIVATE_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "private ");
 if (_mask & PROTECTED_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "protected ");
 if (_mask & PUBLIC_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "public ");
 if (_mask & AUTO_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
 if (_mask & EXPLICIT_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "explicit ");
 if (_mask & EXPORT_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "export ");
 if (_mask & EXTERN_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "extern ");
 if (_mask & FRIEND_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "friend ");
//  
 if (_mask & NOT_INLINE_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "!inline ");
 else if (_mask & INLINE_MASK)
 {
  tailChar = FogStream::space_and_emit(s, tailChar, "inline ");
  if (_mask & INLINE_AS_FRIEND_MASK)
   tailChar = FogStream::space_and_emit(s, tailChar, "/friend ");
  if (_mask & INLINE_IN_IMPLEMENTATION_MASK)
   tailChar = FogStream::space_and_emit(s, tailChar, "/implementation ");
  if (_mask & INLINE_IN_INTERFACE_MASK)
   tailChar = FogStream::space_and_emit(s, tailChar, "/interface ");
 }
//  
 if (_mask & MUTABLE_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "mutable ");
 if (_mask & REGISTER_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "register ");
 if (_mask & NOT_STATIC_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "!static ");
 if (_mask & STATIC_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "static ");
 if (_mask & TYPEDEF_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "typedef ");
//  
 if (_mask & NOT_VIRTUAL_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "!virtual ");
 else if ((_mask & VIRTUAL_MASK) && (_mask & PURE_MASK))
  tailChar = FogStream::space_and_emit(s, tailChar, "virtual/pure ");
 else if (_mask & VIRTUAL_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "virtual ");
 else if (_mask & PURE_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "!virtual/pure ");
//  
 if (_mask & NOT_CONST_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "!const ");
 if (_mask & CONST_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "const ");
 if (_mask & NOT_VOLATILE_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "!volatile ");
 if (_mask & VOLATILE_MASK)
  tailChar = FogStream::space_and_emit(s, tailChar, "volatile ");
//  	if (_mask & INVALID_MASK)
//  		tailChar = FogStream::space_and_emit(s, tailChar, "invalid ");
 return tailChar;
}

bool FogDeclSpecifierValue::resolve_semantics(FogSemantics& theSemantics) const
{
 theSemantics.reset();
 switch (_mask)
 {
  case AUTO_MASK:
//  			theSemantics.set(FogSemantics::IS_AUTO_DECL_SPECIFIER_SEQ);
   break;
  case USING_MASK:
   theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
   theSemantics.set(FogSemantics::IS_USING_DECL_SPECIFIER_SEQ);
   break;
  case TYPEDEF_MASK:
   theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
   theSemantics.set(FogSemantics::IS_UNTYPED_DECL_SPECIFIER_SEQ);
   break;
  case CONST_MASK:
  case NOT_CONST_MASK:
  case VOLATILE_MASK:
  case NOT_VOLATILE_MASK:
   theSemantics.set(FogSemantics::IS_CV_QUALIFIER);
   break;
  default:
   if (((_mask & CV_MASK) != 0) && ((_mask & ~CV_MASK) == 0))
    theSemantics.set(FogSemantics::IS_CV_QUALIFIER_SEQ);
   else if (((_mask & TYPE_SPECIFIER_MASK) != 0) && ((_mask & ~TYPE_SPECIFIER_MASK) == 0))
    theSemantics.set(FogSemantics::IS_TYPE_SPECIFIER_SEQ);
   else if (((_mask & DECL_SPECIFIER_MASK) != 0) && ((_mask & ~DECL_SPECIFIER_MASK) == 0))
   {
    if ((_mask & TYPE_SPECIFIER_MASK) != 0)
    {
     theSemantics.set(FogSemantics::IS_TYPED_DECL_SPECIFIER_SEQ);
     if ((_mask & USING_MASK) != 0)
      theSemantics.set(FogSemantics::IS_USING_NAME_DECL_SPECIFIER_SEQ);
    }
    else
    {
     theSemantics.set(FogSemantics::IS_UNTYPED_DECL_SPECIFIER_SEQ);
     if ((_mask & USING_MASK) != 0)
      theSemantics.set(FogSemantics::IS_USING_DECL_SPECIFIER_SEQ);
    }
   }
   if ((_mask & USING_MASK) != 0)
   {
    if ((_mask & TYPE_SPECIFIER_MASK) != 0)
     theSemantics.set(FogSemantics::IS_USING_NAME_DECL_SPECIFIER_SEQ);
    else
     theSemantics.set(FogSemantics::IS_USING_DECL_SPECIFIER_SEQ);
   }
 }
 if ((_mask & AUTO_MASK) != 0)
 {
  if ((_mask & META_TYPE_NAME_MASK) != 0)
   theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
  else
   theSemantics.set(FogSemantics::IS_AUTO_DECL_SPECIFIER_SEQ);
 }
 else if ((_mask & META_DECL_SPECIFIER_MASK) != 0)
 {
  if ((_mask & META_TYPE_NAME_MASK) != 0)
   theSemantics.set(FogSemantics::IS_META_DECL_SPECIFIER_SEQ);
  else
   theSemantics.set(FogSemantics::IS_UNMETA_DECL_SPECIFIER_SEQ);
 }
 return true;
}

std::ostream& operator<<(std::ostream& s, const FogDeclSpecifierValue& declSpecifier)
{
 declSpecifier.print_named(s, 0);
 if (declSpecifier._mask)
  s << ' ';
 return s;
}
