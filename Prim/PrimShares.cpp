#include <Prim/PrimIncludeAll.h>





//  
//  	Destruction verifies consistent share count maintenance.
//  
PrimShares::~PrimShares()
{
 _shares--;
 if (_shares != 0)
    {
        char b_u_f[512] = { 0 };
        sprintf(b_u_f, "PrimShares @0x%p destructing with %d shares outstanding", this, _shares);
        PrimNotice_Error aNotice(b_u_f, sizeof(b_u_f));
        aNotice.post_on(0);
//        Visual C++ ostrstream doesn't work during static destruction when this is most likley called.
//  		GLOBAL_ERRMSG("PrimShares @" << this << " destructing with " << _shares << " shares outstanding");
    }
}

//  
//  	Annul one share, returning true if successful. Returns false if there was only one share in which case the
//  	caller must destruct in order to annul the last share.
//  
bool PrimShares::annul() const { return annul_inline(); }
//  
//  	Report the number of shares as part of a diagnostic print-out.
//  
std::ostream& operator<<(std::ostream& s, const PrimShares& someShares) { return (s << someShares._shares); }
