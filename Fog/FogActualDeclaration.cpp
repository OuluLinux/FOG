#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogActualDeclaration, Super)

FogActualDeclaration::FogActualDeclaration(const FogSpecifier* aSpecifier)
	:
	_specifier(aSpecifier) {}

FogActualDeclaration::~FogActualDeclaration() {}

void FogActualDeclaration::destroy() {
	_mutable.reset();
	_specifier.reset();
}

//
//  	Modify the declaration to trim all components that do not belong with an instantiation, that is
//  	remove initialisers of a variable of ctor-inits and body of a function.
//
void FogActualDeclaration::instantiate() {
	FogSpecifier& mutableSpecifier = mutable_specifier();
	
	if (!mutableSpecifier.is_null())    //   Null if nothing from primary
		mutableSpecifier.instantiate();
}

//
//  	Update _specifier to incorporate a merge of aSpecifier, using _mutable as a repository of a
//  	writeable version of _specifier that can be updated when necessary.
//
void FogActualDeclaration::merge(FogMergeContext& mergeContext, const FogSpecifier& aSpecifier) {
	if (!_specifier)
		_specifier = aSpecifier;
	else {
		const FogMerge& needsMerge = _specifier->needs_merge(mergeContext, aSpecifier);
		
		if (needsMerge.is_impossible())
			ERRMSG(needsMerge << " -- cannot merge\n\t\t" << viz(aSpecifier)
			       << "\n\t\tinto " << viz(*_specifier));
		else if (needsMerge.left_is_valid())
			;
		else if (needsMerge.right_is_valid()) {
			_specifier = aSpecifier;
			_mutable.reset();
		}
		else {
			if (!_mutable) {
				_mutable.adopt(_specifier->derived_clone());
				_specifier = &*_mutable;
			}
			
			if (_mutable) {
				aSpecifier.merge_into(mergeContext, _mutable.to_const());
				_specifier = &*_mutable;
			}
		}
	}
}

FogSpecifier& FogActualDeclaration::mutable_specifier() {
	if (!_mutable) {
		_mutable.adopt(_specifier->derived_clone());
		_specifier = &*_mutable;
	}
	
	return *_mutable;
}

std::ostream& FogActualDeclaration::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_specifier)
		_specifier->print_deep(s, aDepth + 1);
		
	return s;
}

std::ostream& FogActualDeclaration::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_specifier)
		_specifier->print_on(s, aDepth + 1);
		
	return s;
}

std::ostream& FogActualDeclaration::print_this(std::ostream& s) const {
	return _specifier->print_this(s);
}

std::ostream& FogActualDeclaration::print_viz(std::ostream& s) const {
	return _specifier->print_viz(s);
}
