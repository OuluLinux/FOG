#include <Fog/FogIncludeAll.h>







struct FogUsageManager_Element {
	FogUsageListOfRef _independents;    //   Categorised unblocked usages.
	FogUsageMapOfRef _dependents;      //   Categorised blocked usages.
	bool _sorted;          //   True if _independents is sorted.
	size_t _threshold;        //   Threshold between processed and unprocessed _independents
	
private:
	FogUsageManager_Element(const FogUsageManager_Element&);
	FogUsageManager_Element& operator=(const FogUsageManager_Element&);
	
public:
	FogUsageManager_Element() : _sorted(false), _threshold(0) {}
};

TYPEINFO_SINGLE(FogUsageManager, Super)

FogUsageManager::FogUsageManager()
		:
		_elements(0),
		_first_index(INDEXES) {}
		
FogUsageManager::~FogUsageManager() {
	destroy();
}

void FogUsageManager::add_dependent(FogUsage& aUsage) {
	if (_elements)
		_elements[aUsage.type()]._dependents.add(aUsage);
	else
		ERRMSG("BUG should not FogUsageManager::add_dependent to unallocated list.");
}

void FogUsageManager::add_independent(FogUsage& aUsage) {
	if (_elements) {
		size_t anIndex = aUsage.type();
		_elements[anIndex]._independents.add(aUsage);
		
		if (anIndex < _first_index)
			_first_index = anIndex;
			
		_elements[anIndex]._sorted = false;
	}
	
	else
		ERRMSG("BUG should not FogUsageManager::add_independent to unallocated list.");
}

void FogUsageManager::compile() {
	_elements = new FogUsageManager_Element[INDEXES];
	
	if (!_elements)
		return;
		
	_usages.sort(&FogUsage::compare);      //   Encourage alphabetical order
	
	for (FogUsageConstListOfRefIterator p(_usages); p; ++p) {
		FogUsage& aUsage = *p;        //   Establish file names
		aUsage.precompile(*this);       //    and register dependencies.
	}
	
	_usages.vacate();
	
	size_t usageNumber = 0;
	const FogScope *lastScope = 0;
	
	while (true) {
	    //   Terminates when all names independent.
		while (_first_index < INDEXES) {
			const size_t thisIndex = _first_index;
			FogUsageManager_Element& firstElement = _elements[thisIndex];
			
			for (size_t toGo = firstElement._independents.tally() - firstElement._threshold; toGo;
				 toGo = firstElement._independents.tally() - firstElement._threshold) {
				if (!firstElement._sorted && (toGo > 1)) {
					firstElement._independents.sort(&FogUsage::compare, firstElement._threshold, toGo);
					firstElement._sorted = true;
				}
				
				FogUsage& inDependency = *firstElement._independents[firstElement._threshold++];
				
				inDependency.remove_all_uses(*this, ++usageNumber);  //   May change _first_index.
				_usages.add(inDependency);
				
				if (_first_index != thisIndex)        //   Higher priority list needed
					break;
			}
			
			if (_first_index >= thisIndex)
				_first_index++;
		}
		
		bool allDone = true;
		
		for (int i = 0; i < INDEXES; i++) {
			FogUsageManager_Element& anElement = _elements[i];
			
			if (anElement._dependents.tally()) {
			    //    Always breaks one loop. Eventually broken loops create independencies.
				allDone = false;
				FogUsageListOfRefToConst tracedList(64);
				FogUsageMapOfRefToConst tracedMap(128);
				FogUsageConstMapOfRefIterator p(anElement._dependents);
				FogUsage *endUsage = &*p;
				
				while (true) {
					FogUsage *nextUsage = endUsage->pick_cyclic_use_of(*this);
					if (nextUsage == endUsage) {
						int i = 0;
					}
					
					if (!nextUsage)        //   Only occurs if a bug has left endUsage acyclic
						break;         //    in which case there is a new _independents.
						
					VERBOSE(printf("SEARCHING %s\n", nextUsage->id().str()););
					
					if (!tracedMap.find(nextUsage->id())) {
						tracedList.add(*nextUsage);
						tracedMap.add(*nextUsage);
						endUsage = nextUsage;
					}
					
					else {
						VERBOSE(printf("FOUND %s\n", nextUsage->id().str());
						
						std::strstream str;
						str << "USAGE MANAGER\n";
						print_members(str, 0);
						std::cout << str.str() << endl;);
			
						show_cycle(tracedList, *nextUsage);
						endUsage->remove_use_of(*nextUsage, *this);
						break;
					}
				}
				
				break;
			}
		}
		
		if (allDone)
			break;
	}
	
	delete[] _elements;
	
	_elements = 0;
	
	for (FogUsageConstListOfRefIterator p4(_usages); p4; ++p4) {
		FogUsage& aUsage = *p4;        //   Register usages dependencies with files
		aUsage.compile();         //    before working list gets trashed
	}
}

void FogUsageManager::destroy() {
	for (FogUsageConstListOfRefIterator p(_usages); p; ++p)
		p->destroy();
		
	_usages.vacate();
	
	if (_elements) {
		delete[] _elements;
		_elements = 0;
	}
}

FogUsageManager& FogUsageManager::mutable_null() {
	static FogUsageManager nullManager;
	return nullManager;
}

FogUsage *FogUsageManager::make_enum_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers) {
	FogUsage *aUsage = FogUsage::new_enum_usage(anEntity, declSpecifiers);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_file_usage(FogTargetFile& targetFile) {
	FogUsage *aUsage = FogUsage::new_file_usage(targetFile);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_friend_usage(FogScope& ofScope, FogEntity& anEntity) {
	FogUsage *aUsage = FogUsage::new_friend_usage(ofScope, anEntity);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_function_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers) {
	FogUsage *aUsage = FogUsage::new_function_usage(anEntity, declSpecifiers);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_implementation_usage(FogEntity& anEntity) {
	FogUsage *aUsage = FogUsage::new_implementation_usage(anEntity);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_inline_usage(FogScope& aScope) {
	FogUsage *aUsage = FogUsage::new_inline_usage(aScope);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_interface_usage_finish(FogEntity& anEntity) {
	FogUsage *aUsage = FogUsage::new_interface_usage_finish(anEntity);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_interface_usage_start(FogEntity& anEntity) {
	FogUsage *aUsage = FogUsage::new_interface_usage_start(anEntity);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_name_usage(FogEntity& anEntity) {
	FogUsage *aUsage = FogUsage::new_name_usage(anEntity);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_typedef_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers) {
	FogUsage *aUsage = FogUsage::new_typedef_usage(anEntity, declSpecifiers);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

FogUsage *FogUsageManager::make_variable_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers) {
	FogUsage *aUsage = FogUsage::new_variable_usage(anEntity, declSpecifiers);
	
	if (aUsage)
		_usages.adopt(*aUsage);
		
	return aUsage;
}

std::ostream& FogUsageManager::print_depth(std::ostream& s, int aDepth) const {
	if (_usages.tally()) {
		s << indent(aDepth) << "usages\n";
		_usages.print_deep(s, aDepth+1);
	}
	
	if (_elements) {
		for (int j = 0; j < INDEXES; j++)
			if (_elements[j]._independents.tally()) {
				s << indent(aDepth) << "independent " << TypeEnum(j) << "s\n";
				_elements[j]._independents.print_members(s, aDepth+1);
			}
			
		for (int k = 0; k < INDEXES; k++)
			if (_elements[k]._dependents.tally()) {
				s << indent(aDepth)<< TypeEnum(k) << " usages with residual dependences\n";
				
				for (FogUsageConstMapOfRefToConstIterator p1(_elements[k]._dependents); p1; ++p1) {
//  					s << indent(aDepth+1) << p1->entity() << '\n';
					s << indent(aDepth+1) << *p1 << '\n';
					
					for (FogUsageConstMapOfRefToConstIterator p2(p1->use_of()); p2; ++p2)
						s << indent(aDepth+2) << *p2 << '\n';
				}
			}
	}
	
	return s;
}

std::ostream& FogUsageManager::print_members(std::ostream& s, int aDepth) const {
	if (_usages.tally()) {
		s << indent(aDepth) << "usages\n";
		_usages.print_members(s, aDepth+1);
	}
	
	if (_elements) {
		for (int j = 0; j < INDEXES; j++)
			if (_elements[j]._independents.tally()) {
				s << indent(aDepth) << "independent " << TypeEnum(j) << "s\n";
				_elements[j]._independents.print_members(s, aDepth+1);
			}
			
		for (int k = 0; k < INDEXES; k++)
			if (_elements[k]._dependents.tally()) {
				s << indent(aDepth) << TypeEnum(k) << " usages with residual dependences\n";
				_elements[k]._dependents.print_members(s, aDepth+1);
			}
	}
	
	return s;
}

std::ostream& FogUsageManager::print_viz(std::ostream& s) const {
	s << "\"usage-manager: \"";
	return s;
}

void FogUsageManager::promote_to_independent(FogUsage& aUsage) {
	TypeEnum anIndex = aUsage.type();
	
	if (!_elements[anIndex]._dependents.remove(aUsage.id()))
		ERRMSG("BUG - should not promote_to_independent " << viz(aUsage));
		
	add_independent(aUsage);
}

//
//  	Generate a diagnostic to explain the detected dependency cycle comprising the elements of thePath
//  	from cycleStart to the end of thePath. (Preceeding elements represent a non-cyclic pre-amble).
//
void FogUsageManager::show_cycle(const FogUsageListOfRefToConst& thePath, const FogUsage& cycleStart) const {
	size_t iFirst = 0;
	
	for (FogUsageConstListOfRefToConstIterator p(thePath); p; ++p, ++iFirst)
		if (&*p == &cycleStart)
			break;
			
	static const char *anExplanation =
		"\n"
		"FOG has failed to identify a legal declaration order.\n"
		"A usage cycle has been identified whereby declaration A directly or indirectly\n"
		"requires B to be declared first, but declaration B requires the reverse order.\n"
		"FOG will arbitrarily discard ordering constraints until all cycles have been eliminated.\n"
		"Output files can then be emitted, but compilation errors are to be expected.\n"
		"\n"
		"The problem must be resolved by:\n"
		"\treorganising nested declarations\n"
		"\tsimplifying inline function calls.\n";
	    
	if (anExplanation) {
		DIAMSGZ(anExplanation);
		anExplanation = 0;
	}
	
	PrimOstrstream s;
	
	s << "Cyclic usage identified and arbitrarily broken involving:";
	
	for (size_t i = thePath.tally(); i-- > iFirst;) {
		const FogUsage& aUsage = *thePath[i];
		s << "\n\t" << c_string(aUsage.id().str());
	}
	
	ERRMSGZ(s.str());
}
