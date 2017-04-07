#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

static void yummyfood()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("foodlog", LOG_PID, LOG_DAEMON);
}

static void weGotFood(){
    syslog(LOG_NOTICE, "Calling python script");
    //system("python foodeatingcontest.py");

    //Wait for script to complete
    while(1){
        sleep(1);
        if ( access( "config.csv", F_OK ) != -1 ){
            //Write to serial now with formatted file
        }
    }
}


int main()
{
    yummyfood();
    syslog (LOG_NOTICE, "Lets start eating.");

    while (1)
    {
        //TODO: Insert daemon code here.
        if( access( "config.csv", F_OK ) != -1 ) {
            weGotFood();
        }
        sleep (1);
        break;
    }

    syslog (LOG_NOTICE, "We are done eating.");
    closelog();

    return EXIT_SUCCESS;
}