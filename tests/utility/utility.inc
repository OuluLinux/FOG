class PrimTypeInfo;

//
//**********************************************************************************************
//	PrimPrint RTTI protocols.
//**********************************************************************************************
//
auto initializer_clause TYPEINFO_BASE_OF(identifier Base, identifier Parent)
{{ &${Base}::static_type_info(), (char *)&($Base&)(${Parent}&)*(char *)0x1000 - (char *)0x1000 }};

class PrimPrint
{
public:
	virtual const PrimTypeInfo& dynamic_type_info() const
	:{ derived(true) { return static_type_info(); } };
	static const PrimTypeInfo& static_type_info();
};

auto declaration PRIMPRINT_RTTI_0(identifier Class)
{
	class $Class
	{
		using/implementation=PrimTypeInfo;
	public:
		using static_type_info
		{
			static const PrimTypeInfo_Base baseList[2] =
			{
				{ 0, 0 }
			};
			static const PrimTypeInfo typeInfo(""$Class, baseList);
			return typeInfo;
		};
	};
};

auto declaration PRIMPRINT_RTTI_1(identifier Class, identifier Base)
{
	class $Class
	{
		using/implementation=PrimTypeInfo;
	public:
		using static_type_info
		{
			static const PrimTypeInfo_Base baseList[2] =
			{
				${TYPEINFO_BASE_OF(${Base},$Class)},
				{ 0, 0 }
			};
			static const PrimTypeInfo typeInfo(""$Class, baseList);
			return typeInfo;
		};
	};
};

$PRIMPRINT_RTTI_0(PrimPrint);

class UtilityInc1 : public PrimPrint
{
	int _x;
};

$PRIMPRINT_RTTI_1(UtilityInc1,PrimPrint);

class UtilityInc2
{
	int _x;
};

class UtilityRef1;
class UtilityRef2;
class UtilityRef3;
