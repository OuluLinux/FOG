#include <Prim/PrimIncludeAll.h>




#ifndef NO_PRIM_TYPEINFO







TYPEINFO_SINGLE(PrimTypeInfo, PrimPrint);

#ifndef NO_PRIM_CHECKS
//
//  		Check the returns from the run-time type information to validate that the compileTimeCast is
//  		identical to the runTimeCast, using someText to qualify any error diagnostics. If the runTimeCast
//  		failed 0 is returned, if inconsistent or correct the compileTimeCast is returned.
//
//  		This routine is intended to invoked by the CHECKED_CAST macro which provides the arguments
//  		and a correct type for the return.
//
//  		CHECKED_CAST(Type, aPointer, someText) performs a cast of aPointer to Type * returning 0
//  		if the cast is not possible after using someText as part of an explanatory error message.
//  		If PRIM_TYPEINFO is not defined CHECKED_CAST just performs a "C" style cast.
//
void* PrimTypeInfo::check_cast(void* compileTimeCast, void* runTimeCast, const char* someText) {
	if (runTimeCast == 0) {
		ERRMSG("Run-time cast failed in " << someText);
		return 0;
	}
	
	if (compileTimeCast != runTimeCast)
		ERRMSG("Run-time cast inconsistent in " << someText);
		
	return compileTimeCast;
}
#endif

//
//  		The default initialise class implementation does nothing, returning true if the class is
//  		fully initialised, false if initialisation has yet to occur. Derived template classes reimplement
//  		to establish composite type names.
//
bool PrimTypeInfo::initialise_class() const {
	return _name != 0;
}

//
//  		Report the address of the base class described by aTypeInfo within ofObject, returning
//  		0 if ofObject has no such base class. If isDirect is true, the further constraint that
//  		aTypeInfo describes a direct base class is imposed. If ofObject is described by aTypeInfo
//  		report the address of ofObject.
//
//  		This method is intended to be used to support locate_type(). If a requirement for more
//  		general use is identified, consult the author to define an appropriate interface macro.
//
void* PrimTypeInfo::locate_base(const PrimTypeInfo& aTypeInfo, const void* ofObject, bool isDirect) const {
	if (this == &aTypeInfo)
		return (void*)ofObject;
		
	if (_bases != 0) {
		const void* bestBase = 0;
		const PrimTypeInfo_Base* aBase = _bases;
		
		for ( ; aBase->type != 0; aBase++) {
			if (&aTypeInfo == aBase->type)
				return (void*)((const char*)ofObject + aBase->offset);
				
			if (isDirect)
				continue;
				
			const void* const returnAddress = aBase->type->locate_base(aTypeInfo, ofObject, false);
			
			if ((returnAddress != 0) && ((bestBase == 0) || (returnAddress < bestBase)))
				bestBase = returnAddress;
		}
		
		if (bestBase != 0)
			return ((void*)((const char*)bestBase + aBase->offset));
	}
	
	return 0;
}

//
//  		Report the address of the class of type aTypeInfo within the object at ofObject most closely associated
//  		with the sub-object at ofSubObject. If isDirect is true, the further constraint that
//  		aTypeInfo describes a direct base class is imposed.
//
//  		If ofObject is described by aTypeInfo report the address of ofObject.
//
//  		Otherwise if ofSubObject has a base of aTypeInfo then return the first such base.
//
//  		Otherwise recurse down through each derived class of ofSubObject returning the first base of type
//  		aTypeInfo.
//
//  		Otherwise return 0.
//
//  		This method should be invoked via the ptr_cast or ref_cast macros, which automatically
//  		determine "this", aTypeInfo from sensible parameters, and performs the required
//  		cast back from the returned void *.
//
//  		Unlike Stroustrup's can_cast/hasBase mechanism, this method performs the address
//  		translation as required. Stroustrup's macro assumes that the compiler is able to
//  		perform the correct addressing offset from inherited base class. This is not
//  		true for ptr_cast(b, c), where c has b as a non-first base but at the point
//  		of invocation c is declared to be a pointer to one of its other bases.
//  .details
//  		If you don't understand the foregoing discussion consider the following cases:
//
//  										A		B		A		B
//  										 \	   /		 \	   /
//  										  \	  /			  \	  /
//  										   \ /			   \ /
//  							C				D				E
//  								\			|			/
//  									\		|		/
//  										\	|	/
//  											Q
//
//  		[ class Q : C, D, E {}; class D : A, B {}; class E : A, B {}; ]
//
//  		Case 1: an apparently ambiguous base class enquiry - given a E& return a B&
//  						this must return Q::E::B, not Q::D::B although B is not the depth-first first B.
//
//  		Case 2: an ambiguous base class enquiry - given a Q& return a B&
//  						either Q::D::B or Q::E::B is possible, return Q::D::B as the first.
//
//  		Case 3: a genuinely ambiguous base class enquiry - given a C& return a B&
//  						there is no directly available B, so it could be argued the cast should fail,
//  						however in most implementations C and Q are at the same address and so
//  						the foregoing would be inconsistent according to the base alignments so
//  						either Q::D::B or Q::E::B is possible, return Q::D::B as the first.
//
//  		Case 4: a very simple down cast - given a C& return a Q&
//  						this just down-casts at the same address.
//
//  		Case 5: a simple down cast - given a D& return a Q&
//  						this just down-casts applying the appropriate address bias.
//
//  		Case 6: a tricky down cast - given a E::B& return a E&
//  						this must down-cast to Q::E not to Q::D.
//
//  		Case 7: anarchy - given a E::B& return a A&
//  						this must down-cast to Q::E and then up-cast to Q::E:A.
//
//  		The foregoing examples demonstrate the need for three pieces of information.
//  		The base address of the overall object in question (Q&), the starting point of the
//  		type conversion (E::B& in case 7), and the resultant type (A in case 7).
//
//  		The virtual function PrimPrint::report_this() provides the overall address.
//  		The address as seen by the compiler is the sub-object address.
//  		The static function A::report_type_info() privides the target type.
//
//  		The ptr_cast() macro hides the complexity from the user.
//
void* PrimTypeInfo::locate_type(const PrimTypeInfo& aTypeInfo, const void* ofObject,
                                const void* ofSubObject, bool isDirect) const {
	if (this == &aTypeInfo)
		return ((void*)ofObject);
		
	if (_bases != 0) {
		for (const PrimTypeInfo_Base* aBase = _bases; aBase->type != 0; aBase++) {
			const void* const loAddress = (const char*)ofObject + aBase->offset;
			
			if (ofSubObject < loAddress)
				continue;
				
			if ((aBase + 1)->type != 0) {
				const void* const hiAddress = (const char*)ofObject + (aBase + 1)->offset;
				
				if (hiAddress <= loAddress)
					PRIMERR(BAD_TYPE_ORDER, *this << " has inconsistent base class ordering.");
					
				if (hiAddress <= ofSubObject)
					continue;
			}
			
			if (&aTypeInfo == aBase->type)
				return (void*)((const char*)ofObject + aBase->offset);
				
			if (isDirect)
				return 0;
				
			void* const returnAddress = aBase->type->locate_type(aTypeInfo, loAddress, ofSubObject, false);
			
			if (returnAddress != 0)
				return returnAddress;
		}
		
		return (locate_base(aTypeInfo, ofObject, false));
	}
	
	return 0;
}

#ifndef NO_PRIM_STREAMS
//
//  		Provide additional one-line diagnostic description of type information.
//
std::ostream& PrimTypeInfo::print_this(std::ostream& s) const {
	return s << _name;
}
#endif

//
//  		Create a new type descriptor named aName<t1> with someBases identifying the base classes
//
//  PrimTypeInfo1::PrimTypeInfo1(const char *aName, const PrimTypeId& t1, const PrimTypeInfo_Base *someBases)
//  :
//  	PrimTypeInfo(aName, someBases),
//  	_type_info1(t1.type_info())
//  {}

//
//  		Initialise this class by constructing the composite type description as soon as the template
//  		class has been constructed. Returns false if the class remains invalid awaiting its template
//  		parameter.
//
//  bool PrimTypeInfo1::initialise_class() const
//  {
//  	if (!_full_name && _type_info1.initialise_class())
//  	{
//  		ostrstream s;
//  		const char *const aClassName = _type_info1.name();
//  		s << PrimTypeInfo::name() << '<' << aClassName << '>' << ends;
//  		PrimTypeInfo1 *mutableThis = (PrimTypeInfo1 *)this;
//  		PrimAdoptedArray<char> buf(s.str());
//  		mutableThis->_full_name = buf.get();
//  	}
//  	return _full_name.is_valid();
//  }

//
//  		Report the name of this class, constructing the full template name if not yet available.
//
//  const char *PrimTypeInfo1::name() const
//  {
//  	return !_full_name && !initialise_class() ? PrimTypeInfo::name() : _full_name.str();
//  }
#endif
