#ifndef MESSAGE_CXX
#define MESSAGE_CXX

#ifndef MESSAGE_HXX
#include <Message.hxx>
#endif

#line 96 "tests/marshal/marshal.inc"
inline Message::Message(unsigned char dataBuffer[])
{
    unsigned char * p = dataBuffer + 2;
};

#line 80
Message *Message::make(unsigned char dataBuffer[])
{
#line 83
    if (*p != 0)
        return 0;
    else
        return new Message(dataBuffer);
};

#line 60
size_t Message::marshal(unsigned char dataBuffer[]) const
{
#line 64
    unsigned char * p = dataBuffer;
    *p ++ = MESSAGE_Message;
    *p ++ = 0;
#line 70
    return p - (dataBuffer + 2);
};

#line 74
Message *Message::unmarshal(unsigned char dataBuffer[])
{
    switch (dataBuffer[0])
    {
#line 54
        default:
            return 0;
#line 94
        case MESSAGE_Message:
            return Message::make(dataBuffer);
#line 94
        case MESSAGE_StockReport:
            return StockReport::make(dataBuffer);
    }
};

#line 96
inline StockReport::StockReport(unsigned char dataBuffer[])
{
    unsigned char * p = dataBuffer + 2;
    {
#line 20
        unsigned long temp = *p ++ ;
        temp = (temp << 8) | *p ++ ;
        temp = (temp << 8) | *p ++ ;
        temp = (temp << 8) | *p ++ ;
        _item_number = temp;
    }
    {
#line 42
        unsigned long temp = *p ++ ;
        temp = (temp << 8) | *p ++ ;
        _stock_level = temp;
    }
};

#line 80
StockReport *StockReport::make(unsigned char dataBuffer[])
{
#line 83
    if (*p != 6)
        return 0;
    else
        return new StockReport(dataBuffer);
};

#line 60
size_t StockReport::marshal(unsigned char dataBuffer[]) const
{
#line 64
    unsigned char * p = dataBuffer;
    *p ++ = MESSAGE_StockReport;
    *p ++ = 6;
#line 11
    *p ++ = (_item_number >> 24) & 0xFF;
    *p ++ = (_item_number >> 16) & 0xFF;
    *p ++ = (_item_number >> 8) & 0xFF;
    *p ++ = _item_number & 0xFF;
#line 35
    *p ++ = (_stock_level >> 8) & 0xFF;
    *p ++ = _stock_level & 0xFF;
#line 70
    return p - (dataBuffer + 2);
};

#endif
