#ifndef MESSAGE_HXX
#define MESSAGE_HXX

#line 3 "tests/marshal/marshal.inc"
typedef unsigned long size_t;

class Message
{
protected:
    enum MessageTypes
    {
        MESSAGE_Message,
        MESSAGE_StockReport
    };
    
public:
#line 60
    virtual size_t marshal(unsigned char dataBuffer[]) const;
    
private:
#line 96
    inline Message(unsigned char dataBuffer[]);
    
public:
#line 80
    static Message *make(unsigned char dataBuffer[]);
#line 74
    static Message *unmarshal(unsigned char dataBuffer[]);
};

class StockReport : public Message
{
private:
#line 10 "tests/marshal/marshal.fog"
    unsigned long _item_number;
    short _stock_level;
    
public:
#line 60 "tests/marshal/marshal.inc"
    virtual size_t marshal(unsigned char dataBuffer[]) const;
    
private:
#line 96
    inline StockReport(unsigned char dataBuffer[]);
    
public:
#line 80
    static StockReport *make(unsigned char dataBuffer[]);
};

#endif
