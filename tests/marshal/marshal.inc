auto class Marshal {};

typedef unsigned long size_t;

auto declaration unsigned long::marshal(expression name)
{
	byte_count += 4;
//	using marshal
	public virtual size_t marshal(unsigned char dataBuffer[]) const
	{
    	*p++ = ($name >> 24) & 0xFF;
		*p++ = ($name >> 16) & 0xFF;
		*p++ = ($name >> 8) & 0xFF;
		*p++ = $name & 0xFF;
    }
//	using ${Scope}
	private ${Scope}(unsigned char dataBuffer[])
	{
		{
			unsigned long temp = *p++;
			temp = (temp << 8) | *p++;
			temp = (temp << 8) | *p++;
			temp = (temp << 8) | *p++;
			$name = temp;
		}
	}
}

auto declaration short::marshal(expression name)
{
	auto byte_count += 2;
//	using marshal
	public virtual size_t marshal(unsigned char dataBuffer[]) const
	{
        *p++ = ($name >> 8) & 0xFF;
		*p++ = $name & 0xFF;
    }
//	using ${This}
	private ${Scope}(unsigned char dataBuffer[])
	{
		{
			unsigned long temp = *p++;
			temp = (temp << 8) | *p++;
			$name = temp;
		}
	}
}

auto declaration Marshal::install()
{
	auto type MessageClass = $Scope;
    auto static statement switchBody =
    {
        default:
            return 0;
    }	
	auto number byte_count = 0;
	protected enum MessageTypes {};

	public virtual size_t marshal(unsigned char dataBuffer[]) const
	:{
        derived(true) entry
        {
            unsigned char *p = dataBuffer;
			*p++ = MESSAGE_@Scope;
			*p++ = @byte_count;
        }
	    derived(true) exit
        {
            return p - (dataBuffer+2);
        }
    };

	public static !inline $Scope *unmarshal(unsigned char dataBuffer[])
	{
        switch (dataBuffer[0])
            @switchBody;
    }

	public static @Scope *make(unsigned char dataBuffer[])
	:{ derived(true)
		{
            if (*p != @byte_count)
                return 0;
			else
                return new @{Scope}(dataBuffer);
        }
    };

	auto ${Scope}()
	{
		protected enum ${MessageClass}::MessageTypes { MESSAGE_$Dynamic };
        auto switchBody +=
            case MESSAGE_$Dynamic:
                return ${Dynamic}::make(dataBuffer);
		private inline/implementation ${Dynamic}(unsigned char dataBuffer[])
		{
            unsigned char *p = dataBuffer + 2;
        }
	}

	auto ~${Scope}()
	{
		auto for (iterator i = $variables(); i; i++)
			auto if (!i->is_static())
				$i->type().marshal($i->id());
	}
}
