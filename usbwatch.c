#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( int argc, char **argv ) 
{
  int length, i = 0;
  int fd;
  int wd1,wd2,wd3,wd4;
  char buffer[BUF_LEN];

  fd = inotify_init();

  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  wd1 = inotify_add_watch( fd, "/dev/bus/usb/001", 
                         IN_MODIFY | IN_CREATE | IN_DELETE );
  wd2 = inotify_add_watch( fd, "/dev/bus/usb/002", 
                         IN_MODIFY | IN_CREATE | IN_DELETE );
  wd3 = inotify_add_watch( fd, "/dev/bus/usb/003", 
                         IN_MODIFY | IN_CREATE | IN_DELETE );
  wd4 = inotify_add_watch( fd, "/dev/bus/usb/004", 
                         IN_MODIFY | IN_CREATE | IN_DELETE );


  length = read( fd, buffer, BUF_LEN );  

  if ( length < 0 ) {
    perror( "read" );
  }  

  while ( i < length ) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if ( event->len ) {
      if ( event->mask & IN_CREATE ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "The directory %s was created.\n", event->name );       
        }
        else {
          printf( "The file %s was created.\n", event->name );
        }
      }
      else if ( event->mask & IN_DELETE ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "The directory %s was deleted.\n", event->name );       
        }
        else {
          printf( "The file %s was deleted.\n", event->name );
        }
      }
      else if ( event->mask & IN_MODIFY ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "The directory %s was modified.\n", event->name );
        }
        else {
          printf( "The file %s was modified.\n", event->name );
        }
      }
    }
    i += EVENT_SIZE + event->len;
   main(argc,argv);
  }

  ( void ) inotify_rm_watch( fd, wd1);
  ( void ) inotify_rm_watch( fd, wd2);
  ( void ) inotify_rm_watch( fd, wd3);
  ( void ) inotify_rm_watch( fd, wd4);
  ( void ) close( fd );
  return 0;
}
