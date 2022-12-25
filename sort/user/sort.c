#include "user.h"

typedef struct {
    char *base;
    unsigned int length;
    unsigned int capacity;
} Buffer;

// forward declarations
Buffer allocBuffer(int);
int readIntoBuffer(int, Buffer *);
int getNums(Buffer buffer, int reverse);
void sort(int size, int* ptr);
void sortReverse(int size, int* ptr);

int main(int argc, char *argv[]){
    // fd: file descriptor
    int fd;
    int rev = 0; // set as no reverse

    // no input
    if (argc < 1){
        exit();
    }
    // standard input; cat file.txt | sort
    if (argc == 1){
        fd = 0;
    }

    if (argc == 2){
        // cat file.txt | sort -r
        if (strcmp(argv[1], "-r") == 0){
            rev = 1; // set as reverse       
        }
        // sort file.txt
        else if((fd = open(argv[1], O_RDWR)) < 0){
            printf(1, "sort: open error 2 %s\n", argv[1]); // fail to open
            exit();
        }
    }

    if (argc == 3){
        // sort -r numbers.txt
        if (strcmp(argv[1], "-r") == 0){
            rev = 1; // set as reverse     
        }
        if((fd = open(argv[2], O_RDWR)) < 0){
            printf(1, "sort: open error 1 %s\n", argv[2]); // fail to open
            exit();
        }
    }

    // read into buffer
    Buffer buffer = allocBuffer(512);
    if (readIntoBuffer(fd, &buffer) < 0){
        printf(1, "sort: read error\n");
        exit();
    }

    // print out result to stdout
    getNums(buffer, rev);

    exit(); // exit program
    
}

// sorting algorithm - reversed
void sortReverse(int size, int* ptr){
    int i, j, temp;

    for (i = 0; i < size; i++){
        for (j = i; j < size; j++){
            if (*(ptr + j) > *(ptr + i)){
                temp = *(ptr + i);
                *(ptr + i) = *(ptr + j);
                *(ptr +j) = temp;
            }
        }
    }
}

// sorting algorithm
void sort(int size, int* ptr){
    int i, j, temp;

    for (i = 0; i < size; i++){
        for (j = i; j < size; j++){
            if (*(ptr + j) < *(ptr + i)){
                temp = *(ptr + i);
                *(ptr + i) = *(ptr + j);
                *(ptr +j) = temp;
            }
        }
    }
}

// get num into an array to be sorted
int getNums(Buffer buffer, int reverse){ // fd is file being sorted
    int result[buffer.capacity];
    int i = 0;
    result[i] = atoi(buffer.base);
    int numElem;

    // setting list to integers in the file
    for (char *cur = buffer.base; cur != buffer.base + buffer.length; ++cur){
        if (*cur == ' '){
            i += 1;
            result[i] = atoi(cur+1);
        }
    }
    
    // find the integers to sort
    numElem = i + 1;

    // if reverse is not true do this
    if (reverse == 0){
        sort(numElem, result);
    }
    // if reverse is true do this
    else if (reverse == 1){
        sortReverse(numElem, result);
    }
    
    // printing out the sorted list
    for (int i = 0; i < numElem; i++){
        printf(1, "%d ", result[i]);
    }  
    return *result;
}

// allocating buffer using Buffer struct
Buffer allocBuffer(int initialSize){
    return (Buffer){
        .base = malloc(initialSize),
        .length = 0,
        .capacity = initialSize,
    };
}

// read from file into the buffer using a while loop
int readIntoBuffer(int fd, Buffer *buffer){
    int result;
    while ((result = read(fd, buffer->base + buffer->length, buffer->capacity - buffer->length))){
        buffer->length += result;
        if (buffer->length == buffer->capacity){
            buffer->capacity *= 2;
            buffer->base = realloc(buffer->base, buffer->capacity);
        }
    }
    return result;
    free(buffer->base);
}