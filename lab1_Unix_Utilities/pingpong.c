
#include "kernel/types.h"

#include "user/user.h"
#include "kernel/fcntl.h"




int main( int argc, char *argv[])
{
	int pd[2], cd[2]; 

	pipe(pd);
	pipe( cd); 



	// calling the child process 



	if(fork() == 0)
	{ 
		char buf[10]; 

		// read file descriptor
		close( pd[1]);
		close( cd[0]);
		read( pd[0], buf, 1);


		if( buf[0] == '1')
		{
			printf(" process id %d, received ping \n", getpid());
			close( pd[0]);

                	write( cd[1], "0", 1);
                	close( cd[1]); 

		}

	}
	else 
	{
		// closed the  parent read one and child write one  
		close( pd[0]); 
		write( pd[1], "1", 1);

		close( pd[1]);

		close( cd[1]);
		
		char buf1[10];
		// read from child pipe 
		read( cd[0], buf1, 1); 
		if( buf1[0] == '0')
		{
			printf(" process id %d, received pong \n", getpid());
			close( cd[0]); 

		}


	}

exit();
return 0;


	
	
}

