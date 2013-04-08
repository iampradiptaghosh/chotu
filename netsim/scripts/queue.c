#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
        FILE*   pFile = fopen( argv[1], "r" );
        char   pStr[256];
        char   pTemp[256];

        int     n, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12;
	int	nTime, nSize;

	if ( argc < 3 )
	{
		printf( " ./queue file [Node1] [Node2]\n" );
		exit(1);
	}

        while ( 1 )
        {
                if( NULL == fgets( pStr, 256, pFile) )
                        break;

		if( 0 == memcmp( pStr," <", 2 ))
		{
			sscanf( pStr, " <%d,%s", &nTime, pTemp );
			continue;
		} 

		if( 0 == memcmp(pStr,"Queue size",10 ) )
		{
			sscanf( pStr, "Queue size at (%d, %d) is %d", &n1, &n2, &nSize );

			if( n1 == atoi(argv[2]) && n2 == atoi(argv[3]) )
				printf("%d %d\n", nTime, nSize);  
		}
        }

        return 0;
}
