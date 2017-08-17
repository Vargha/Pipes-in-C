/* * * * * * * * * * * * * * * * *
 * Vargha Hokmran  SID: 11543295 *
 * CS-360   Systems Programming  *
 * Assignment -4 Piping in C     *
 * * * * * * * * * * * * * * * * */

# define _BSD_SOURCE
# define RED          "\x1b[31m"
# define GREEN        "\x1b[32m"
# define YELLOW       "\x1b[33m"
# define BLUE         "\x1b[34m"
# define BOLDBLACK    "\033[1m\033[30m"
# define BOLDRED      "\033[1m\033[31m"
# define BOLDGREEN    "\033[1m\033[32m"
# define BOLDYELLOW   "\033[1m\033[33m"
# define ANSI_RESET   "\x1b[0m"

# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
bool noRight(int argc, char *argv[]);   // if there is no argument on :'s left hand side
void msg(char type, char* msg);         // type: E=Error, W=Warning, G=Guid


/* ********** MAIN FUNCTION ********** */
int main(int argc, char* argv[])
{
  int cloneCount =0;                     // if the user is piping at all
  int secondArgIndex=0;
  int cloneIndex=0;
  for (int i=0; i<argc; i++)
  {
    if (strcmp(argv[i], ":")==0)
    {
      cloneCount++;
      argv[i]= '\0';
      secondArgIndex = i+1;
      cloneIndex = i;
    }
  }

  if (argc==1)
    msg('G', "help");
  else if (cloneCount == 0)
    msg ('W',"no pipe");
  else if (cloneCount > 1)
    msg ('W',"extra pipe");
  else if (cloneIndex == 1 )
    msg ('E', "no left");
  else if (cloneIndex+2 > argc)
    msg ('E', "no right");
  else if ( argc<4 )
    msg ('E', "bad arg");
  // The actual progress happens in here
  else
  {
    int fd[2];                         // pipe file descriptors
    int rdr, wtr;                      // fd[0] and fd[1]
    pipe (fd);
    rdr = fd[0];
    wtr = fd[1];

    if (fork ())
    { // parent becomes bc and reads
      close (wtr);
      close (0); dup (rdr); close (rdr);
      execvp (argv[secondArgIndex], &argv[secondArgIndex]);
      fprintf (stderr, "Failed to execute %s\n", argv[secondArgIndex]);
      exit (1);
    }
    else
    { //child writes
      close (rdr);
      close (1); dup (wtr); close (wtr);
      execvp (argv[1], &argv[1]);
      fprintf (stderr, "Failed to execute %s\n", argv[1]);
      exit (1);
    }
  }
	return 0;
}


/* ********** FUNCTIONS ********** */

/* ********** ERROR HANDELING ********** */
void msg(char type, char* msg)         // type: E=Error, W=Warning, M=Message
{
  if       (type == 'E')      printf (BOLDRED     "ERROR:\t"   ANSI_RESET);
  else if  (type == 'W')      printf (BOLDYELLOW  "Warning:\t" ANSI_RESET);
  else if  (type == 'G')      printf (BOLDGREEN   "Guid:\t"    ANSI_RESET);

  if       (strcmp(msg, "help")==0)
    printf ("To pipe Program B's output into Program A use:\tconnect <Program A>:<Program B>\n");
  else if  (strcmp(msg, "no pipe")==0)
    printf ("Unable to find piping symbol. Run the program directly, or Use connect with no argument for help\n");
  else if  (strcmp(msg, "extra pipe")==0)
    printf ("Sorry, the current version can only handle one pipe\n");
  else if  (strcmp(msg, "bad arg")==0)
    printf ("Impropper use of command line argument\n");
  else if  (strcmp(msg, "no left")==0)
    printf ("No argument on the left side of piping symbol\n");
  else if  (strcmp(msg, "no right")==0)
    printf ("No argument on the right side of piping symbol\n");
}
