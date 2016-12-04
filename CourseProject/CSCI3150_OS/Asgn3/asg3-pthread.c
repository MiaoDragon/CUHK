/*
* CSCI3150 Assignment 3 - Implement pthread and openmp program
*
*/
/* Header Declaration */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <sys/time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
/* macro function */
/* Function Declaration */
extern int *readdata(char *filename, long *number);
void *init(void*);  // achieve initialization of hash table
void *hashData(void*);  // hash array1
void *andData(void*);   // AND array2, here we add the hash value by 1
void *count(void*);
void *convert(void*);
/* Contant & Global Variable */
#define MAX 100000000
#define MAX_WRITE1 3
#define MAX_WRITE2 5 
#define threshold1 50000000
#define threshold2 81000000
int threadNum;
pthread_t tid[4];
int pos[9] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
int mins[4] = {MAX, MAX, MAX, MAX};
int counts[4] = {0, 0, 0, 0};
int maxs[4] = {0, 0, 0, 0};   
int min, max;
struct iovec iov[4];
int *array1, *array2;
long num1, num2;
struct arg
{
    int index;
    char *buf;
    char *hash;
} args[4];
/* Main */
int main(int argc, char *argv[]) {
    if(argc!=5) {
        printf("usage:\n");
        printf("    ./asg3-pthread inputfile1 inputfile2 outputfile ThreadNum\n");

        return -1;
    }
    array1 = readdata(argv[1], &num1);
    array2 = readdata(argv[2], &num2);
    /* do your assignment start from here */
    //struct timeval tv1, tv2;
    //gettimeofday(&tv1, NULL);
    int fd = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    int writen;
    if (num1 == 0 || num2 == 0)
    {
        writen = write(fd, "\n", 1);
        return 0;
    }

    char *hash = (char*) malloc(sizeof(char) * (MAX+1));
    threadNum = (int)(argv[4][0] - '0');  // including main thread
    /* hash initialization; hash array1; array2 AND hash */
    int i = 0;
    for (; i < threadNum; i++)  // arguments initialization
    {
        args[i].index = i;
        args[i].hash = hash;
        args[i].buf = NULL;
    }
    // init
    for (i = 0; i < threadNum-1; i++)
    {   
        pthread_create(&(tid[i]), NULL, init, &(args[i]));
    }
    (*init)(&(args[threadNum-1]));
    for (i = 0; i < threadNum-1; i++)
        pthread_join(tid[i], NULL);
    // hashData
    for (i = 0; i < threadNum-1; i++)
    {
        pthread_create(&(tid[i]), NULL, hashData, &(args[i]));
    }
    (*hashData)(&(args[threadNum-1]));
    for (i = 0; i < threadNum-1; i++)
        pthread_join(tid[i], NULL);
    // andData
    for (i = 0; i < threadNum-1; i++)
    {
        pthread_create(&(tid[i]), NULL, andData, &(args[i]));
    }
    (*andData)(&(args[threadNum-1]));
    min = mins[threadNum-1]; max = maxs[threadNum-1];
    for (i = 0; i < threadNum-1; i++)
    {
        pthread_join(tid[i], NULL);
        if (mins[i] < min)  min = mins[i];
        if (maxs[i] > max)  max = maxs[i];
    }
    // count
    for (i = 0; i < threadNum-1; i++)
        pthread_create(&(tid[i]), NULL, count, &(args[i]));
    (*count)(&(args[threadNum-1]));
    int num = counts[threadNum-1];
    for (i = 0; i < threadNum-1; i++)
    {
        pthread_join(tid[i], NULL);
        num += counts[i];
    }
    // output
    // multithread convert int to string
    // then writev
    // disaster case: write 5 times
    // mediate case: write 3 times
    // light case: write 1 time
    int temp = max, step;
    int j;
    if (num > threshold2)
    {
        step = (max-min+1)/MAX_WRITE2;
        for (j = 0; j < MAX_WRITE2; j++)
        {
            if (j == MAX_WRITE2-1)
                max = temp;
            else
                max = min+step;
            for (i = 0; i < threadNum-1; i++)
                pthread_create(&(tid[i]), NULL, convert, &(args[i]));
            (*convert)(&(args[threadNum-1]));
            for (i = 0; i < threadNum-1; i++)
                pthread_join(tid[i], NULL);
            writen = writev(fd, iov, threadNum);
            min = max+1;
        }
    }
    else if (num > threshold1)
    {
        step = (max-min+1)/MAX_WRITE1;
        for (j = 0; j < MAX_WRITE1; j++)
        {
            if (j == MAX_WRITE1-1)
                max = temp;
            else
                max = min+step;
            for (i = 0; i < threadNum-1; i++)
                pthread_create(&(tid[i]), NULL, convert, &(args[i]));
            (*convert)(&(args[threadNum-1]));
            for (i = 0; i < threadNum-1; i++)
                pthread_join(tid[i], NULL);
            writen = writev(fd, iov, threadNum);
            min = max+1;
        }
    }
    else
    {
        for (i = 0; i < threadNum-1; i++)
            pthread_create(&(tid[i]), NULL, convert, &(args[i]));
        (*convert)(&(args[threadNum-1]));
        for (i = 0; i < threadNum-1; i++)
            pthread_join(tid[i], NULL);
        writen = writev(fd, iov, threadNum);
    }
    close(fd);
    for (i = 0; i < threadNum; i++)
        free(args[i].buf);
    free(hash);
    //benchmark
    //gettimeofday(&tv2, NULL);
    //printf("PROCESS TIME: %f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec)/1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
    return 0;
}
void *init(void *input)
// init the hash table
{
    struct arg *para = (struct arg*) input;
    int tem = MAX/threadNum;
    int start = tem*(para->index);
    int end;
    if (para->index < threadNum-1)
        end = tem*(para->index+1)-1;
    else
        end = MAX;
    int i;
    for (i = start; i <= end; i++)
    {
        *(para->hash+i) = (char) 0;
    }
}
void *hashData(void *input)
// hash array1
{
    struct arg* para = (struct arg*) input;
    long tem = num1/threadNum;
    long start = tem*(para->index);
    long end;
    if (para->index < threadNum-1)
        end = tem*(para->index+1)-1;
    else
        end = num1-1;
    long i = 0;
    for (i = start; i <= end; i++)
    {
        *(para->hash+array1[i]) = (char) 1;
    }
}
void *andData(void *input)
// read array2, hash value + 1
{
    struct arg* para = (struct arg*) input;
    long tem = num2/threadNum;
    long start = tem*(para->index);
    long end;
    if (para->index < threadNum-1)
        end = tem*(para->index+1)-1;
    else
        end = num2-1;
    long i = 0;
    for (i = start; i <= end; i++)
    {
        if (*(para->hash+array2[i]) == (char)1)
        {
            *(para->hash+array2[i]) = (char)2;
            if (array2[i] < mins[para->index])
                mins[para->index] = array2[i];
            if (array2[i] > maxs[para->index])
                maxs[para->index] = array2[i];
        }
    }
}
void *count(void *input)
{
    struct arg* para = (struct arg*) input;
    int tem = (max-min+1)/threadNum;
    int start = tem*(para->index)+min;
    int end;
    if (para->index < threadNum-1)
        end = tem*(para->index+1)-1+min;
    else
        end = max;
    int i = 0;
    for (i = start; i <= end; i++)
        if (*(para->hash+i) == (char)2)
            counts[para->index] += 1;
}
void *convert(void *input)
// convert data from hash table to string
{
    struct arg* para = (struct arg*) input;
    int tem = (max-min+1)/threadNum;
    int start = tem*(para->index)+min;
    int end;
    if (para->index < threadNum-1)
        end = tem*(para->index+1)-1+min;
    else
        end = max;
    free(para->buf);
    para->buf = (char*)malloc(sizeof(char)*9*(end-start+1));
    char* ptr = para->buf;
    int i = 0, j = 0, d1;
    char dig_num = 0;
    for (i = start; i <= end; i++)
    {
        if (*(para->hash+i) == (char)2)
        {
            //original method
            /*dig_num = 8;
            for (j = MAX; j > 0; j /= 10)
                if (i >= j)   break;
                else    dig_num--;
            if (i == 0)   dig_num = 0;
            for (j = 0; j <= dig_num; j++)
            {
                *(para->buf+length+j) = (char)('0' + (i/pos[dig_num-j]) % 10);
            }
            length = length + dig_num + 2; //next position
            *(para->buf+length-1) = '\n';
            */
            d1 = i/pos[5];
            if (d1 > 0)
            // aaaaaaaaa
            {
                 dig_num = 8;
                 for (j = pos[3]; j > 0; j /= 10)
                     if  (d1 >= j) break;
                     else  dig_num--;
                 for (j = dig_num; j >= 0; j--)
                 {
                     *ptr++ = '0' + (i/pos[j]%10);
                 }
                 *ptr++ = '\n';
            }
            else
            //xxxxbbbbb
            {
                dig_num = 4;
                for (j = pos[4]; j > 0; j /= 10)
                    if  (i >= j) break;
                    else  dig_num--;
                if (i==0)  dig_num = 0;
                for (j = dig_num; j>= 0; j--)
                {
                    *ptr++ = '0' + (i/pos[j]%10);
                }
                *ptr++ = '\n';
            }
        }
    }
    *(ptr) = '\0';
    iov[para->index].iov_base = para->buf;
    iov[para->index].iov_len = (int)(ptr-para->buf);
}

