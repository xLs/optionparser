#include "options/optionparser.h"

using namespace option;

enum  optionIndex {
	OPT_HELP, OPT_DEBUG, OPT_VERSION, OPT_NUM, OPT_STRING, OPT_MULTIARGS,
	OPT_REQ, OPT_SINGLE
};

const option::Descriptor usage[] =
{
	{ 0,  0, 0, option::Arg::Dummy, "USAGE: example [options]\n\n Options:" },
	{ OPT_HELP,			"h", "help", option::Arg::None,				"  --help           Print usage and exit." },
	{ OPT_DEBUG,		"d", "debug", option::Arg::None,			"  --debug, -s      Set optional debug mode." },
	{ OPT_VERSION,		"v", "version", option::Arg::None,			"  --version, -v    Print version." },
	{ OPT_NUM,			"n", "number", option::Arg::Numeric,		"  --number, -n     Set a number (must be numeric)." },
	{ OPT_STRING,		"s", "string", option::Arg::String,			"  --string, -s     Set a string (any string)." },
	{ OPT_MULTIARGS,	"m", "multi", option::Arg::String | option::Arg::Multiple,
																	"  --multi, -m      Add string to options." },
	{ 0,  0, 0, option::Arg::Dummy, "------- Separator ------------------------------" },
	{ OPT_REQ,	"r", "required", option::Arg::String | option::Arg::Required,
																	"  --required, -r   Set required option." },
	{ OPT_SINGLE,	"o", "option", option::Arg::None,				"  --flag, -f       Enable this option"},
	{ 0, 0, 0, option::Arg::Dummy,"\nExamples:\n"
	"  example -h \n"
	"  example -r \"musthave\" -d -v -n 123 -s \"hello world\" -m=test -m=\"test 2\" -m \"test 3\"\n"
	"  example \n" },
	{ 0, 0, 0, option::Arg::End, 0 }	// terminating
};


int main(int argc, const char** argv)
{
	option::Options opts((option::Descriptor*)usage);
	
	
	if (!opts.Parse(&argv[1], argc))	// skip first argument (module path)
	{
		
#ifdef _WIN32
		OutputDebugStringA(opts.error_msg());
		OutputDebugStringA(opts.cstr());

#endif
		printf("* option error: %s\n", opts.error_msg());
		
		opts.print();
		return -1;
	}

	// optionally load commandline overrides from file
	// opts.ParseFile("test.cmdline");


	if (opts[OPT_DEBUG])
	{
		printf("debug enabled\n");
	}

	if (opts[OPT_VERSION])
	{
		printf("options version 0.99a\n");
	}

	if (opts[OPT_NUM])
	{
		int num = 0;
		opts.GetArgument(OPT_NUM,num);
		printf("input number: %d\n", num);
	}

	if (opts[OPT_STRING])
	{
		printf("input string: %s\n", opts.GetValue(OPT_STRING));
	}

	if (opts[OPT_MULTIARGS])
	{
		auto arg = opts.GetArgument(OPT_MULTIARGS);
		printf("input multi args:\n");
		while (arg != nullptr)
		{
			printf("\t%s\n",arg->value);
			arg = arg->pNext;
		}
	}
	if (opts[OPT_REQ])
	{
		printf("required input: %s", opts.GetValue(OPT_REQ));
	}
	printf("\n");

}

#ifdef _WIN32

#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR lpCmdLine, int /*nCmdShow*/)
{
    int argc = 0;
    char** argv = option::CommandLineToArgvWin(nullptr, &argc);
    main(argc, (const char**)(argv));	// skipp first argument

    option::free(argv);
}
#endif