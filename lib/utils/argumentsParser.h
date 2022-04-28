typedef struct
{
  char * version;
  char * username;
  char * version_list;
  int assets_ignore;
} CmdArg;

CmdArg  parse_arguments(int argc, char * argv[]);
