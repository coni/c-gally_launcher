#include "argumentsParser.h"
#include "system.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int _is_boolean(char * option, CMD_ARG *arguments, int dashc)
{
  if (dashc == 1)
  {
    if ((strcmp(option, "ai") == 0))
      arguments->assets_ignore = 1;
    else
      return 1;
  }
  else
  {
    if ((strcmp(option, "assets-ignore") == 0))
      arguments->assets_ignore = 1;
    else
      return 1;
  }
  return 0;
}

void _put_value(char * option, char * value, CMD_ARG *arguments, int dashc)
{
  if (dashc == 1)
  {
    if (strcmp(option, "v") == 0)
      arguments->version = value;
    else if (strcmp(option, "u") == 0)
      arguments->username = value;
    else
    {
      char error_stderr[18 + strlen(option)];
      strcpy(error_stderr, "invalid option: ");
      strcat(error_stderr, option);
      strcat(error_stderr, "\n");
      system_error(1, error_stderr);
    }
  }
  else
  {
    if (strcmp(option, "version") == 0)
      arguments->version = value;
    else if (strcmp(option, "username") == 0)
      arguments->username = value;
    else
    {
      char error_stderr[18 + strlen(option)];
      strcpy(error_stderr, "invalid option: ");
      strcat(error_stderr, option);
      strcat(error_stderr, "\n");
      system_error(1, error_stderr);
    }
  }
  
}

CMD_ARG parse_arguments(int argc, char * argv[])
{
  CMD_ARG args;
  char *currentToken = NULL;
  char *currentOption = malloc(sizeof(char *));
  char tmpChr[] = "a";
  char *currentOptionArgument = NULL;
  int c = 0;
  int dashc = 0;
  
  if (argc > 0)
  {
    for (int i = 1; i < argc; i++)
    {
      currentOption = realloc(currentOption, (strlen(argv[i])+1));

      if (c == 0)
      {
        for (int j = 0; j < strlen(argv[i]); j++)
        {
          if (argv[i][j] == '-')
          {
            if (strcmp(currentOption, "") == 0 && dashc < 2)
              dashc++;
            else
            {
              tmpChr[0] = argv[i][j];
              strcat(currentOption, tmpChr);
            }
          }
          else
          {
            tmpChr[0] = argv[i][j];
            strcat(currentOption, tmpChr);
          }
        }
        // When c == 0, argv[i] is the current option
        if (dashc == 0)
        {
          char error_stderr[18 + strlen(currentOption)];
          strcpy(error_stderr, "invalid option: ");
          strcat(error_stderr, currentOption);
          strcat(error_stderr, "\n");
          system_error(1, error_stderr);
        }
        c = _is_boolean(currentOption, &args, dashc);
        if (c == 0)
        {
          strcpy(currentOption ,"");
          dashc = 0;
        }
      }
      else
      {
        // When c == 0, argv[i] is the argument of the option
        _put_value(currentOption, argv[i], &args, dashc);
        strcpy(currentOption ,"");
        dashc = 0;
        c = 0;
      }
    }
  }
  return args;
}
