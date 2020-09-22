#ifndef FILE_CXX
#define FILE_CXX

#ifndef FILE_HXX
#include <File.hxx>
#endif

#line 96 "tests/derived/derived.fog"
const char *Level0::class_name = "Level0";

#line 82
const Level0 *Level0::clone() const
{
#line 82
    return new Level0(*this);
};

#line 96
const char *Level1::class_name = "Level1";

#line 82
const Level0 *Level1::clone() const
{
#line 82
    return new Level1(*this);
};

#line 96
const char *Level2::class_name = "Level2";

#line 82
const Level0 *Level2::clone() const
{
#line 82
    return new Level2(*this);
};

#line 96
const char *Level3::class_name = "Level3";

#line 90
const Multiple *Level3::boundary_clone() const
{
#line 91
    return new Level3(*this);
};

const Multiple *Level3::boundary_method() const
{
#line 94
    return new Level3(*this);
};

#line 82
const Level0 *Level3::clone() const
{
#line 82
    return new Level3(*this);
};

#line 98
inline bool Level3::is_bool() const
{
    return _number.is_bool();
};

#line 96
const char *Level4::class_name = "Level4";

#line 82
const Level0 *Level4::clone() const
{
#line 82
    return new Level4(*this);
};

#endif
