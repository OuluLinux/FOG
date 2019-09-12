
#ifndef FOGSCOPEENUMS_HXX
#define FOGSCOPEENUMS_HXX



class FogScopeEnums
{
public:
    enum FindStrategy                           //   Category of entity eligible for return by find_entity
    {
        FIND_ENTITY,                                //   Visible type or non-type
        FIND_NAME,                                  //   Visible non-type
        FIND_SCOPE,                                 //   Visible scope
        FIND_SIGNATURE,                             //   Visible function indexed by signature
        FIND_TYPE                                   //   Visible type
    };

    enum HasContext                             //   Whether made entity gets initial context
    {
        NO_CONTEXT,                                 //   Nothing to add to new entity
        HAS_CONTEXT                                 //   Context to be added to new entity
    };

    enum InScope                                //   Depth of search for a name.
    {
        IN_ANY_SCOPE,                               //   In this scope or its bases or its enclosing scopes.
        IN_BASE_SCOPE,                              //   Must be in this scope or its bases.
        IN_THIS_SCOPE,                              //   Must be in this scope.
        IN_ANY_NOT_THIS_SCOPE,                      //   Must be visible from this scope (e.g a base class)
        IN_BASE_NOT_THIS_SCOPE                      //   Must be visible above this scope (e.g an inherited member)
    };

    enum IsExportTemplate                       //   Presence of export (template) keyword
    {
        NON_EXPORT_TEMPLATE,
        IS_EXPORT_TEMPLATE
    };

    enum IsUsingTemplate                        //   Presence of using (template) keyword
    {
        NON_USING_TEMPLATE,
        IS_USING_TEMPLATE
    };

    enum IsArrayDelete                          //   Whether [] accompanies delete
    {
        SCALAR_DELETE,                              //   delete
        ARRAY_DELETE                                //   delete[]
    };

    enum IsExposed
    {
        IS_ENCAPSULATED,                            //   encapsulated list of object meta-type
        IS_EXPOSED                                  //   exposed list of object meta-type
    };

    enum IsGlobalDelete                         //   Whether :: accompanies delete
    {
        SCOPED_DELETE,                              //   delete
        GLOBAL_DELETE                               //   ::delete
    };

    enum IsList
    {
        IS_NOT_LIST,                                //   object is not a list
        IS_ABUT_LIST,                               //   object is a list of abutted elements
        IS_COMMA_LIST                               //   object is a list of comma separated elements
    };

    enum IsReference                            //   Whether FogMakeTemplateContext is referencing or defining
    {
        IS_DEFINITION,                              //   Template definition
        IS_REFERENCE                                //   Template reference
    };

    enum IsStatic
    {
        IS_NOT_STATIC,                              //   non-static meta-object
        IS_STATIC                                   //   static meta-object
    };

    enum IsTemplate
    {
        IS_DEFAULT,                                 //   implicit behaviour
        IS_TEMPLATE                                 //   explicit template qualifier
    };

 enum NameSort
 {
  INVALID_NAME,
  CONSTRUCTOR_NAME,
  DESTRUCTOR_NAME,
  OPERATOR_NAME,
  CONVERSION_NAME,
  NAME_NAME
 };

    enum ResolveAlias                           //   Nature of typedef usage.
    {
        KEEP_ALIAS,                                 //   Reference, resolved to original (typedef'd) name
        RESOLVE_ALIAS                               //   Reference, resolved to actual type.
    };
};
#endif

