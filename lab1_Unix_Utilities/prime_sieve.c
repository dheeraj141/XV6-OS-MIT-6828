
#include "kernel/types.h"

#include "user/user.h"
#include "kernel/fcntl.h"











int main(int argc,char *argv[])
{
	int i;

	int count = 0;  
	int p[35][2]; 
	for(  i =2; i<=35; i++)
	{
		int j = 0; 
		for( j = 0; j<count; j++)
		{
			close( p[j][1]); 
			int x;  
			read(p[j][0], &x, sizeof( x) ); 
			close( p[j][0]);

			// number is not divisble
			if( i%x != 0)
			{
				continue; 

			}
			else 
				break;


		}

		if( j>=count)
		{
			// found prime print it 
			printf( "%d\n", i);
			pipe( p[count]); 
			if( fork() == 0)
			{
				close( p[count][0]); 

				write( p[count][1], &i, sizeof(i));
				close( p[count][1]);
			}
			else 
			{

			}
			count++;

		}

		


	}

	exit(); 
	return 0;

}


		
