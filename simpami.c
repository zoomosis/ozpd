/*	Hello World program for Amiga		*/
/*	Written by Craig Delahoy		*/
/*	Released into the public domain		*/

#include	<intuition/intuition.h>
#include	<intuition/intuitionbase.h>
#include	<proto/intuition.h>

struct NewWindow new_p = 
	{120,50,400,80,-1,-1,CLOSEWINDOW,ACTIVATE|WINDOWCLOSE,NULL,NULL,
	"The first C program",NULL,NULL,400,80,400,80,WBENCHSCREEN};

struct IntuiText itext_p = {1,0,JAM1,150,35,NULL,"Hello World!",NULL};

struct IntuitionBase *IntuitionBase;
	
void	main()
{
	struct Window *window_p;

	if((IntuitionBase = (struct IntuitionBase *)
		OpenLibrary("intuition.library",0)) != NULL)
	{
		if((window_p = OpenWindow(&new_p)) != NULL)
		{
			PrintIText(window_p->RPort,&itext_p,0,0);
			WaitPort(window_p->UserPort);
			CloseWindow(window_p);
		}
		CloseLibrary(IntuitionBase);
	}
}

