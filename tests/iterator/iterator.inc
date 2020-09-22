class Meta
{
};

struct MetaFunctionInfo
{
	export/interface Meta; export/implementation Meta;
	const char *_name;
};

struct MetaVariableInfo
{
	export/interface Meta; export/implementation Meta;
	const char *_name;
};

struct MetaTypeInfo
{
	export/interface Meta; export/implementation Meta;
	const char *_name;
	const MetaTypeInfo *const *_bases;
	const MetaFunctionInfo *_functions;
	const MetaVariableInfo *_static_variables;
	const MetaVariableInfo *_member_variables;
};

auto Meta::Meta()
{
	public static const MetaTypeInfo& static_class_info() { return _type_info; };
	public virtual !inline const MetaTypeInfo& dynamic_class_info() const { return _type_info; };
	private static const MetaTypeInfo _type_info;
	private static const MetaTypeInfo *_bases[] = {};
	private static const MetaFunctionInfo _functions[] = {};
	private static const MetaVariableInfo _static_variables[] = {};
	private static const MetaVariableInfo _member_variables[] = {};
};

auto Meta::~Meta()
{
	private static const MetaTypeInfo _type_info =
	{
		""$This,
		${This}::_bases,
		${This}::_functions,
		${This}::_static_variables,
		${This}::_member_variables
	};
	for (iterator i = $bases(); i; ++i)
		if (!i->is_auto())
			private static const MetaTypeInfo *_bases[] = { &${i->base().name() }::static_class_info() };
	private static const MetaTypeInfo *_bases[] = { 0 };
	for (iterator j = $functions(); j; ++j)
		private static const MetaFunctionInfo _functions[] = { { ""$j->name() } };
	private static const MetaFunctionInfo _functions[] = { { 0 } };
	for (iterator k = $variables(); k; ++k)
		if (k->is_static())
			private static const MetaVariableInfo _static_variables[] = { { ""$k->name() } };
		else
			private static const MetaVariableInfo _member_variables[] = { { ""$k->name() } };
	private static const MetaVariableInfo _static_variables[] = { { 0 } };
	private static const MetaVariableInfo _member_variables[] = { { 0 } };
};
