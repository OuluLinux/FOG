#ifndef FILE_HXX
#define FILE_HXX

struct File
{
};

class Front;

class Level0
{
private:
#line 13 "tests/derived/derived.fog"
    typedef Level0 Level0_Boundary;
    typedef Level0 Level0_Concrete;
#line 8
    typedef Level0 Level0_Root;
#line 11
    typedef Level0 Level0_Tree;
    
public:
#line 82
    virtual const Level0 *clone() const;
#line 84
    inline Front *derived_clone() const;
    
protected:
#line 96
    static const char *class_name;
};

class Level1 : public Level0
{
private:
#line 9
    typedef Level1 Level0_Branch;
#line 14
    typedef Level1 Level0_Concrete;
#line 11
    typedef Level1 Level0_Tree;
#line 26
    typedef Level1 Level1_Boundary;
    typedef Level1 Level1_Concrete;
#line 21
    typedef Level1 Level1_Root;
#line 24
    typedef Level1 Level1_Tree;
#line 7
    typedef Level0 Super;
    
public:
#line 82
    virtual const Level0 *clone() const;
#line 84
    inline Front *derived_clone() const;
    
protected:
#line 96
    static const char *class_name;
};

class Multiple
{
private:
#line 38
    typedef Multiple Multiple_Pure;
#line 34
    typedef Multiple Multiple_Root;
#line 37
    typedef Multiple Multiple_Tree;
    
private:
#line 90
    /*pure*/virtual const Multiple *boundary_clone() const = 0;
#line 93
    /*pure*/virtual const Multiple *boundary_method() const = 0;
};

class Level2 : public Level1, private Multiple
{
private:
#line 9
    typedef Level2 Level0_Branch;
#line 14
    typedef Level2 Level0_Concrete;
#line 11
    typedef Level2 Level0_Tree;
#line 22
    typedef Level2 Level1_Branch;
#line 27
    typedef Level2 Level1_Concrete;
#line 24
    typedef Level2 Level1_Tree;
#line 51
    typedef Level2 Level2_Pure;
#line 47
    typedef Level2 Level2_Root;
#line 50
    typedef Level2 Level2_Tree;
#line 35
    typedef Level2 Multiple_Branch;
#line 38
    typedef Level2 Multiple_Pure;
#line 37
    typedef Level2 Multiple_Tree;
#line 7
    typedef Level1 Super;
    
public:
#line 82
    virtual const Level0 *clone() const;
#line 84
    inline Front *derived_clone() const;
    
protected:
#line 96
    static const char *class_name;
};

class Level3 : public Level2
{
private:
#line 9
    typedef Level3 Level0_Branch;
#line 14
    typedef Level3 Level0_Concrete;
#line 11
    typedef Level3 Level0_Tree;
#line 22
    typedef Level3 Level1_Branch;
#line 27
    typedef Level3 Level1_Concrete;
#line 24
    typedef Level3 Level1_Tree;
#line 52
    typedef Level3 Level2_Boundary;
#line 48
    typedef Level3 Level2_Branch;
#line 53
    typedef Level3 Level2_Concrete;
#line 50
    typedef Level3 Level2_Tree;
#line 65
    typedef Level3 Level3_Boundary;
    typedef Level3 Level3_Concrete;
#line 60
    typedef Level3 Level3_Root;
#line 63
    typedef Level3 Level3_Tree;
#line 39
    typedef Level3 Multiple_Boundary;
#line 35
    typedef Level3 Multiple_Branch;
#line 40
    typedef Level3 Multiple_Concrete;
#line 37
    typedef Level3 Multiple_Tree;
#line 7
    typedef Level2 Super;
    
public:
#line 82
    virtual const Level0 *clone() const;
#line 84
    inline Front *derived_clone() const;
#line 98
    inline bool is_bool() const;
    
private:
#line 90
    virtual const Multiple *boundary_clone() const;
#line 94
    virtual const Multiple *boundary_method() const;
    
protected:
#line 96
    static const char *class_name;
};

class Level4 : public Level3
{
private:
#line 14
    typedef Level4 Level0_Concrete;
#line 10
    typedef Level4 Level0_Leaf;
    typedef Level4 Level0_Tree;
#line 27
    typedef Level4 Level1_Concrete;
#line 23
    typedef Level4 Level1_Leaf;
    typedef Level4 Level1_Tree;
#line 53
    typedef Level4 Level2_Concrete;
#line 49
    typedef Level4 Level2_Leaf;
    typedef Level4 Level2_Tree;
#line 66
    typedef Level4 Level3_Concrete;
#line 62
    typedef Level4 Level3_Leaf;
    typedef Level4 Level3_Tree;
#line 78
    typedef Level4 Level4_Boundary;
    typedef Level4 Level4_Concrete;
#line 75
    typedef Level4 Level4_Leaf;
#line 73
    typedef Level4 Level4_Root;
#line 76
    typedef Level4 Level4_Tree;
#line 40
    typedef Level4 Multiple_Concrete;
#line 36
    typedef Level4 Multiple_Leaf;
    typedef Level4 Multiple_Tree;
#line 7
    typedef Level3 Super;
    
public:
#line 82
    virtual const Level0 *clone() const;
#line 84
    inline Front *derived_clone() const;
    
protected:
#line 96
    static const char *class_name;
};

#line 84
inline Front *Level0::derived_clone() const
{
    return clone();
};

#line 84
inline Front *Level1::derived_clone() const
{
#line 87
    Level1 * p = clone();
#line 87
    return p ? p : 0;
};

#line 84
inline Front *Level2::derived_clone() const
{
#line 87
    Level1 * p = clone();
#line 87
    return p ? p : 0;
};

#line 84
inline Front *Level3::derived_clone() const
{
#line 87
    Level1 * p = clone();
#line 87
    return p ? p : 0;
};

#line 84
inline Front *Level4::derived_clone() const
{
#line 87
    Level1 * p = clone();
#line 87
    return p ? p : 0;
};

#endif
