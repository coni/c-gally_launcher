typedef struct
{
  char * version;
  int assets_ignore;
  char * username;
} CmdArg;

CmdArg  parse_arguments(int argc, char * argv[]);
