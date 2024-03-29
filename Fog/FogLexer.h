#ifndef FLEX_HEADER_FogLexer_h
#define FLEX_HEADER_FogLexer_h
#define YY_FogLexer_CHAR char
/* A lexical scanner header generated by flex */
/* MODIFIED FOR C++ CLASS BY Alain Coetmeur: coetmeur(at)icdc.fr */
/* Note that (at) mean the 'at' symbol that I cannot write */
/* because it is expanded to the class name */
/* made at Informatique-CDC, Research&development department */
/* company from the Caisse Des Depots et Consignations */


/*********************************************/
/* SYSTEM dependent declaration, includes... */
/*********************************************/
/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
	#ifndef __cplusplus
		#define __cplusplus
	#endif
#endif
#ifdef __cplusplus
	#ifndef YY_USE_PROTOS
		#define YY_USE_PROTOS
	#endif
	#ifndef YY_USE_CLASS
		#define YY_USE_CLASS
	#endif
#else	/* ! __cplusplus */
	#ifdef __STDC__
		#ifdef __GNUC__
		#else
		#endif	/* __GNUC__ */
		#ifndef YY_USE_PROTOS
			#define YY_USE_PROTOS
		#endif
	#endif	/* __STDC__ */
#endif	/* ! __cplusplus */
/*********************************************/
/* COMPILER DEPENDENT   MACROS               */
/*********************************************/
/* use prototypes in function declarations */
#ifndef YY_PROTO
	#ifdef YY_USE_PROTOS
		#define YY_PROTO(proto) proto
	#else
		#define YY_PROTO(proto) ()
	#endif
#endif
#include <stdio.h>




/* % here is the declaration from section1 %header{  */
#define YY_FogLexer_INHERIT  : public FogLexerContext
#define YY_FogLexer_CONSTRUCTOR_PARAM  \
	std::istream& inputStream, \
	const FogSourceFile& sourceFile, \
	std::ostream *outputStream
#define YY_FogLexer_CONSTRUCTOR_INIT  : \
	FogLexerContext(sourceFile), \
	_state(&initial_state), \
	_pre_comment_state(&initial_state), \
	_pre_off_state(&initial_state)
#define YY_FogLexer_CONSTRUCTOR_CODE  yyin = &inputStream; yyout = outputStream;
#define YY_FogLexer_ECHO_NOCODE
#define YY_FogLexer_INPUT_NOCODE
#define YY_FogLexer_WRAP_NOCODE
#define YY_FogLexer_LEX_PARAM  FogTokenRef& yyValue
#define YY_FogLexer_LEX_RETURN  bool
#define YY_FogLexer_IOSTREAM
#define YY_FogLexer_MEMBERS  \
	private: \
	typedef FogLexerContext Super; \
	TYPEDECL_SINGLE(FogLexer, Super) \
	const FogLexerState *_state; \
	const FogLexerState *_pre_comment_state; \
	const FogLexerState *_pre_off_state; \
	private: \
	static const FogLexerState comment_state; \
	static const FogLexerState hash_state; \
	static const FogLexerState include_state; \
	static const FogLexerState initial_state; \
	static const FogLexerState off_state; \
	static const FogLexerState soak_state; \
	public: \
	static size_t _tokens; \
	static size_t _body_tokens; \
	private: \
	size_t columns(const char *aString, size_t aSize); \
	void got_character(FogTokenRef& yyValue, const char *someText, size_t aLength); \
	void got_string(FogTokenRef& yyValue, const char *aString, size_t aSize, bool isPrefixed); \
	const char *state_name() const; \
	const char *text() const { return (const char *)yytext; } \
	protected: \
	const FogLexerState& begin(const FogLexerState& aState); \
	const FogLexerState& begin_include() { return begin(include_state); } \
	const FogLexerState& begin_soak() { return begin(soak_state); } \
	const char *get(FogTokenContext& tokenContext); \
	void include_file(const FogSourceFile& sourceFile, const FogUtility& aUtility); \
	void set_truth(bool isEnabled); \
	public: \
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const; \
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const; \
	virtual std::ostream& print_this(std::ostream& s) const; \
	const FogLexerState& state() const { return *_state; } \
	public: \
	static std::ostream& print_statistics(std::ostream& s);

#ifdef YY_USE_CLASS
	#ifdef YY_FogLexer_IOSTREAM
		#include <iostream>
		#define YY_FogLexer_IFILE std::istream
		#define YY_FogLexer_OFILE std::ostream
		#define YY_FogLexer_ERRFILE std::cerr
		
		#ifndef YY_FogLexer_IFILE_DEFAULT
			#define YY_FogLexer_IFILE_DEFAULT &std::cin
		#endif
		
		#ifndef YY_FogLexer_OFILE_DEFAULT
			#define YY_FogLexer_OFILE_DEFAULT &std::cout
		#endif
		
	#endif
#endif

#ifndef YY_FogLexer_IFILE
	#define YY_FogLexer_IFILE FILE
#endif

#ifndef YY_FogLexer_OFILE
	#define YY_FogLexer_OFILE FILE
#endif

#ifndef YY_FogLexer_ERRFILE
	#define YY_FogLexer_ERRFILE stderr
#endif

#ifndef YY_FogLexer_IFILE_DEFAULT
	#define YY_FogLexer_IFILE_DEFAULT stdin
#endif

#ifndef YY_FogLexer_OFILE_DEFAULT
	#define YY_FogLexer_OFILE_DEFAULT stdout
#endif




#ifndef YY_FogLexer_TEXT
	#define YY_FogLexer_TEXT yytext
#endif
#ifndef YY_FogLexer_LENG
	#define YY_FogLexer_LENG yyleng
#endif
#ifndef YY_FogLexer_IN
	#define YY_FogLexer_IN yyin
#endif
#ifndef YY_FogLexer_OUT
	#define YY_FogLexer_OUT yyout
#endif

#ifndef YY_FogLexer_LEX_RETURN
	#define YY_FogLexer_LEX_RETURN int
#else
	#ifndef YY_FogLexer_LEX_DEFINED
		#define YY_FogLexer_LEX_DEFINED
	#endif
#endif

#ifndef YY_FogLexer_LEX
	#define YY_FogLexer_LEX yylex
#else
	#ifndef YY_FogLexer_LEX_DEFINED
		#define YY_FogLexer_LEX_DEFINED
	#endif
#endif

#ifndef YY_FogLexer_LEX_PARAM
	#ifndef YY_USE_PROTOS
		#define YY_FogLexer_LEX_PARAM
	#else
		#define YY_FogLexer_LEX_PARAM void
	#endif
#else
	#ifndef YY_FogLexer_LEX_DEFINED
		#define YY_FogLexer_LEX_DEFINED
	#endif
#endif

#ifndef YY_FogLexer_LEX_PARAM_DEF
	#define YY_FogLexer_LEX_PARAM_DEF
#else
	#ifndef YY_FogLexer_LEX_DEFINED
		#define YY_FogLexer_LEX_DEFINED
	#endif
#endif

#ifndef YY_FogLexer_RESTART
	#define YY_FogLexer_RESTART yyrestart
#endif
#ifndef YY_FogLexer_SWITCH_TO_BUFFER
	#define YY_FogLexer_SWITCH_TO_BUFFER yy_switch_to_buffer
#endif
#ifndef YY_FogLexer_LOAD_BUFFER_STATE
	#define YY_FogLexer_LOAD_BUFFER_STATE yy_load_buffer_state
#endif

#ifndef YY_FogLexer_CREATE_BUFFER
	#define YY_FogLexer_CREATE_BUFFER yy_create_buffer
	#ifndef YY_USE_CLASS
		#ifndef yy_new_buffer
			#define yy_new_buffer yy_create_buffer
		#endif
	#endif
#endif
#ifndef YY_FogLexer_DELETE_BUFFER
	#define YY_FogLexer_DELETE_BUFFER yy_delete_buffer
#endif
#ifndef YY_FogLexer_INIT_BUFFER
	#define YY_FogLexer_INIT_BUFFER yy_init_buffer
#endif



#ifdef YY_FogLexer_FLEX_DEBUG
	#ifndef YY_FogLexer_DEBUG
		#define YY_FogLexer_DEBUG 1
	#endif
#else
	#ifndef YY_FogLexer_DEBUG
		#define YY_FogLexer_DEBUG 0
	#endif
#endif

#if YY_FogLexer_DEBUG != 0
	#ifndef YY_FogLexer_DEBUG_FLAG
		#define YY_FogLexer_DEBUG_FLAG yy_flex_debug
	#endif
	#ifndef YY_FogLexer_DEBUG_INIT
		#define YY_FogLexer_DEBUG_INIT 1
	#endif
#endif




#ifndef YY_USE_CLASS
#ifndef YY_FogLexer_CURRENT_BUFFER
	#define YY_FogLexer_CURRENT_BUFFER yy_current_buffer
#endif
typedef struct yy_buffer_state* YY_BUFFER_STATE;

extern void YY_FogLexer_RESTART YY_PROTO(( YY_FogLexer_IFILE* input_file ));
extern void YY_FogLexer_SWITCH_TO_BUFFER YY_PROTO(( YY_BUFFER_STATE new_buffer ));
extern void YY_FogLexer_LOAD_BUFFER_STATE YY_PROTO(( void ));
extern YY_BUFFER_STATE YY_FogLexer_CREATE_BUFFER YY_PROTO(( YY_FogLexer_IFILE* file, int size ));
extern void YY_FogLexer_DELETE_BUFFER YY_PROTO(( YY_BUFFER_STATE b ));
extern void YY_FogLexer_INIT_BUFFER YY_PROTO(( YY_BUFFER_STATE b, YY_FogLexer_IFILE* file ));

#if YY_FogLexer_DEBUG != 0
	extern int YY_FogLexer_DEBUG_FLAG ;
#endif
extern YY_FogLexer_CHAR*  YY_FogLexer_TEXT;
extern int YY_FogLexer_LENG;
extern YY_FogLexer_IFILE* YY_FogLexer_IN;
extern YY_FogLexer_OFILE* YY_FogLexer_OUT;
#ifdef YY_FogLexer_LEX_DEFINED
	extern YY_FogLexer_LEX_RETURN YY_FogLexer_LEX ( YY_FogLexer_LEX_PARAM )
	YY_FogLexer_LEX_PARAM_DEF
#else
	#ifndef YY_DECL
		extern YY_FogLexer_LEX_RETURN YY_FogLexer_LEX ( YY_FogLexer_LEX_PARAM )
		YY_FogLexer_LEX_PARAM_DEF
	#else
		/* no declaration if oldstyle flex */
	#endif
#endif
#else

#ifndef YY_FogLexer_CURRENT_BUFFER
	#define YY_FogLexer_CURRENT_BUFFER YY_CURRENT_BUFFER
#endif
#ifndef YY_FogLexer_CLASS
	#define YY_FogLexer_CLASS FogLexer
#endif
#ifndef YY_FogLexer_ECHO
	#define YY_FogLexer_ECHO yy_echo
#endif
#ifdef YY_FogLexer_ECHO_PURE
	#define YY_FogLexer_ECHO_NOCODE
#endif

#ifndef YY_FogLexer_ECHO_CODE
	#ifndef YY_FogLexer_IOSTREAM
		#define YY_FogLexer_ECHO_CODE fwrite( (char *) YY_FogLexer_TEXT, YY_FogLexer_LENG, 1, YY_FogLexer_OUT );
	#else
		#define YY_FogLexer_ECHO_CODE (YY_FogLexer_OUT->write( (char *) YY_FogLexer_TEXT, YY_FogLexer_LENG));
	#endif
#endif

#ifndef YY_FogLexer_INPUT
	#define YY_FogLexer_INPUT yy_input
#endif
#ifdef YY_FogLexer_INPUT_PURE
	#define YY_FogLexer_INPUT_NOCODE
#endif

#ifndef YY_FogLexer_INPUT_CODE
	#ifndef YY_FogLexer_IOSTREAM
		#define YY_FogLexer_INPUT_CODE return result= fread(  buffer, 1,max_size,YY_FogLexer_IN );
	#else
		#define YY_FogLexer_INPUT_CODE if(YY_FogLexer_IN->eof())  result=0;else {YY_FogLexer_IN->read(buffer,max_size);result=YY_FogLexer_IN->gcount();YY_FogLexer_IN->clear(YY_FogLexer_IN->rdstate()&(~std::ios::failbit));if(YY_FogLexer_IN->bad()) result= -1;} return result;
	#endif
#endif

#ifdef YY_FogLexer_FATAL_ERROR_PURE
	#define YY_FogLexer_FATAL_ERRO_NOCODE
#endif
#ifndef YY_FogLexer_FATAL_ERROR
	#define YY_FogLexer_FATAL_ERROR yy_fatal_error
#endif

#ifndef YY_FogLexer_FATAL_ERROR_CODE
	#ifndef YY_FogLexer_IOSTREAM
		#define YY_FogLexer_FATAL_ERROR_CODE fputs( msg, YY_FogLexer_ERRFILE );putc( '\n', YY_FogLexer_ERRFILE );exit( 1 );
	#else
		#define YY_FogLexer_FATAL_ERROR_CODE YY_FogLexer_ERRFILE<< msg << std::endl;exit( 1 );
	#endif
#endif

#ifndef YY_FogLexer_WRAP
	#define YY_FogLexer_WRAP yy_wrap
#endif
#ifdef YY_FogLexer_WRAP_PURE
	#define YY_FogLexer_WRAP_NOCODE
#endif
#ifndef YY_FogLexer_WRAP_CODE
	#define YY_FogLexer_WRAP_CODE return 1;
#endif


#ifndef YY_FogLexer_INHERIT
	#define YY_FogLexer_INHERIT
#endif
#ifndef YY_FogLexer_MEMBERS
	#define YY_FogLexer_MEMBERS
#endif
#ifndef YY_FogLexer_CONSTRUCTOR_PARAM
	#define YY_FogLexer_CONSTRUCTOR_PARAM
#endif
#ifndef YY_FogLexer_CONSTRUCTOR_CODE
	#define YY_FogLexer_CONSTRUCTOR_CODE
#endif
#ifndef YY_FogLexer_CONSTRUCTOR_INIT
	#define YY_FogLexer_CONSTRUCTOR_INIT
#endif
#ifndef YY_FogLexer_DESTRUCTOR_CODE
	#define YY_FogLexer_DESTRUCTOR_CODE
#endif
typedef struct yy_buffer_state* YY_BUFFER_STATE;

class YY_FogLexer_CLASS YY_FogLexer_INHERIT {
private:/* data */
	YY_FogLexer_CHAR*  yy_c_buf_p;
	YY_FogLexer_CHAR  yy_hold_char;
	int yy_n_chars;
	int yy_init;
	int yy_start;
	int yy_did_buffer_switch_on_eof;
private: /* functions */
	void yy_initialize();
	int input();
	int yyinput() {return input();};
	int yy_get_next_buffer();
	void yyunput( YY_FogLexer_CHAR  c, YY_FogLexer_CHAR*  buf_ptr );
	/* use long instead of yy_state_type because it is undef */
	long yy_get_previous_state_ ( void );
	long yy_try_NUL_trans_  ( long current_state_ );
protected:/* non virtual */
	YY_BUFFER_STATE YY_FogLexer_CURRENT_BUFFER;
	void YY_FogLexer_RESTART ( YY_FogLexer_IFILE* input_file );
	void YY_FogLexer_SWITCH_TO_BUFFER( YY_BUFFER_STATE new_buffer );
	void YY_FogLexer_LOAD_BUFFER_STATE( void );
	YY_BUFFER_STATE YY_FogLexer_CREATE_BUFFER( YY_FogLexer_IFILE* file, int size );
	void YY_FogLexer_DELETE_BUFFER( YY_BUFFER_STATE b );
	void YY_FogLexer_INIT_BUFFER( YY_BUFFER_STATE b, YY_FogLexer_IFILE* file );
protected: /* virtual */
	virtual void YY_FogLexer_ECHO()
#ifdef YY_FogLexer_ECHO_PURE
	    = 0
#endif
	      ;
	virtual int  YY_FogLexer_INPUT(char*  buf, int& result, int max_size)
#ifdef YY_FogLexer_INPUT_PURE
	    = 0
#endif
	      ;
	virtual void  YY_FogLexer_FATAL_ERROR(const char* msg)
#ifdef YY_FogLexer_FATAL_ERROR_PURE
	    = 0
#endif
	      ;
	virtual int  YY_FogLexer_WRAP()
#ifdef YY_FogLexer_WRAP_PURE
	    = 0
#endif
	      ;
public:
	YY_FogLexer_CHAR*  YY_FogLexer_TEXT;
	int YY_FogLexer_LENG;
	YY_FogLexer_IFILE* YY_FogLexer_IN;
	YY_FogLexer_OFILE* YY_FogLexer_OUT;
	YY_FogLexer_LEX_RETURN YY_FogLexer_LEX ( YY_FogLexer_LEX_PARAM);
	YY_FogLexer_CLASS(YY_FogLexer_CONSTRUCTOR_PARAM) ;
	virtual ~YY_FogLexer_CLASS() ;
#if YY_FogLexer_DEBUG != 0
	int YY_FogLexer_DEBUG_FLAG;
#endif
public: /* added members */
	YY_FogLexer_MEMBERS
};
#endif



/* declaration of externs for public use of yylex scanner */

/* % here is the declaration from section2 %header{ */
#endif

/* end of generated header */
