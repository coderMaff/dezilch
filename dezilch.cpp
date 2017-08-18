/* 
	dezilch
	
	Throw away long chunks of zeros from files
	
	0.0.1 maf 24/11/02 	Created
	0.0.2 maf 25/11/02 	Added modes 1 & 2 to complament rough mode
						Added Timing
	0.0.3 maf 26/11/02	Minor changes, speed improvements
						Reports correct dest file name on error
						Fixed #of bytes written for part 128k blocks
						Changed BUFFER_SIZE from 128000 to 131072 (real 128k ;o) 128 * 1024)
	
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#define VERSION			"0.0.3"
#define BUFFER_SIZE		131072

int main( int argc, char *argv[] )
{
	
	FILE		*s,*d;
	char		b[BUFFER_SIZE],z[BUFFER_SIZE];
	int			iMode,j,size;
	char		fValid;
	time_t  	ntime1,ntime2;


	time( &ntime1 );

	printf("DeZilch %s by MaF - www.kardiak.co.uk\n", VERSION);
	printf("----------------------------------------------\n");

	if ( argc < 3 )
	{
		printf("Usage : dezilch infile outfile [mode]\n");
		printf("        where [mode] is:\n");		
		printf("        -1 : MemCmp Mode ( default )\n");
		printf("        -2 : Loop Mode ( slowest )\n");
		printf("        -3 : Rough Mode, fastest, but looses some valid data\n");
		printf("What it does:\n");
		printf("dezilch loads 128k chunks from a file and if all the bytes of that 128k chunk\n");
		printf("are zero, dezlich throws them away, but if theres any good data ( not zero )\n");
		printf("then this gets written to your output file - see the readme.txt for more info\n");
		return -1;
	}

	if( (s=fopen( argv[1], "rb" )) == NULL )
	{
		printf("Yah gone did summit wrong boye! cannot open source file : %s\n",argv[1]);
		return -2;
	}

	if( (d=fopen( argv[2], "wb+" )) == NULL )
	{
		printf("Yah gone did summit wrong boye! cannot open destination file %s\n",argv[2]);
		fclose( s );
		return -2;
	}

	iMode = 1;
	
	if( argc == 4 )		// 4 Params so modes been specified
	{
		     if( strcmp( argv[3], "-1" ) == 0 ) iMode = 1;	// Loop through manualy mode
		else if( strcmp( argv[3], "-2" ) == 0 )	iMode = 2;  // memcmp
		else if( strcmp( argv[3], "-3" ) == 0 ) iMode = 3;  // strcmp ( rough )
		else iMode = 1;
		
	}

	memset(z, 0, sizeof(z));

	do
	{
		size = fread( b, 1, BUFFER_SIZE, s);			

		fValid = 0;

		switch( iMode )
		{
			case	1	:	if ( memcmp( b, z, sizeof(b) ) != 0 )	fValid = 1;
							break;

			case	2	:	for( j = 0; j < BUFFER_SIZE; j++ )			
							{
								if( b[j] != 0x00 )
								{
									fValid = 1;
									j = BUFFER_SIZE + 1;
								}
							}			
							break;

			case	3	:	if ( strcmp( b, z ) != 0 )	fValid = 1;
							break;

		}

		if( fValid )
		{
			fwrite( b, 1, size, d);	
			printf("1");				
		}
		else		
			printf("0");
										
	}
	while( !feof( s ) );

	fclose(d);
	fclose(s);
	
    time( &ntime2 );

	printf("\n----------------------------------------------\n");
    printf("%s dezliched to %s in %i second(s)",argv[1],argv[2],ntime2 - ntime1 );	

	return 0;
}