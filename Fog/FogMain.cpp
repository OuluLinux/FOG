//
// 	  Title:			Main program for Flexible Object Generator
//
// 	  File Name:		FogMain.cpp
//
// 	  Author:			E.D.Willink
// END
//

#include <Fog/FogIncludeAll.h>

bool debug_delete;
PrimText files;
PrimStringHandle global_name;
bool help;
PrimStringHandle log_file;
PrimStringHandle output_file;
bool quiet;
bool statistics;

PrimComParse& main_parser = PrimComParse::main_parser();
static PrimComToken_String tok_anon_prefix(main_parser, Fog::anon_prefix(),
		"anon_prefix", "Prefix for \"anonymous\" names (default is \"_anon_\").", *Fog::anon_prefix());
static PrimComToken_String tok_cd(main_parser, Fog::implementation_path(),
		"cd", "Emitted implementation file directory path.", *Fog::implementation_path());
static PrimComToken_Bool tok_comment_line_numbers(main_parser, Fog::comment_line_numbers(),
		"comment_line_numbers", "Enclose #line numbers as comments in emitted files.", Fog::comment_line_numbers());
static PrimComToken_String tok_cp(main_parser, Fog::implementation_prefix(),
		"cp", "Emitted implementation file prefix.", *Fog::implementation_prefix());
static PrimComToken_String tok_cs(main_parser, Fog::implementation_suffix(),
		"cs", "Emitted implementation file suffix.", *Fog::implementation_suffix());
static PrimComToken_String tok_ctd(main_parser, Fog::template_implementation_path(),
		"ctd", "Emitted template implementation file directory path.", *Fog::template_implementation_path());
static PrimComToken_String tok_ctp(main_parser, Fog::template_implementation_prefix(),
		"ctp", "Emitted template implementation file prefix.", *Fog::template_implementation_prefix());
static PrimComToken_String tok_cts(main_parser, Fog::template_implementation_suffix(),
		"cts", "Emitted template implementation file suffix.", *Fog::template_implementation_suffix());
static PrimComToken_Bool tok_cplusplus(main_parser, Fog::cplusplus(),
		"c++", "Behave more like a C++ compiler.", Fog::cplusplus());
static PrimComToken_Strings tok_D(main_parser, Fog::define_strings(),
		"D", "Define a preprocessor macro value.");
static PrimComToken_String tok_extern_prefix(main_parser, Fog::extern_prefix(),
		"extern_prefix", "Prefix for \"extern\" linkage names (default is \"_extern_\").", *Fog::extern_prefix());
static PrimComToken_Bool tok_force(main_parser, Fog::force(),
		"f", "Force file emission (bypass redundancy comparisons).");
static PrimComToken_String tok_global(main_parser, global_name,
		"global", "(File)name of the global namespace.", *global_name);
static PrimComToken_String tok_hd(main_parser, Fog::interface_path(),
		"hd", "Emitted interface file directory path.", *Fog::interface_path());
static PrimComToken_Bool tok_help(main_parser, help,
		"help", "Display this usage help.", help);
static PrimComToken_String tok_hp(main_parser, Fog::interface_prefix(),
		"hp", "Emitted interface file prefix.", *Fog::interface_prefix());
static PrimComToken_String tok_hs(main_parser, Fog::interface_suffix(),
		"hs", "Emitted interface file suffix.", *Fog::interface_suffix());
static PrimComToken_String tok_htd(main_parser, Fog::template_interface_path(),
		"htd", "Emitted template interface file directory path.", *Fog::template_interface_path());
static PrimComToken_String tok_htp(main_parser, Fog::template_interface_prefix(),
		"htp", "Emitted template interface file prefix.", *Fog::template_interface_prefix());
static PrimComToken_String tok_hts(main_parser, Fog::template_interface_suffix(),
		"hts", "Emitted template interface file suffix.", *Fog::template_interface_suffix());
static PrimComToken_Strings tok_i(main_parser, FogFileManager::include_paths(),
		"i", "Source include file path(s) (defaults to current directory).");
static PrimComToken_Strings tok_I(main_parser, FogFileManager::include_paths(),
		"I", "Source include file path(s) (defaults to current directory).");
static PrimComToken_String tok_log_output(main_parser, log_file,
		"log", "Log file name (duplicates standard error).");
static PrimComToken_Bool tok_long_long_type(main_parser, Fog::long_long_type(),
		"long_long_type", "Treat (unsigned) long long as built-in (Sun C++ language extension).", Fog::long_long_type());
static PrimComToken_Int tok_max_errors(main_parser, PrimNotifier::maximum_error_count(),
		"max_errors", "maximum number of errors before program termination (100).", 100);
static PrimComToken_Int tok_max_warnings(main_parser, PrimNotifier::maximum_warning_count(),
		"max_warnings", "maximum number of warnings before program termination (0).", 0);
static PrimComToken_Int tok_mbc(main_parser, Fog::multi_byte_count(),
		"mbc", "Bytes in a multi-byte character (4).", Fog::multi_byte_count());
static PrimComToken_Bool tok_microsoft_extensions(main_parser, Fog::microsoft_extensions(),
		"microsoft_extensions", "Support Microsoft extensions.", Fog::microsoft_extensions());
static PrimComToken_Bool tok_notify_create(main_parser, Fog::notify_create(),
		"nc", "Notify emitted file names that are created.");
static PrimComToken_Bool tok_notify_equivalence(main_parser, Fog::notify_equivalence(),
		"ne", "Notify emitted file names that are suppressed through equivalence.");
static PrimComToken_Bool tok_nobanner(main_parser, Fog::nobanner(),
		"nobanner", "Suppress emitted comment banners (to ease regression testing).", Fog::nobanner());
static PrimComToken_Bool tok_no_access(main_parser, Fog::no_access(),
		"no_access", "Diagnose access declarations (ARM C++ compatibility).", Fog::no_access());
static PrimComToken_Bool tok_no_bool_type(main_parser, Fog::no_bool_type(),
		"no_bool_type", "Do not treat bool as built-in (ARM C++ compatibility).", Fog::no_bool_type());
static PrimComToken_Bool tok_no_line_numbers(main_parser, Fog::no_line_numbers(),
		"no_line_numbers", "Omit #line numbers from emitted files.", Fog::no_line_numbers());
static PrimComToken_Bool tok_no_namespace(main_parser, Fog::no_namespace(),
		"no_namespace", "Treat namespace as a synonym for class.", Fog::no_namespace());
static PrimComToken_Bool tok_no_specialisation_prefix(main_parser, Fog::no_specialisation_prefix(),
		"no_specialisation_prefix",
		"Do not require \"template<>\" for specialisation (Pre-Ansi C++ compatibility).",
		Fog::no_specialisation_prefix());
static PrimComToken_Bool tok_no_using(main_parser, Fog::no_using(),
		"no_using", "Emit using declarations as access declarations (ARM C++ compatibility).", Fog::no_using());
static PrimComToken_Bool tok_no_wchar_t_type(main_parser, Fog::no_wchar_t_type(),
		"no_wchar_t_type", "Do not treat wchar_t as built-in (ARM C++ compatibility).", Fog::no_wchar_t_type());
static PrimComToken_String tok_o(main_parser, output_file,
		"o", "File name for make dependencies between source and emitted files.");
static PrimComToken_Bool tok_quiet(main_parser, quiet,
		"q", "Suppress the program identification message.");
static PrimComToken_Bool tok_readonly(main_parser, Fog::readonly(),
		"readonly", "Just read source files to gather token count statistics.", Fog::readonly());
static PrimComToken_Bool tok_statistics(main_parser, statistics,
		"statistics", "Emit program performance statistics.");
static PrimComToken_Int tok_tabs(main_parser, Fog::tab_size(),
		"t", "Columns per tab in source files (default 8).", Fog::tab_size());
static PrimComToken_String tok_template_parameter_prefix(main_parser, Fog::template_parameter_prefix(),
		"template_parameter_prefix", "Prefix for normalised template parameter names (default is \"_\").", *Fog::template_parameter_prefix());
static PrimComToken_Bool tok_unnest(main_parser, Fog::unnest(),
		"unnest", "Emit nested classes after rather than within enclosing class.", Fog::unnest());
static PrimComToken_Bool tok_za(main_parser, Fog::debug_activity(),
		"za", "Display changes to the activity status of declarations.", Fog::debug_activity());
// static PrimComToken_Bool tok_zc(main_parser, PrimCheckLock::debug_check(),
// 	"zc", "Perform superficial regular internal consistency checks.", PrimCheckLock::debug_check());
static PrimComToken_Bool tok_zd(main_parser, debug_delete,
		"zd", "Delete all objects rigorously on exit (for purify testing).");
// static PrimComToken_Bool tok_zf(main_parser, PrimCheckLock::debug_full_check(),
// 	"zf", "Perform full regular internal consistency checks.", PrimCheckLock::debug_full_check());
static PrimComToken_Bool tok_zf(main_parser, Fog::debug_file(),
		"zf", "Display file name as each entity is (re)positioned.", Fog::debug_file());
static PrimComToken_Bool tok_zi(main_parser, Fog::debug_input(),
		"zi", "Display each input and macro line.", Fog::debug_input());
static PrimComToken_Bool tok_zl(main_parser, Fog::debug_lex(),
		"zl", "Display the behaviour of the lexer.", Fog::debug_lex());
static PrimComToken_Bool tok_zp(main_parser, Fog::debug_purity(),
		"zp", "Display changes to the purity status.", Fog::debug_purity());
static PrimComToken_Bool tok_zs(main_parser, Fog::debug_scope(),
		"zs", "Display changes to the default parser scope.", Fog::debug_scope());
static PrimComToken_Bool tok_zu(main_parser, Fog::debug_utility(),
		"zu", "Display changes to the composed entity and parser default utility.", Fog::debug_utility());
static PrimComToken_Bool tok_zt(main_parser, Fog::debug_lex2main(),
		"zt", "Display each token passed between lexer and main parser.", Fog::debug_lex2main());
static PrimComToken_Bool tok_zx(main_parser, PrimPrint::print_address(),
		"zx", "Display full hex address of each object in diagnostics.", PrimPrint::print_address());
static PrimComToken_Bool tok_zy(main_parser, Fog::debug_yacc(),
		"zy", "Display YACC parser progress.", Fog::debug_yacc());
static PrimComToken_Bool tok_zz(main_parser, Fog::debug_repeat(),
		"zz", "Repeat certain failed invocations after generating an error to aid debug.", Fog::debug_repeat());
static PrimComToken_Bool tok_z2h(main_parser, Fog::debug_lex2hash(),
		"z2h", "Display each token passed between lexer and hash parser.", Fog::debug_lex2hash());
static PrimComToken_Bool tok_z2l(main_parser, Fog::debug_lex2locate(),
		"z2l", "Display each token passed between lexer and locate parser.", Fog::debug_lex2locate());
static PrimComToken_Bool tok_z2m(main_parser, Fog::debug_lex2main(),
		"z2m", "Display each token passed between lexer and main parser.", Fog::debug_lex2main());
static PrimComToken_Bool tok_z2r(main_parser, Fog::debug_lex2replace(),
		"z2r", "Display each token passed between lexer and replace parser.", Fog::debug_lex2replace());
static PrimComToken_Bool tok_z2s(main_parser, Fog::debug_lex2substitute(),
		"z2s", "Display each token passed between lexer and substitute parser.", Fog::debug_lex2substitute());
static PrimComToken_Files tok_file(main_parser, files,
		"<file>", "Source file name(s), may be - for standard input.");
static PrimComToken_Bool tok_hm(main_parser, Fog::emit_preheader(),
		"hm", "Add PREHEADER macro call in header files.", Fog::emit_preheader());
static PrimComToken_Bool tok_uc(main_parser, Fog::use_test_breakers(),
		"uc", "Use changes, which breaks tests compatibility.", Fog::use_test_breakers());


void PrintAST(const FogRaw& tk, int indent=0) {
	std::strstream str;
	tk.print_depth(str);
	for(int i = 0; i < indent; i++)
		printf("\t");
	printf("%s\n", str.str());
	const FogRaws* raws = dynamic_cast<const FogRaws*>(&tk);
	if (raws) {
		for (FogRawConstListOfRefToConstIterator p(raws->raws()); p; ++p) {
			PrintAST(*p, indent+1);
		}
	}
}

//
// 	The program parses the command line, reads the grammar, optimises it, and generates interface and
// 	implementation files to define a code generator.
//
int main(int argc, char *argv[]) {
	if (!main_parser.parse_args(argc, argv) || help) {
		main_parser.print_usage(std::cout);
		return EXIT_FAILURE;
	}
	
	if (PrimCheckLock::debug_full_check())
		PrimCheckLock::debug_check() = true;
		
	if (log_file) {
		PrimNotifier::set_global_notice_log_file(log_file.str());
		PrimNotifier::set_global_notice_stream(0);
	}
	
	if (Fog::no_bool_type())
		FogTokenType::set_no_bool_type();
		
	if (Fog::no_wchar_t_type())
		FogTokenType::set_no_wchar_t_type();
		
	PrimOstrstream programName;
	
	programName << "fog " << FOG_MAJOR << '.' << FOG_MINOR << '.' << FOG_LETTER
	<< " of " << __TIME__ << ' ' << __DATE__ << std::ends;
	
	CONDMSG(!quiet, "Flexible Object Generator " << programName);
	
	FogRootRef aFog(global_name ? new FogRoot(*PrimIdHandle(global_name.str())) : new FogRoot(PrimId::null()),
					FogRootRef::ADOPT);
	                
	aFog->set_program(programName.string());
	
	FogReader& aReader = aFog->reader();
	
	if (Fog::cplusplus())
		aReader.set_utility_mode(FogUtility::frozen_utility());
	else
		aReader.set_utility_mode(FogUtility::emit_utility());
		
	for (PrimTextIterator p(files); p; p++) {
		const char *fileArg = (strcmp(p->str(), "-") == 0) ? 0 : p->str();
		
		if (fileArg)
			aReader.add_source(*PrimIdHandle(fileArg));
			
		PrimIfstreamMonitor lexMonitor("Reading", *PrimStringHandle(fileArg), ".", &std::cin, "<cin>");
		
		FogLexerTower aLexer(aReader);
		
		const PrimIdHandle anId(lexMonitor.file_name().str());
		
		const FogSourceFile *sourceFile = aReader.find_source_file(*anId, FogSourceFileType::top_input());
		
		if (sourceFile) {
			aReader.set_utility_mode(aReader.utility_mode(), sourceFile);
			aLexer.push_stream(aReader, lexMonitor.s(), *sourceFile);
			FogParser aParser(aLexer, FogToken::mutable_null(), "main", 0);
			int parseError = aParser.yyparse();
			FogToken *theToken = aParser.result();
			
			if (!parseError && theToken) {
				FogRaw *aDeclaration = theToken->is_raw();
				
				if (0) {
					FogRaws* r = dynamic_cast<FogRaws*>(theToken);
					PrintAST(*r);
				}
				
				//return 0;
				//if (aDeclaration)
				//	aParser.compile_declaration(&FogUtility::emit_utility(), aDeclaration);
			}
		}
	}
	
	
	aFog->compile();
	
	aFog->validate();
	aFog->emit_files();
	
	//PrintAST(*aFog);
	
	if (output_file)
		aFog->emit_dependencies(*output_file);
		
	if (debug_delete) {
		aFog->destroy();    //  Need to break self-reference in root etc.
		FogParser::terminate();
	}
	
	if (statistics)
		FogLexer::print_statistics(PrimNotifier::global_notice_stream_or_cerr());
		
	if (PrimNotifier::error_count() > 0)
		DIAMSG(PrimNotifier::error_count() << " errors reported.");
		
	if (PrimNotifier::warning_count() > 0)
		DIAMSG(PrimNotifier::warning_count() << " warnings reported.");
		
	return PrimNotifier::error_count() > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

// #include <stdio.h>
// #include <stdlib.h>

#if REALLY_DESPERATE
void operator delete(void *p) {
	if (p) {
		unsigned char *q = (unsigned char *)p - 256;
		printf("Deleting %p\n", q);
		size_t aSize = *(size_t *)q;
		
		if (aSize != *(size_t *)((const unsigned char *)q+sizeof(size_t)))
			printf("memory corruption at %p or %p\n", q, (const unsigned char *)q+sizeof(size_t));
		else {
			const unsigned char *r = (const unsigned char *)((const unsigned char *)q+2*sizeof(size_t));
			
			for (int i = 256-2*sizeof(size_t); i--; r++)
				if (*r != 0xA5)
					printf("memory corruption at %p\n", r);
					
			r = (const unsigned char *)q + 256 + aSize;
			
			for (int j = 256; j--; r++)
				if (*r != 0xA5)
					printf("memory corruption at %p\n", r);
		}
		
		free(q);
	}
	
	else
		printf("Not-Deleting 0\n");
		
}

void *operator new(size_t aSize) {
	if (aSize <= 0)
		aSize = 1;
		
	void *p = malloc(aSize + 512);
	
	printf("Allocated 256 + %d + 256 bytes at %p\n", aSize, p);
	
	memset(p, 0xA5, aSize + 512);
	
	*(size_t *)p = aSize;
	
	*(size_t *)((unsigned char *)p+sizeof(size_t)) = aSize;
	
	return (char *)p + 256;
}
#endif
