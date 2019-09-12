#include <Fog/FogIncludeAll.h>








//  
//  	Return true if oldFile has equivalent content to newText, (and so there is no need to emit newText).
//  
FogFileComparator::Status FogFileComparator::equivalent(const char *newText, const PrimId& oldFile)
{
 std::ifstream f1(oldFile.str());
 if (!f1)
  return NEW_TEXT;
 char buf[4096];
 f1.read(buf, sizeof(buf));
 if ((f1.gcount() < 11) || (strncmp(buf, "/*!$@FOG@$!", 11) != 0))
  return NON_FOG_TEXT;
 PrimOstrstream s0;
 s0.write(buf, f1.gcount());
 while (f1)
 {
  f1.read(buf, sizeof(buf));
  s0.write(buf, f1.gcount());
 }
 FogFileComparator p1(s0.str());
 FogFileComparator p2(newText);
 int c1, c2;
 while (c1 = p1.next(), c2 = p2.next(), (c1 >= 0) || (c2 >= 0))
  if (c1 != c2)
   return CHANGED_TEXT;
 return SAME_TEXT;
}

//  
//  	Return the next non-comment character from the input stream, or -ve at end of stream.
//  
//  	/* ... */ 	unquoted and unescaped comments are ignored.
//  	// ... 		unquoted and unescaped comments are ignored.
//  	leading whitespace is suppressed.
//  	trailing whitespace is suppressed.
//  	all unquoted an unescaped whitespace is compressed to a single space.
//  
int FogFileComparator::next()
{
	bool gotSpace = false;
	_return_count++;
	while (*_p)
	{
		if (_escaped)
		{
			_escaped = false;
			if ((*_p == '\n') || (*_p == '\f'))
				ERRMSG("Escape encountered at end of line.");
			return *_p++;
		}
		else if (*_p == '\\')
		{
			_escaped = true;
			return *_p++;
		}
		else if ((*_p == '\'') && !_double_quoted)
		{
			_single_quoted = !_single_quoted;
			return *_p++;
		}
		else if ((*_p == '"') && !_single_quoted)
		{
			_double_quoted = !_double_quoted;
			return *_p++;
		}
		else if (_single_quoted || _double_quoted)
		{
			if ((*_p != '\n') && (*_p != '\f'))
				;
			else if (_single_quoted)
			{
				ERRMSG("End of line encountered within single quotes");
				_single_quoted = false;
			}
			else if (_double_quoted)
			{
				ERRMSG("End of line encountered within double quotes");
				_double_quoted = false;
			}
			return *_p++;
		}
		else if ((*_p == '/') && (*(_p+1) == '/'))
		{
			_p += 2;
			while (*_p && (*_p != '\n'))
				_p++;
			gotSpace = true;
		}
		else if ((*_p == '/') && (*(_p+1) == '*'))
		{
			_p += 2;
			while (*_p && ((*_p != '*') || (*(_p+1) != '/')))
				_p++;
			if (*_p)
				_p += 2;
			gotSpace = true;
		}
		else if (isspace(*_p))
		{
			_p++;
			gotSpace = true;
		}
		else if (gotSpace && (_return_count > 1))
			return ' ';
		else
			return *_p++;
	}
	return -1;
}
