//
// 	  Title:			YAC action routine macros
//
// 	  File Name:		FogParserMacros.h
//
// 	  Author:			E.D.Willink
// END
//
#ifndef FOGPARSERMACROS_H
#define FOGPARSERMACROS_H

#define YACC_ABSTRACT_ARRAY_EXPRESSION(a) make_abstract_array_expression(a)
#define YACC_ABSTRACT_FUNCTION_EXPRESSION(a) make_abstract_function_expression(a)
#define YACC_ACCESSIBILITY_SPECIFIER(a) make_accessibility_specifier(a)
#define YACC_ACCESS_BASE_SPECIFIER(a,b) make_access_base_specifier(a,b)
#define YACC_ACCESS_SPECIFIER_ID(a) make_access_specifier_id(a)
#define YACC_ADD_EXPRESSION(a,b) make_add_expression(a, b)
#define YACC_AND_EXPRESSION(a,b) make_and_expression(a,b)
#define YACC_ARRAY_EXPRESSION(a,b) make_array_expression(a,b)
#define YACC_ARROW_EXPRESSION(a,b) make_arrow_expression(a,b)
#define YACC_ARROW_STAR_EXPRESSION(a,b) make_arrow_star_expression(a,b)
#define YACC_ASM_DEFINITION(a) make_asm_definition(a)
#define YACC_ASSIGNMENT_EXPRESSION(a,b,c) make_assignment_expression(a,b,c)
#define YACC_BASE_SPECIFIER(a) make_base_specifier(a)
#define YACC_BASE_SPECIFIERS(a,b) make_base_specifiers(a,b)
#define YACC_BIT_FIELD_EXPRESSION(a,b) make_bit_field_expression(a,b)
#define YACC_BREAK_STATEMENT() make_break_statement()
#define YACC_BUILT_IN_ID(a) make_built_in_id(a)
#define YACC_BUILT_IN_ID_ID(a) make_built_in_id_id(a)
#define YACC_BUILT_IN_IDS(a,b) make_built_in_ids(a,b)
#define YACC_BUILT_IN_NAME(a,b) make_built_in_name(a,b)
#define YACC_CALL_EXPRESSION(a,b) make_call_expression(a,b)
#define YACC_CASE_STATEMENT(a,b) make_case_statement(a,b)
#define YACC_CAST_EXPRESSION(a,b) make_cast_expression(a,b)
#define YACC_CHARACTER_LITERAL_EXPRESSION(a) make_character_literal_expression(a)
#define YACC_CLASS_MEMBERS(a,b) make_class_members(a,b)
#define YACC_CLASS_SPECIFIER_ID(a,b,c) make_class_specifier_id(a,b,c)
#define YACC_CLASS_TYPE_PARAMETER(a) make_class_type_parameter(a)
#define YACC_COMPILE_DECLARATION(a,b) compile_declaration(a,b)
#define YACC_COMPILE_STATEMENT(a) compile_statement(a)
#define YACC_COMPLEMENT_EXPRESSION(a) make_complement_expression(a)
#define YACC_COMPOUND_STATEMENT(a) make_compound_statement(a)
#define YACC_CONDITION(a) make_condition(a)
#define YACC_CONDITIONAL_EXPRESSION(a,b,c) make_conditional_expression(a,b,c)
#define YACC_CONST_CAST_EXPRESSION(a,b) make_const_cast_expression(a,b)
#define YACC_CONTINUE_STATEMENT() make_continue_statement()
#define YACC_CONVERSION_FUNCTION_ID(a) make_conversion_function_id(a)
#define YACC_CTOR_DEFINITION(a,b) make_ctor_definition(a,b)
#define YACC_CTOR_EXPRESSION(a) make_ctor_expression(a)
#define YACC_CTOR_FUNCTION_BLOCK(a,b) make_ctor_function_block(a,b)
#define YACC_CV_DECLARATOR(a,b) make_cv_declarator(a,b)
#define YACC_CV_DECL_SPECIFIER(a) make_cv_decl_specifier(a)
#define YACC_CV_QUALIFIERS(a,b) make_cv_qualifiers(a,b)
#define YACC_DECLARATIONS(a,b) make_declarations(a,b)
#define YACC_DECLARATION_STATEMENT(a) make_declaration_statement(a)
#define YACC_DECL_SPECIFIER_DECLARATION(a,b) make_decl_specifier_declaration(a,b)
#define YACC_DECL_SPECIFIER_EXPRESSION(a,b) make_decl_specifier_expression(a,b)
#define YACC_DECL_SPECIFIER_NAME(a,b) make_decl_specifier_name(a,b)
#define YACC_DECL_SPECIFIER_PARAMETER(a,b) make_decl_specifier_parameter(a,b)
#define YACC_DECL_SPECIFIERS(a,b) make_decl_specifiers(a,b)
#define YACC_DECL_SPECIFIER_TREE_ARGUMENTS(a,b) make_decl_specifier_tree_arguments(a,b)
#define YACC_DEFAULT_STATEMENT(a) make_default_statement(a)
#define YACC_DELETE_EXPRESSION(a) make_delete_expression(a)
#define YACC_DERIVED_CLAUSE(a,b) make_derived_clause(a,b)
#define YACC_DESTRUCTOR_ID(a) make_destructor_id(a)
#define YACC_DIVIDE_EXPRESSION(a,b) make_divide_expression(a,b)
#define YACC_DOT_EXPRESSION(a,b) make_dot_expression(a,b)
#define YACC_DOT_STAR_EXPRESSION(a,b) make_dot_star_expression(a,b)
#define YACC_DO_WHILE_STATEMENT(a,b) make_do_while_statement(a,b)
#define YACC_DYNAMIC_CAST_EXPRESSION(a,b) make_dynamic_cast_expression(a,b)
#define YACC_ELABORATED_TYPE_SPECIFIER(a,b) make_elaborated_type_specifier(a,b)
#define YACC_ELLIPSIS_EXPRESSION() make_ellipsis_expression()
#define YACC_ENUMERATOR(a,b) make_enumerator(a,b)
#define YACC_ENUMERATORS(a,b) make_enumerators(a,b)
#define YACC_ENUM_SPECIFIER_ID(a,b) make_enum_specifier_id(a,b)
#define YACC_ENUM_TREE_ID(a) make_enum_tree_id(a)
#define YACC_EPSILON() make_epsilon()
#define YACC_EQUAL_EXPRESSION(a,b) make_equal_expression(a,b)
#define YACC_EXCEPTION_DECLARATION(a) make_exception_declaration(a)
#define YACC_EXCEPTION_SPECIFICATION(a) make_exception_specification(a)
#define YACC_EXCLUSIVE_OR_EXPRESSION(a,b) make_exclusive_or_expression(a,b)
#define YACC_EXPLICIT_SPECIALIZATION(a) make_explicit_specialization(a)
#define YACC_EXPORT_ALL_DECLARATION(a) make_export_all_declaration(a)
#define YACC_EXPORT_IMPLEMENTATION_DECLARATION(a) make_export_implementation_declaration(a)
#define YACC_EXPORT_INTERFACE_DECLARATION(a) make_export_interface_declaration(a)
#define YACC_EXPORT_NOIMPLEMENTATION_DECLARATION() make_export_noimplementation_declaration()
#define YACC_EXPORT_UTILITY_DECLARATION(a) make_export_utility_declaration(a)
#define YACC_EXPRESSION(a) make_expression(a)
#define YACC_EXPRESSIONS(a,b) make_expressions(a,b)
#define YACC_EXPRESSION_PARAMETER(a) make_expression_parameter(a)
#define YACC_FALSE_EXPRESSION() make_false_expression()
#define YACC_FILESPACE_DECLARATION(a) make_filespace_declaration(a)
#define YACC_FILESPACE_SPECIFIER(a,b) make_filespace_specifier(a,b)
#define YACC_FILE_ID(a) make_file_id(a)
#define YACC_FILE_ID_IMPLEMENTATION(a) make_file_id_implementation(a)
#define YACC_FILE_ID_INTERFACE(a) make_file_id_interface(a)
#define YACC_FILE_IDS(a,b) make_file_ids(a,b)
#define YACC_FILE_NAME(a) make_file_name(a)
#define YACC_FILE_NAME_GUARD(a,b) make_file_name_guard(a,b)
#define YACC_FILE_NAME_IMPLEMENTATION(a) make_file_name_implementation(a)
#define YACC_FILE_NAME_INTERFACE(a) make_file_name_interface(a)
#define YACC_FILE_NAME_NOGUARD(a) make_file_name_noguard(a)
#define YACC_FILE_NAME_PATH(a,b) make_file_name_path(a,b)
#define YACC_FILE_NAME_PREFIX(a,b) make_file_name_prefix(a,b)
#define YACC_FILE_NAME_SUFFIX(a,b) make_file_name_suffix(a,b)
#define YACC_FILE_NAME_TEMPLATE(a) make_file_name_template(a)
#define YACC_FILE_NAME_UTILITY(a,b) make_file_name_utility(a,b)
#define YACC_FLOATING_LITERAL_EXPRESSION(a) make_floating_literal_expression(a)
#define YACC_FOR_STATEMENT(a,b,c,d) make_for_statement(a,b,c,d)
#define YACC_FUNCTION_BLOCK(a) make_function_block(a)
#define YACC_FUNCTION_DEFINITION(a,b) make_function_definition(a,b)
#define YACC_GLOBAL_DECLARATOR(a,b) make_global_declarator(a,b)
#define YACC_GLOBAL_EXPRESSION(a, b) make_global_expression(a,b)
#define YACC_GLOBAL_ID(a,b) make_global_id(a,b)
#define YACC_GOTO_STATEMENT(a) make_goto_statement(a)
#define YACC_GREATER_EQUAL_EXPRESSION(a,b) make_greater_equal_expression(a,b)
#define YACC_GREATER_THAN_EXPRESSION(a,b) make_greater_than_expression(a,b)
#define YACC_HANDLER(a,b) make_handler(a,b)
#define YACC_HANDLERS(a,b) make_handlers(a,b)
#define YACC_IF_STATEMENT(a,b,c) make_if_statement(a,b,c)
#define YACC_INCLUDE_DECLARATION(a,b) make_include_declaration(a,b)
#define YACC_INCLUSIVE_OR_EXPRESSION(a,b) make_inclusive_or_expression(a,b)
#define YACC_INDEX_CAST_EXPRESSION(a,b) make_index_cast_expression(a,b)
#define YACC_INITIALIZER_CLAUSES(a,b) make_initializer_clauses(a,b)
#define YACC_INITIALIZER_EXPRESSION_CLAUSE(a) make_initializer_expression_clause(a)
#define YACC_INITIALIZER_LIST_CLAUSE(a) make_initializer_list_clause(a)
#define YACC_INIT_EXPRESSION(a) make_init_expression(a)
#define YACC_INIT_TEMPLATED_PARAMETER(a,b) make_init_templated_parameter(a,b)
#define YACC_INIT_SIMPLE_TYPE_PARAMETER(a,b) make_init_simple_type_parameter(a,b)
#define YACC_INLINE_IN_IMPLEMENTATION() make_inline_in_implementation()
#define YACC_INLINE_IN_INTERFACE() make_inline_in_interface()
#define YACC_INPUT_FILE(a) make_input_file(a)
#define YACC_INTEGER_LITERAL_EXPRESSION(a) make_integer_literal_expression(a)
#define YACC_LABEL_STATEMENT(a,b) make_label_statement(a,b)
#define YACC_LESS_EQUAL_EXPRESSION(a,b) make_less_equal_expression(a,b)
#define YACC_LESS_THAN_EXPRESSION(a,b) make_less_than_expression(a,b)
#define YACC_LINE() make_line()
#define YACC_LINED_DECLARATION(a,b) make_lined_declaration(a,b)
#define YACC_LINED_STATEMENT(a,b) make_lined_statement(a,b)
#define YACC_LINED_TOKEN(a,b) make_lined_token(a,b)
#define YACC_LINKAGE_SPECIFICATION(a) make_linkage_specification(a)
#define YACC_LINKAGE_SPECIFIER(a,b) make_linkage_specifier(a,b)
#define YACC_LOGICAL_AND_EXPRESSION(a,b) make_logical_and_expression(a,b)
#define YACC_LOGICAL_OR_EXPRESSION(a,b) make_logical_or_expression(a,b)
#define YACC_MEMBER_DECLARATIONS(a,b) make_member_declarations(a,b)
#define YACC_MEM_INITIALIZER(a,b) make_mem_initializer(a,b)
#define YACC_MEM_INITIALIZERS(a,b) make_mem_initializers(a,b)
#define YACC_META_ASSIGNMENT_EXPRESSION(a,b,c) make_meta_assignment_expression(a,b,c)
#define YACC_META_BASE_SPECIFIER(a) make_meta_base_specifier(a)
#define YACC_META_BUILT_IN_TYPE(a) make_meta_built_in_type(a)
#define YACC_META_CLASS(a,b,c) make_meta_class(a,b,c)
#define YACC_META_DECLARATION(a) make_meta_declaration(a)
#define YACC_META_FUNCTION(a,b,c) make_meta_function(a,b,c)
#define YACC_META_INITIALIZER(a,b) make_meta_initializer(a,b)
#define YACC_META_INITIALIZERS(a,b) make_meta_initializers(a,b)
#define YACC_META_STATEMENT(a) make_meta_statement(a)
#define YACC_META_STATEMENT_DECLARATION(a) make_meta_statement_declaration(a)
#define YACC_META_TYPE(a) make_meta_type(a)
#define YACC_MINUS_EXPRESSION(a) make_minus_expression(a)
#define YACC_MODULUS_EXPRESSION(a,b) make_modulus_expression(a,b)
#define YACC_MULTIPLY_EXPRESSION(a,b,c) make_multiply_expression(a,b,c)
#define YACC_NAME(a) make_name(a)
#define YACC_NAMESPACE_ALIAS_DEFINITION(a,b) make_namespace_alias_definition(a,b)
#define YACC_NAMESPACE_DECLARATION(a) make_namespace_declaration(a)
#define YACC_NAMESPACE_DEFINITION(a,b) make_namespace_definition(a,b)
#define YACC_NAME_EXPRESSION(a) make_name_expression(a)
#define YACC_NESTED_DECLARATOR(a,b) make_nested_declarator(a,b)
#define YACC_NESTED_ID(a,b) make_nested_id(a,b)
#define YACC_NESTED_SCOPE(a) make_nested_scope(a)
#define YACC_NEW_EXPRESSION(a,b,c) make_new_expression(a,b,c)
#define YACC_NEW_TYPE_ID_EXPRESSION(a,b,c) make_new_type_id_expression(a,b,c)
#define YACC_NOT_CONST() make_not_const()
#define YACC_NOT_EQUAL_EXPRESSION(a,b) make_not_equal_expression(a,b)
#define YACC_NOT_EXPRESSION(a) make_not_expression(a)
#define YACC_NOT_INLINE() make_not_inline()
#define YACC_NOT_STATIC() make_not_static()
#define YACC_NOT_VIRTUAL() make_not_virtual()
#define YACC_NOT_VIRTUAL_BASE_SPECIFIER(a) make_not_virtual_base_specifier(a)
#define YACC_NOT_VOLATILE() make_not_volatile()
#define YACC_NUMBER_LITERAL_EXPRESSION(a) make_number_literal_expression(a)
#define YACC_OBJECT_SCOPE_EXPRESSION(a,b) make_object_scope_expression(a,b)
#define YACC_OPERATOR_ADD_ID() make_operator_add_id()
#define YACC_OPERATOR_ARROW_ID() make_operator_arrow_id()
#define YACC_OPERATOR_ARROW_STAR_ID() make_operator_arrow_star_id()
#define YACC_OPERATOR_ASS_ADD_ID() make_operator_ass_add_id()
#define YACC_OPERATOR_ASS_BIT_AND_ID() make_operator_ass_bit_and_id()
#define YACC_OPERATOR_ASS_BIT_OR_ID() make_operator_ass_bit_or_id()
#define YACC_OPERATOR_ASS_DIV_ID() make_operator_ass_div_id()
#define YACC_OPERATOR_ASS_ID() make_operator_ass_id()
#define YACC_OPERATOR_ASS_MOD_ID() make_operator_ass_mod_id()
#define YACC_OPERATOR_ASS_MUL_ID() make_operator_ass_mul_id()
#define YACC_OPERATOR_ASS_SHL_ID() make_operator_ass_shl_id()
#define YACC_OPERATOR_ASS_SHR_ID() make_operator_ass_shr_id()
#define YACC_OPERATOR_ASS_SUB_ID() make_operator_ass_sub_id()
#define YACC_OPERATOR_ASS_XOR_ID() make_operator_ass_xor_id()
#define YACC_OPERATOR_BIT_AND_ID() make_operator_bit_and_id()
#define YACC_OPERATOR_BIT_NOT_ID() make_operator_bit_not_id()
#define YACC_OPERATOR_BIT_OR_ID() make_operator_bit_or_id()
#define YACC_OPERATOR_CALL_ID() make_operator_call_id()
#define YACC_OPERATOR_COMMA_ID() make_operator_comma_id()
#define YACC_OPERATOR_DEC_ID() make_operator_dec_id()
#define YACC_OPERATOR_DELETE_ID() make_operator_delete_id()
#define YACC_OPERATOR_DIV_ID() make_operator_div_id()
#define YACC_OPERATOR_EQ_ID() make_operator_eq_id()
#define YACC_OPERATOR_FUNCTION_ID(a) make_operator_function_id(a)
#define YACC_OPERATOR_GE_ID() make_operator_ge_id()
#define YACC_OPERATOR_GT_ID() make_operator_gt_id()
#define YACC_OPERATOR_INC_ID() make_operator_inc_id()
#define YACC_OPERATOR_INDEX_ID() make_operator_index_id()
#define YACC_OPERATOR_LE_ID() make_operator_le_id()
#define YACC_OPERATOR_LOG_AND_ID() make_operator_log_and_id()
#define YACC_OPERATOR_LOG_NOT_ID() make_operator_log_not_id()
#define YACC_OPERATOR_LOG_OR_ID() make_operator_log_or_id()
#define YACC_OPERATOR_LT_ID() make_operator_lt_id()
#define YACC_OPERATOR_MOD_ID() make_operator_mod_id()
#define YACC_OPERATOR_MUL_ID() make_operator_mul_id()
#define YACC_OPERATOR_NE_ID() make_operator_ne_id()
#define YACC_OPERATOR_NEW_ID() make_operator_new_id()
#define YACC_OPERATOR_SHL_ID() make_operator_shl_id()
#define YACC_OPERATOR_SHR_ID() make_operator_shr_id()
#define YACC_OPERATOR_SUB_ID() make_operator_sub_id()
#define YACC_OPERATOR_XOR_ID() make_operator_xor_id()
#define YACC_PARAMETERS(a,b) make_parameters(a,b)
#define YACC_PARENTHESISED(a,b,c) make_parenthesised(a,b,c)
#define YACC_POINTER_DECLARATOR() make_pointer_declarator()
#define YACC_POINTER_EXPRESSION(a,b) make_pointer_expression(a,b)
#define YACC_PLUS_EXPRESSION(a) make_plus_expression(a)
#define YACC_POSITION(a,b) make_position(a)
#define YACC_POSITION_FUNCTION_BLOCK(a,b) make_position_function_block(a,b)
#define YACC_POST_DECREMENT_EXPRESSION(a) make_post_decrement_expression(a)
#define YACC_POST_INCREMENT_EXPRESSION(a) make_post_increment_expression(a)
#define YACC_PRE_DECREMENT_EXPRESSION(a) make_pre_decrement_expression(a)
#define YACC_PRE_INCREMENT_EXPRESSION(a) make_pre_increment_expression(a)
#define YACC_PSEUDO_DESTRUCTOR_ID(a,b) make_pseudo_destructor_id(a,b)
#define YACC_PURE_VIRTUAL() make_pure_virtual()
#define YACC_READ_ONLY_RESULT(a) make_read_only_result(a)
#define YACC_READ_WRITE_RESULT(a) make_read_write_result(a)
#define YACC_REFERENCE_DECLARATOR() make_reference_declarator()
#define YACC_REINTERPRET_CAST_EXPRESSION(a,b) make_reinterpret_cast_expression(a,b)
#define YACC_RESULT(a) make_result(a)
#define YACC_RETURN_STATEMENT(a) make_return_statement(a)
#define YACC_SCOPED_POINTER_EXPRESSION(a,b,c) make_scoped_pointer_expression(a,b,c)
#define YACC_SEGMENT(a,b) make_segment(a)
#define YACC_SEGMENT_FUNCTION_BLOCK(a,b) make_segment_function_block(a,b)
#define YACC_SET_TEMPLATE_DECLARATION(a) make_set_template_declaration(a)
#define YACC_SET_TEMPLATE_DECL_SPECIFIER(a) make_set_template_decl_specifier(a)
#define YACC_SET_TEMPLATE_EXPRESSION(a) make_set_template_expression(a)
#define YACC_SET_TEMPLATE_ID(a) make_set_template_id(a)
#define YACC_SET_TEMPLATE_NAME(a) make_set_template_name(a)
#define YACC_SHIFT_LEFT_EXPRESSION(a,b) make_shift_left_expression(a,b)
#define YACC_SHIFT_RIGHT_EXPRESSION(a,b) make_shift_right_expression(a,b)
#define YACC_SIMPLE_DECLARATION(a) make_simple_declaration(a)
#define YACC_SIZEOF_EXPRESSION(a) make_sizeof_expression(a)
#define YACC_STATEMENTS(a,b) make_statements(a,b)
#define YACC_STATIC_CAST_EXPRESSION(a,b) make_static_cast_expression(a,b)
#define YACC_STRINGS(a,b) make_strings(a,b)
#define YACC_STRING_LITERAL_EXPRESSION(a) make_string_literal_expression(a)
#define YACC_SUBSPACE(a,b) make_subspace(a,b)
#define YACC_SUBTRACT_EXPRESSION(a,b) make_subtract_expression(a,b)
#define YACC_SWITCH_STATEMENT(a,b) make_switch_statement(a,b)
#define YACC_SYNTAX_MACRO_DEFINITION(a,b,c,d,e) make_syntax_macro_definition(a,b,c,d,e)
#define YACC_SYNTAX_MACRO_PARAMETER(a,b,c) make_syntax_macro_parameter(a,b,c)
#define YACC_SYNTAX_MACRO_PARAMETERS(a,b) make_syntax_macro_parameters(a,b)
#define YACC_TEMPLATE_ARGUMENT(a) make_template_argument(a)
#define YACC_TEMPLATE_ARGUMENTS(a,b) make_template_arguments(a,b)
#define YACC_TEMPLATED_TYPE_PARAMETER(a,b) make_templated_type_parameter(a,b)
#define YACC_TEMPLATE_DECLARATION(a,b) make_template_declaration(a,b)
#define YACC_TEMPLATE_NAME(a,b) make_template_name(a,b)
#define YACC_TEMPLATE_PARAMETER(a) make_template_parameter(a)
#define YACC_TEMPLATE_PARAMETERS(a,b) make_template_parameters(a,b)
#define YACC_THIS_EXPRESSION() make_this_expression()
#define YACC_THROW_EXPRESSION(a) make_throw_expression(a)
#define YACC_TOKENS_EXPRESSION(a) make_tokens_expression(a)
#define YACC_TREE_ARGUMENT(a) make_tree_argument(a)
#define YACC_TREE_ARGUMENTS(a,b) make_tree_arguments(a,b)
#define YACC_TREE_ARRAY_EXPRESSION(a,b) make_tree_array_expression(a,b)
#define YACC_TREE_ARROW_EXPRESSION(a,b) make_tree_arrow_expression(a,b)
#define YACC_TREE_CALL_EXPRESSION(a,b) make_tree_call_expression(a,b)
#define YACC_TREE_DOT_EXPRESSION(a,b) make_tree_dot_expression(a,b)
#define YACC_TREE_EXPRESSION(a) make_tree_expression(a)
#define YACC_TREE_ID(a) make_tree_id(a)
#define YACC_TREE_POINTER_EXPRESSION(a) make_tree_pointer_expression(a)
#define YACC_TREE_STATEMENTS(a,b) make_tree_statements(a,b)
#define YACC_TRUE_EXPRESSION() make_true_expression()
#define YACC_TRY_BLOCK(a,b) make_try_block(a,b)
#define YACC_TRY_FUNCTION_BLOCK(a,b) make_try_function_block(a,b)
#define YACC_TYPE1_EXPRESSION(a,b,c) make_type1_expression(a,b,c)
#define YACC_TYPE1_PARAMETERS(a,b) make_type1_parameters(a,b)
#define YACC_TYPED_EXPRESSION(a,b) make_typed_expression(a,b)
#define YACC_TYPED_NAME(a,b) make_typed_name(a,b)
#define YACC_TYPEID_EXPRESSION(a) make_typeid_expression(a)
#define YACC_TYPENAME_TYPE_PARAMETER(a) make_typename_type_parameter(a)
#define YACC_TYPE_TEMPLATE_PARAMETER(a,b) make_type_template_parameter(a,b)
#define YACC_USING_DECLARATION(a,b) make_using_declaration(a,b)
#define YACC_USING_DIRECTIVE(a) make_using_directive(a)
#define YACC_USING_FUNCTION_BLOCK(a,b) make_using_function_block(a,b)
#define YACC_USING_IMPLEMENTATION_DECLARATION(a) make_using_implementation_declaration(a)
#define YACC_USING_INTERFACE_DECLARATION(a) make_using_interface_declaration(a)
#define YACC_UTILITY(a) make_utility(&FogUtility::a##_utility())
#define YACC_UTILITY_MODE() make_utility_mode()
#define YACC_VIRTUAL_BASE_SPECIFIER(a) make_virtual_base_specifier(a)
#define YACC_WHILE_STATEMENT(a,b) make_while_statement(a,b)

#endif
