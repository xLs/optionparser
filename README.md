# Header only Command line option parser
Command Line Option Parser

example of option struct (probably familiar from opts)

```c++
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
```
help output:
```
Usage:
        USAGE: example [options]

 Options:
        [opt]   --help           Print usage and exit.
        [opt]   --debug, -s      Set optional debug mode.
        [opt]   --version, -v    Print version.
        [opt]   --number, -n     Set a number (must be numeric).
        [opt]   --string, -s     Set a string (any string).
        [opt]   --multi, -m      Add string to options.
        ------- Separator ------------------------------
        [req]   --required, -r   Set required option.
        [opt]   --flag, -f       Enable this option

Examples:
  example -h
  example -r "musthave" -d -v -n 123 -s "hello world" -m=test -m="test 2" -m "test 3"
  example
 ```
 
 example of parsing and getting arguments:
 ```c++
  if (!opts.Parse(&argv[1], argc))	// skip first argument (module path)
  {
    printf("* option error: %s\n", opts.error_msg());
    opts.print();
    return -1;
  }

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
```

output:
```
> build/Linux/option -r "musthave" -d -v -n 123 -s "hello world" -m=test -m="test 2" -m "test 3"
debug enabled
options version 0.99a
input number: 123
input string: hello world
input multi args:
        test
        test 2
        test 3
required input: musthave
```


use  optiont::ParseFile to override or load arguments from file:
```
opts.ParseFile("test.cmdline");
```

test.cmdline content:
```
#d 
-r required 
-m="an arg from file" 
-m "another arg from file"
```
the '#' - character removes this argument if it was previously parsed by option parser

the new output looks like:
```
> build/Linux/option -r "musthave" -d -v -n 123 -s "hello world" -m=test -m="test 2" -m "test 3"
options version 0.99a
input number: 123
input string: hello world
input multi args:
        test
        test 2
        test 3
        *an arg from file*
        *another arg from file*
required input: *required*
```
-d was removed and thus not 'debug enabled' output is visible,
-r was overriden and changed from "musthave" to "required"
-m added two new parameters.
