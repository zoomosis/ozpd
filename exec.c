/****************************************************************************/
/*                                                                          */
/* name: EXEC                                                               */
/*                                                                          */
/* vers: 1.00                                                               */
/*                                                                          */
/* auth: Paul Markham (3:711/934.1@fidonet)                                 */
/*                                                                          */
/* date: May 1993                                                           */
/*                                                                          */
/* func: This program executes another program at a specified priority.     */
/*       Usage: exec PTRY_CLASS PRTY_DELTA PROGRAM PARMS                    */
/*       Where:                                                             */
/*                                                                          */
/*       PRTY_CLASS = priority class from 0 to 4 (0 leaves priority class   */
/*                    unchanged).                                           */
/*                                                                          */
/*       PRTY_DELTA = change in priority level from -31 to +31              */
/*                                                                          */
/*       PROGRAM    = program name (with .EXE extension)                    */
/*                                                                          */
/*       PARMS      = parms to the program to be executed                   */
/*                                                                          */
/*                                                                          */
/*       eg. EXEC 2 31 TEST.EXE -a -b -c                                    */
/*                                                                          */
/*                                                                          */
/*       This program is released to the Public Domain.                     */
/****************************************************************************/

#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_NOPMAPI
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
	{
	int			i, parm_size;
	char		*exec_parm, *pgm_parm, err_obj[100];
	ULONG 		prty_class;
	long 		prty_delta;
	APIRET 		rc;
	RESULTCODES results;

	if (argc < 4)
		{
		fprintf(stderr, "Format: exec PRTY_CLASS PRTY_DELTA PROGRAM PARMS\n");
		exit(EXIT_FAILURE);
		}

	prty_class = atoi(argv[1]);
	prty_delta = atoi(argv[2]);

	rc = DosSetPriority(PRTYS_PROCESSTREE, prty_class, prty_delta, 0);
	if (rc)
		{
		fprintf(stderr, "rc %ld from DosSetPriority\n", rc);
		exit(EXIT_FAILURE);
		}


/* allow for length of program name plus a NUL byte
   and an extra NUL at end of parm   */

	parm_size = strlen(argv[3]) + 2;
	for (i = 4; i < argc; i++)
		parm_size += strlen(argv[i]) + 1; /* allow for parm size plus space */

	if ((exec_parm = malloc(parm_size)) == NULL)
		{
		fprintf(stderr, "Can't allocate %ld bytes\n", parm_size);
		exit(EXIT_FAILURE);
		}
	memset(exec_parm, '\0', parm_size);
	strcpy(exec_parm, argv[3]);
	pgm_parm = exec_parm + strlen(argv[3]) + 1;
	for (i = 4; i <argc; i++)
		{
		strcat(pgm_parm, argv[i]);
		if (i != argc - 1)
			strcat(pgm_parm, " ");
		}

	rc = DosExecPgm(err_obj, sizeof(err_obj), EXEC_SYNC,
		(PSZ)exec_parm,	NULL, &results, (PSZ)exec_parm);
	if (rc)
		{
		fprintf(stderr, "rc %ld from DosExecPgm\n", rc);
		exit(EXIT_FAILURE);
		}

	free(exec_parm);

	return(results.codeResult);
	}

