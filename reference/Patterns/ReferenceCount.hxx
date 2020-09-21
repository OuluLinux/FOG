/*!$@FOG@$!
 *	Generated at Sat Sep 14 14:59:18 2019
 *
 *	by fog 0.1.a of 14:58:03 Sep 14 2019
 *
 *	from
 *		Patterns.fog
 */

#ifndef REFERENCECOUNT_HXX
#define REFERENCECOUNT_HXX

class ReferenceCount
{
private:
#line 45 "Patterns.fog"
    unsigned int shares;
    
public:
    inline ReferenceCount();
    inline ReferenceCount(const ReferenceCount& );
#line 51
    inline ~ReferenceCount();
#line 50
    inline ReferenceCount& operator= (const ReferenceCount& );
#line 52
    bool Dec() const;
    inline bool DecHeapOnly() const;
#line 55
    inline unsigned int GetShareCount() const;
#line 54
    inline void Inc() const;
};

#line 48
inline ReferenceCount::ReferenceCount()
:
    shares(1)
{};

#line 49
inline ReferenceCount::ReferenceCount(const ReferenceCount& )
:
    shares(1)
{};

#line 51
inline ReferenceCount::~ReferenceCount() {};

#line 50
inline ReferenceCount& ReferenceCount::operator= (const ReferenceCount& )
{
#line 50
    return *this;
};

inline bool ReferenceCount::DecHeapOnly() const
{
#line 53
    return -- _shares != 0;
};

#line 55
inline unsigned int ReferenceCount::GetShareCount() const
{
#line 55
    return shares;
};

#line 54
inline void ReferenceCount::Inc() const
{
#line 54
    shares ++ ;
};

#endif
