/* Written by Joshua Marshall, 3:640/452.35@fidonet */
/* Released to the public domain */

#ifndef FSF
#define FSF 1

int find_string_format(char *filename,
                       void *srchstr);
                        
extern char xtras[100][80];
extern int xtrapos;
extern int in_comment, in_bracket, in_quote, in_quotes, touchem;
/**************************************************************
find_string_format searches for string srchstr in the file
filename, and looks inside comments if in_comment = 1, 
looks inside brackets if in_bracket = 1,
looks inside single quotes (') if in_quote = 1,
looks inside double quotes (") if in_quotes = 1.

To look outside, use 2.
To disable looking in/outside use 0.
**************************************************************/

void prname(char *name);

#endif
