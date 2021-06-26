#include <syscall.h>

int main (int, char *[]);
void _start (int argc, char *argv[]);

void
_start (int argc, char *argv[]) 
{
  for(int i=0; i<argc; ++i) 
  exit (main (argc, argv));
}
