// **************************** RACAL Confidential ****************************
//
// 	  Title:			Main program for C++ preprocessor
//
// 	  File Name:		CcpMain.cxx
//
// 	  Author:			E.D.Willink
//
// 	  Description:
//
// 	  SCCS:				%W% %G%
//
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
//  EDW	1.2		Date:	 4-Sep-1997		Add test harness support
//  EDW	1.3		Date:	28-Apr-1998		Adjust to PrimStringHandle, use std streams
// END
//
#include <Ccp/CcpIncludeAll.h>

PrimComParse& main_parser = PrimComParse::main_parser();

bool debug_delete;
bool debug_objects;
PrimStringHandle c_prefix;
PrimStringHandle c_suffix;
PrimStringHandle h_prefix;
PrimStringHandle h_suffix;
PrimText include_paths;
PrimText input_files;
PrimStringHandle log_file;
PrimStringHandle t_prefix;
PrimStringHandle t_suffix;

static PrimComToken_String tok_c_prefix(main_parser, c_prefix,
		"cp", "implementation output file name prefix", "");
static PrimComToken_String tok_c_suffix(main_parser, c_suffix,
		"cs", "implementation output file name suffix", ".cxx");
static PrimComToken_String tok_h_prefix(main_parser, h_prefix,
		"hp", "interface output file name prefix", "");
static PrimComToken_String tok_h_suffix(main_parser, h_suffix,
		"hs", "interface output file name suffix", ".hxx");
static PrimComToken_Strings tok_include_paths(main_parser, CcpReader::include_paths(),
		"I", "Include file paths");
static PrimComToken_String tok_log_output(main_parser, log_file,
		"log", "Log file name");
static PrimComToken_Bool tok_quiet(main_parser, CcpReader::quiet(),
		"q", "suppress program identification");
static PrimComToken_String tok_t_prefix(main_parser, t_prefix,
		"tp", "test harness output file name prefix", "");
static PrimComToken_String tok_t_suffix(main_parser, t_suffix,
		"ts", "test harness output file name suffix", ".txx");
static PrimComToken_Files tok_input_files(main_parser, input_files,
		"<files>", "Composite cxx/hxx source files");
static PrimComToken_Bool tok_zc(main_parser, PrimCheckLock::debug_check(),
		"zc", "perform superficial regular consistency checks");
static PrimComToken_Bool tok_zd(main_parser, debug_delete,
		"zd", "delete all objects programmatically on exit");
static PrimComToken_Bool tok_zf(main_parser, PrimCheckLock::debug_full_check(),
		"zf", "perform full regular consistency checks");
static PrimComToken_Bool tok_zl(main_parser, CcpReader::debug_lex(),
		"zl", "activate lex token diagnostics");
static PrimComToken_Bool tok_zo(main_parser, debug_objects,
		"zo", "generate a print out of all objects");
static PrimComToken_Bool tok_zx(main_parser, PrimPrint::print_address(),
		"zx", "display full hex address in diagnostics");
static PrimComToken_Bool tok_zy(main_parser, CcpReader::debug_yacc(),
		"zy", "activate YACC diagnostics");
        
//
// 	The program parses the command line, reads the target filter, identifies candidate prime filters
// 	and then permutes them to produce a qunatised approximation.
//
int main(int argc, const char *const argv[]) {
	if (!main_parser.parse_args(argc, argv)) {
		main_parser.print_usage(std::cout);
		return (EXIT_FAILURE);
	}
	
	if (PrimCheckLock::debug_full_check())
		PrimCheckLock::debug_check() = true;
		
	std::ostream *lf = 0;
	
	if (log_file) {
#ifdef NEEDS_STD
		lf = new std::ofstream(log_file.str(), std::ios::out, ofstream_open_mode());
#else
		lf = new std::ofstream(log_file.str(), std::ios::out);
#endif
		
		if (!lf)
			ERRMSG("Unable to create memory for \"" << log_file << "\"");
		else
			if (!*lf)
				WRNMSG("Unable to open \"" << log_file << "\"");
			else
				PrimNotifier::set_global_notice_log(lf);
	}
	
	CONDMSG(!CcpReader::quiet(), "C++ preprocessor " << CCP_MAJOR << "." << CCP_MINOR << "." << CCP_LETTER << " of " << __TIME__ << " " << __DATE__);
	
	for (PrimTextIterator p(input_files); p; ++p) {
		const char *fileEnd = strrchr(p->str(), '.');
		
		if (!fileEnd)
			fileEnd = p->str() + strlen(p->str());
			
		const char *fileStart = fileEnd;
		
		while ((fileStart > p->str()) && (*(fileStart-1) != '/')  && (*(fileStart-1) != '\\'))
			fileStart--;
			
		PrimStringHandle fileName(fileStart, fileEnd - fileStart);
		
		std::ostrstream fs;
		
		for (const char *p1 = (*p)->str(); *p1; p1++)
			fs << char((*p1 == '\\') ? '/' : *p1);
		
		fs << std::ends;
		
		std::ostrstream hs;
		
		hs << h_prefix << fileName << h_suffix << std::ends;
		
		PrimAdoptedArray<char> hName(hs.str());
		
		std::ostrstream cs;
		
		cs << c_prefix << fileName << c_suffix << std::ends;
		
		PrimAdoptedArray<char> cName(cs.str());
		
		std::ostrstream ts;
		
		ts << t_prefix << fileName << t_suffix << std::ends;
		
		PrimAdoptedArray<char> tName(ts.str());
		
		const CcpReader aReader(*PrimStringHandle(fs.str()), hName.get(), cName.get(), tName.get(), lf);
	}
	
	if (lf)
		delete lf;
		
	return (PrimNotifier::error_count() > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
