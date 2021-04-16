#include "read_lib.h"

/*******************************************************
Nom ......... : viderBuffer
Role ........ : 
Arg ......... : 
Return....... : VOID
********************************************************/
void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

/*******************************************************
Nom ......... : lire
Role ........ : 
Arg ......... : *chaine  :
		        longueur : 
Return....... : int
********************************************************/
int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;

    if (fgets(chaine, longueur, stdin) != NULL)
    {
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL)
        {
            *positionEntree = '\0';
        }
        else
        {
            viderBuffer();
        }
        return 1;
    }
    else
    {
        viderBuffer();
        return 0;
    }
}
