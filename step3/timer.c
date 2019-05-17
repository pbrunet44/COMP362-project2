/**
* Name: Philip Brunet
* Lab/task: Project 2
* Date: 05/07/19
**/
#include "disk.c"
#include <sys/signal.h>
#include <time.h>
#include <sys/time.h>


#define COUNTDOWN_VALUE 1000

#define TIMEOUT_DELAY_S 1
#define TIMEOUT_DELAY_NS 0

#define TIMEOUT_INTERVAL_S 0
#define TIMEOUT_INTERVAL_NS 1000000

timer_t gTimerid;
int count = COUNTDOWN_VALUE;

void start_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = TIMEOUT_DELAY_S;
    value.it_value.tv_nsec = TIMEOUT_DELAY_NS;

    value.it_interval.tv_sec = TIMEOUT_INTERVAL_S;
    value.it_interval.tv_nsec = TIMEOUT_INTERVAL_NS;

    timer_create (CLOCK_REALTIME, NULL, &gTimerid);

    timer_settime (gTimerid, 0, &value, NULL);

}

void stop_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;

    timer_settime (gTimerid, 0, &value, NULL);
}

void timer_callback(int sig)
{
    printk("Caught timer signal: %d ... !!\n", sig);
    struct timeval ts;
    time_t tm;

    checkDisk();

    time(&tm); // man 3 time
    printk("Time: %s", ctime(&tm)); // man ctime

    gettimeofday(&ts, NULL); // man gettimeofday
    printk("Time: %ld.%06ld secs.\n\n", (long)ts.tv_sec, (long)ts.tv_usec);
    count--;
}

void checkDisk()
{
    int logaddr = rand() % MAX_LOGICAL_SECTOR;
    printTransl(logaddr);
    int numBlocks = (rand() % 5) + 1;
    if(rand() % 2 == 0) //Read
    {
        char** readbuf = kmalloc(numBlocks*SECT_SIZE + 1, GFP_KERNEL);
        readDisk(logaddr,numBlocks, readbuf);
        printk("Reading %d blocks\nRead buffer content: %s\n", numBlocks, readbuf);
        kfree(readbuf);
    }
    else //Write
    {
        char* writebuf = generateContent((SECT_SIZE * numBlocks));
        writeDisk(logaddr,numBlocks, writebuf);
        printk("Writing %d blocks\nWriting content: %s\n", numBlocks, writebuf);
    }

}

int main(int ac, char **av)
{
    (void) signal(SIGALRM, timer_callback);
    start_timer();
    while (count >= 0);
}
