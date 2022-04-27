typedef struct
{
  char * version;
  int assets_ignore;
  char * username;
} CMD_ARG;

CMD_ARG parse_arguments(int argc, char * argv[]);
