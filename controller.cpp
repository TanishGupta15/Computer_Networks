#include <stdio.h>
#include <string.h>
struct mydata
{
    int checkpoints[1000];
    char *data[1000];
    int complete;
};

void *controller(void *args)
{
    struct mydata *mydat = (struct mydata *)args;
    while (mydat->complete == 0)
    {
        // printf("here\n");
        int chk = 0;
        for (int i = 0; i < 1000; i++)
        {
            if (mydat->checkpoints[i] == 0)
            {
                chk = 0;
                break;
            }
        }
        if (chk == 1)
        {
            mydat->complete = 1;
        }
    }
    int a = 2;
    int *b = &a;
    void *c = (void *)b;
    return c;
}