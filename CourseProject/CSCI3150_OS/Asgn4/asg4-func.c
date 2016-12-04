#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define ADDERROR  "Add error!"
#define RETRIEVEERROR  "Retrieve error!"
#define DIFFERROR  "Diff error!"
#define CALERROR  "Calculate error!"
#define ADD_SUC "Add success!"
#define RETRIEVE_SUC "Retrieve success!"
#define PATH ".vfsdata"
/**
  version 3
  added:
    compress the encoded binary file again
    decompress the encoded binary file when read
    done by inserting head and data
*/
/**
  version 2
  added:
    store binary files containing the encoded bytes
                       with a header(int) of how many nums
*/
/**
  version 1 (not working for partB)
  store binary files containing only integers inside vfsdata
  for all functions: if folder not exist, create
*/
void readBit(int, int*, unsigned char*, int);
void writeBit(int, unsigned char**, int*, int);
int getBitNum(int);
void readIn(int, int**, FILE*);  // read in from the encoded file
                                  // get the array of int
void encode(int, unsigned char**);  // append the encoded int to string;
                           // then change the ptr to the next pos
int decode(int* ,unsigned char*, int);  // decode the buf to int array
int getSqrt(int);
int getLength(int);
const int BYTE[3] = {255, 65535, 16777215};
char* getFilename(char*);
int  add(char *path){
/**
  add:  
        1. use version_function to store the latest version #
        (not exist then create with 1; else increment by 1)
        2. read in string buf;
        3. convert each substring to integers;
        4. write integer array to binary file
*/
	FILE *in, *out, *check;
    int version, num_rw;
    char* filename = getFilename(path);
    in = fopen(path, "r");
    if (!in)
    {
        printf("%s\n", ADDERROR); 
        return -1;
    }
    // get latest version of file
    char check_path[100];
    sprintf(check_path, "%s/version_%s", PATH, filename);
    check = fopen(check_path, "r+b");
    if (!check) //NULL
    {
        check = fopen(check_path, "wb");
        version = 1;
    }
    else
    {
        num_rw = fread(&version, sizeof(version), 1, check);
        version++;
        fseek(check, 0, SEEK_SET);
    }
    num_rw = fwrite(&version, sizeof(version), 1, check);
    close(check);
    // read in string
    char out_path[100];
    sprintf(out_path, "%s/%d_%s", PATH, version, filename);
    out = fopen(out_path, "wb");
 
    char* buf = NULL;
    int length = 0, i = 0, num = 0, max = 0, count = 0;
    length = getline(&buf, &length, in);
    // check how many integers
    while ((length > 0) && !(buf[length-1] >= '0' && buf[length-1] <= '9'))
        length--;
    for (i = 0; i < length; i++)
        if (buf[i] == ' ')  num++;
    if (length <= 0)
    {
        num_rw = fwrite(&num, sizeof(int), 1, out);
        num_rw = fwrite(&count, sizeof(int), 1, out);
        free(buf);
        printf("%s\n", ADD_SUC);
        close(in); close(out);
        return 0;
    }
    else    num++;
    int *array = (int*)malloc(sizeof(int) * num);
    // convert string to int array
    char *ptr = buf;
    for (i = 0; i < num; i++)
    {
        array[i] = strtol(ptr, &ptr, 10);
        if (array[i] > max)     max = array[i];
    }
    int row_num = getSqrt(num);
    free(buf);
    int *pos = (int*)malloc(sizeof(int) * (max+1));
    memset(pos, 0, sizeof(int) * (max+1));
    // get each distinct integer position
    for (i = 0; i < num; i++)   
        if (pos[array[i]] == 0)     
        {
            count++;
            pos[array[i]] = 1;
        }
    int* d_array = malloc(sizeof(int) * count);    // save distinct integers
    count = 0;
    for (i = 0; i <= max; i++)
        if (pos[i] == 1)
        {
            d_array[count] = i;     count++;    pos[i] = count;  // start from 1
        }
    for (i = 0; i < num; i++)   array[i] = pos[array[i]]-1;  // now store the positions
    free(pos);

    // encode the distinct int array to byte array; store in buf_encode
    num_rw = fwrite(&num, sizeof(int), 1, out);
    num_rw = fwrite(&count, sizeof(int), 1, out);
    unsigned char* buf_encode = malloc(sizeof(char) * 5 * (num+count));
    unsigned char* ptr_encode = buf_encode;
    for (i = 0; i < count; i++)
        encode(d_array[i], &ptr_encode);
    free(d_array);
    // ptr now points to the next pos of the last element
    // output
    num_rw = fwrite(buf_encode, sizeof(char), ptr_encode-buf_encode, out);
    // store the position of each element
    memset(buf_encode, 0, 5*(num+count));
    ptr_encode = buf_encode;
    int num_bit = getBitNum(count-1), index = 7;
    for (i = 0; i < num; i++)
       writeBit(array[i], &ptr_encode, &index, num_bit);
    if (index != 7)  ptr_encode++;
    num_rw = fwrite(buf_encode, sizeof(char), ptr_encode-buf_encode, out);
    printf("%s\n", ADD_SUC);
    free(array);
    fclose(in); fclose(out);
}


int retrieve(char *path, int version){
/**
  1. check if version_filename exist, if not error
  2. read version_filename into array
  3. convert int array into string
  4. write string to path
*/
    FILE *in, *out;
    char *filename = getFilename(path);
    char in_path[100];
    sprintf(in_path, "%s/%d_%s", PATH, version, filename);
    in = fopen(in_path, "rb");
    if (!in)
    {
        printf("%s\n", RETRIEVEERROR);
        return -1;
    }
    // get the integers num
    out = fopen(path, "w");
    int *array, num;
    int rw_num = fread(&num, sizeof(int), 1, in);
    if (num == 0)
    {
        close(in);
        close(out);
        printf("%s\n", RETRIEVE_SUC);
        return 0;
    }
    readIn(num, &array, in);
    // input
    // get the length of each int
    int i, length = 0;
    for (i = 0; i < num; i++)
        length += getLength(array[i]) + 1;
    // store in the buf
    char *buf = (char*)malloc(sizeof(char) * (length+1));
    length = 0;
    for (i = 0; i < num; i++)
        length += sprintf(buf+length, "%d ", array[i]);
    buf[length] = '\0';
    // print out
    fputs(buf, out);
    printf("%s\n", RETRIEVE_SUC);
    close(in); close(out);
    free(buf); free(array);
	return 0;
}

int diff(char *path, int version1, int version2, int row, int column ){
/**
  1. check if version1_filename, and version2_filename exist
  2. get num of rows/columns
  3. check if row <= #row AND column <= #column
  4. fseek to the position of v1 and v2
  5. output
*/
    FILE *v1, *v2;
    char v1_path[100], v2_path[100];
    char *filename = getFilename(path);
    sprintf(v1_path, "%s/%d_%s", PATH, version1, filename);
    v1 = fopen(v1_path, "rb");
    if (!v1)
    {
        printf("%s\n", DIFFERROR);
        return -1;
    }
    sprintf(v2_path, "%s/%d_%s", PATH, version2, filename);
    v2 = fopen(v2_path, "rb");
    if (!v2)
    {
        printf("%s\n", DIFFERROR);
        return -1;
    }
    int num, *array1, *array2, rw_num;
    rw_num = fread(&num, sizeof(int), 1, v1);
    // get row_num
    int row_num = getSqrt(num);
    if (!(row >= 1 && row <= row_num && column >= 1 && column <= row_num))
    {
        printf("%s\n", DIFFERROR);
        return -1;
    }
    // get position and check
    fseek(v2, sizeof(int), SEEK_SET); 
    readIn(num, &array1, v1);
    readIn(num, &array2, v2);
    int position = (row-1)*row_num + column-1;
    int a1, a2;
    a1 = array1[position];
    a2 = array2[position];
    if (a1 == a2)
        printf("%d\n", 0);
    else
        printf("%d\n", 1);
    free(array1); free(array2);
    close(v1);  close(v2);
	return 0;
}

int calculate(char *path, int version1, int version2, char *par, int row ,int column){
/**
  1. check filename
  2. check row and column
  3. -r: save in array by fseek
     others: use array of all, and check corresponding positions
*/
    FILE *v1, *v2;
    char v1_path[100], v2_path[100];
    char *filename = getFilename(path);
    sprintf(v1_path, "%s/%d_%s", PATH, version1, filename);
    v1 = fopen(v1_path, "rb");
    if (!v1)
    {
        printf("%s\n", CALERROR);
        return -1;
    }
    sprintf(v2_path, "%s/%d_%s", PATH, version2, filename);
    v2 = fopen(v2_path, "rb");
    if (!v2)
    {
        printf("%s\n", CALERROR);
        return -1;
    }
    // check par
    if (!(par[0] == '-' && (par[1] == 'r' || par[1] == 'c' || par[1] == 'a')))
    {
        printf("%s\n", CALERROR);
        return -1;
    }
    int num, *array1, *array2, rw_num;
    rw_num = fread(&num, sizeof(int), 1, v1);
    // get row_num
    int row_num = getSqrt(num);
    if (!(row >= 1 && row <= row_num && column >= 1 && column <= row_num))
    {
        printf("%s\n", CALERROR);
        return -1;
    }
    int i, j, diff = 0;
    fseek(v2, sizeof(int), SEEK_SET);
    readIn(num, &array1, v1);
    readIn(num, &array2, v2);
    int up, down, left, right;
    if (par[1] == 'r')
    {
        up = row-1; down = row-1; left = 0; right = row_num-1;
    }
    else if (par[1] == 'c')
    {
        left = column-1; right=column-1; up = 0; down = row_num-1;
    }
    else
    {
        left = (column == 1) ? column-1:column-2;
        right = (column == row_num) ? column-1:column;
        up = (row == 1) ? row-1:row-2;
        down = (row == row_num) ? row-1:row;
    }
    for (i = up; i <= down; i++)
        for (j = left; j <= right; j++)
            diff += array1[i*row_num+j]-array2[i*row_num+j];
    printf("%d\n", diff);
    close(v1); close(v2); free(array1); free(array2);
}

char* getFilename(char *path)
{
    char *ptr;
    ptr = strrchr(path, '/');
    if (!ptr)   //NULL
        ptr = path;
    else
        ptr++;
    return ptr;
}

int getLength(int n)
{
    char temp[15];
    sprintf(temp, "%d", n);
    return strlen(temp);
}

int getSqrt(int n)
{
    int i = 0;
    for (i = 0; i < n; i++)
        if (i*i==n)
            return i;
}
int getBitNum(int count)
{
    int i = 0;
    unsigned int n = (unsigned int) count;
    for (i = 1; i <= 32; i++)
    {
        n = n >> 1;
        if (n == 0)  break;
    }
    return i;
    
}
void readIn(int num, int **arr_ptr, FILE *in) // read in from the encoded file
                                                   // get the array of int and num
{
    int count;
    int rw_num = fread(&count, sizeof(int), 1, in);
    if (count == 0)
        return;
    fseek(in, 0, SEEK_END);
    int length = ftell(in) - sizeof(int)*2;
    fseek(in, sizeof(int)*2, SEEK_SET);
    int bit_num = getBitNum(count-1);
    int *d_array = (int*)malloc(sizeof(int)*count);
    *arr_ptr = (int*)malloc(sizeof(int)*num);
    unsigned char *buf = malloc(sizeof(char) * length);
    rw_num = fread(buf, sizeof(char), length, in);
    unsigned char *ptr = buf + decode(d_array, buf, count); // next unhandled byte
    readBit(num, *arr_ptr, ptr, bit_num);
    int i = 0;
    for (i = 0; i < num; i++)
        (*arr_ptr)[i] = d_array[(*arr_ptr)[i]];
    free(d_array);
    free(buf);
}
void encode(int n, unsigned char **buf_ptr)  // append the encoded int to string;
                                             // then change the ptr to the next pos
{
    unsigned char length = 0;
    unsigned char *buf = *buf_ptr;
    int i = 0;
    for (i = 0; i < 3; i++)
        if (n <= BYTE[i])
        {
            length = (unsigned char) (i+1);
            break;
        }
    if (length == 0)    length = 4;
    *buf = length;
    for (i = length-2; i >= 0; i--)
    {
        buf++;
        *buf = (unsigned char)(n / (BYTE[i]+1));
        n = n % (BYTE[i]+1);
    }
    buf++;   *buf = (unsigned char) n;
    *buf_ptr = buf+1;   
    
}
int decode(int *d_array, unsigned char *buf, int count)  // decode the buf to int array
{
    int i = 0, j = 0;
    unsigned char num;
    int s, n = 0;
    while (n < count)
    {
        num = buf[i];  s = 0;
        for (j = 0; j < num; j++)
        {
            i++;
            s = (s<<8) + buf[i];
        }
        i++;
        d_array[n] = s;   n++;
    }
    return i;   //return next position in buf
}

void readBit(int num, int* array, unsigned char* buf, int bit_num)
{
    int i, j, index = 7;
    unsigned char bit;
    for (i = 0; i < num; i++)
    {
        array[i] = 0;
        for (j = 0; j < bit_num; j++)
        {
            bit = (*buf >> index) & 1;
            array[i] = (array[i] << 1) | bit;
            index--;
            if (index == -1)
            {
                index = 7;  buf++;
            }
        }
    }
}
void writeBit(int position, unsigned char** buf_ptr, int* index_ptr, int num_bit)
{
    unsigned int pos = position;
    unsigned char* buf = *buf_ptr;
    int index = *index_ptr;
    int i = 0;
    unsigned char bit = 0;
    for (i = num_bit; i > 0; i--)
    {
        bit = (pos >> (i-1)) & 1;
        *buf |= (bit << index);
        index--;
        if (index == -1)
        {
            buf++;  index=7;
        }
    }
    *buf_ptr = buf;
    *index_ptr = index;
}
