#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int NUMS[100];


struct slice
{
    int start, end;
};


void print_arr()
{
    for (int i = 0; i < 100; ++i)
    {
        printf("%d\n", NUMS[i]);
    }
}


void *double_val(void *vargp)
{
    struct slice* section = (struct slice*) vargp;
    
    for (int i = section->start; i < section->end; ++i)
    {
        NUMS[i] *= 2;
    }
    free(section);
}


void demo(int tcount)
{
    /* initialize random values for array */
    for (int i = 0; i < 100; ++i)
    {
        NUMS[i] = rand() % 100;
    }
    print_arr();

    pthread_t threads[tcount];

    int slice = 100 / tcount;
    for (int i = 0; i < tcount; ++i)
    {
        int start = i * slice;
        int end = start + slice;
        if (i == tcount - 1)
        {
            end = 99;
        }
        struct slice * section;
        section = (struct slice*) malloc(sizeof(struct slice));
        section->start = start;
        section->end = end;

        pthread_t tid;
        threads[i] = tid;
        pthread_create(&tid, NULL, double_val, (void *)slice);
    }

    for (int i = 0; i < tcount; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    print_arr();
}


int main(int argc, char** argv)
{
    int tcount = 4;
    if (argc > 1)
    {
        tcount = atoi(argv[1]);
    }

    demo(tcount);
    
    return 0;
}
