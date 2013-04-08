#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
        FILE*   pFile = fopen( argv[1], "r" );
        char  pStr[256];
        char   pTemp[256];

        int     n, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12;

	if ( argc < 2 )
	{
		printf( " ./gen FILENAME [CWND/SST/SUNA/SNXT/RTO/RTT]\n" );
		exit(1);
	}

        while ( 1 )
        {
                if( NULL == fgets( pStr, 256, pFile) )
                        break;


                sscanf( pStr, "%10d(%2d):CWND/SST/SUNA/SNXT/RTO/RTT:%8d/%8d/%8d/%8d/%8d/%8d: %s",
                        &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, pTemp);

                if ( 0 == strcmp ("SEND", argv[2]))
                {
                        sscanf( pStr, "%10d(%2d):CWND/SST/SUNA/SNXT/RTO/RTT:%8d/%8d/%8d/%8d/%8d/%8d: SEND(SZ/SEQ/ACK=%4d/%8d/%8d) to (%2d)\n",
                                &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9, &n10, &n11, &n12);

                        printf("%10d %8d\n", n1, n10);
                        continue;
                }

                if ( 0 == strcmp ("RECV", argv[2]))
                {
                        sscanf( pStr, "%10d(%2d):CWND/SST/SUNA/SNXT/RTO/RTT:%8d/%8d/%8d/%8d/%8d/%8d: RECV(SZ/SEQ/ACK=%4d/%8d/%8d) from (%2d)\n",
                                &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9, &n10, &n11, &n12);
                        printf("%10d %8d\n", n1, n10);
                        continue;
                }


                if ( 0 == strcmp ("CWND", argv[2]))
                    printf("%10d %8d\n", n1, n3);

                if ( 0 == strcmp ("SST", argv[2]))
                    printf("%10d %8d\n", n1, n4);

                if ( 0 == strcmp ("SUNA", argv[2]))
                    printf("%10d %8d\n", n1, n5);

                if ( 0 == strcmp ("SNXT", argv[2]))
                    printf("%10d %8d\n", n1, n6);

                if ( 0 == strcmp ("RTO", argv[2]))
                    printf("%10d %8d\n", n1, n7);

                if ( 0 == strcmp ("RTT", argv[2]))
                    printf("%10d %8d\n", n1, n8);
        }

        return 0;
}
