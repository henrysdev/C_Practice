#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100
int NUMS[SIZE];


struct slice
{
    int start, end;
};


void print_arr()
{
    printf("\n");
    for (int i = 0; i < SIZE-1; ++i)
    {
        printf("%d, ", NUMS[i]);
    }
    printf("%d\n\n", NUMS[SIZE-1]);
}


void *double_val(void *vargp)
{
    struct slice *section = (struct slice*) vargp;
    
    for (int i = (*section).start; i <= (*section).end; ++i)
    {
        NUMS[i] *= 2;
    }
    free(section);
    pthread_exit((void*)vargp);
}


void demo(int tcount)
{
    /* initialize random values for array */
    for (int n = 0; n < SIZE; ++n)
    {
        NUMS[n] = rand() % SIZE;
    }
    print_arr();

    pthread_t threads[tcount];

    struct slice *section;
    int arrslice = SIZE / tcount;
    int rem = SIZE % tcount;
    int lastend = 0;

    /* iterate through threads and distribute work evenly */
    for (int t = 0; t < tcount; ++t)
    {
        int start = lastend;
        int end = start + arrslice - 1;

        if (tcount - t <= rem)
        {
            end += 1;
        }

        lastend = end + 1;
    
        section = malloc(sizeof(struct slice));
        (*section).start = start;
        (*section).end = end;

        pthread_t tid;
        threads[t] = tid;
        printf("start: %d, end: %d\n", start, end);
        pthread_create(&tid, NULL, double_val, (void *)section);
    }

    /* join all threads */
    for (int j = 0; j < tcount; ++j)
    {
        pthread_join(threads[j], NULL);
    }
    
    print_arr();
}


int main(int argc, char** argv)
{
    int tcount = 4;
    if (argc > 1)
    {
        tcount = strtol(argv[1], NULL, 10);
    }
    printf("tcount: %d\n", tcount);

    demo(tcount);
    
    return 0;
}
