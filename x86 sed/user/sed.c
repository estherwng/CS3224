#include "user.h"
#define NULL 0


typedef struct {
    char *base;
    unsigned int length;
    unsigned int capacity;
} Buffer;


// forward declarations
Buffer allocBuffer(int);
int readIntoBuffer(int, Buffer *);
char* strstr(char* a, char* b);
char* sed(char *string, char *str1[], char *str2[]);


int main(int argc, char *argv[]){
    int fd;
    int stringSize = 32;
    char *str1[stringSize];
    char *str2[stringSize];

    if (argc < 1){
        exit();
    }

    // cat filename | sed str1 str2
    if (argc == 3){
        fd = 0;
        *str1 = argv[1];
        *str2 = argv[2];
    }

    // sed str1 str2 filename
    if (argc == 4){
        *str1 = argv[1];
        *str2 = argv[2];

        if ((fd = open(argv[3], O_RDWR)) < 0){
            printf(1, "sed: open error 1 %s\n", argv[3]);
        }
    }

    // read into buffer
    Buffer buffer = allocBuffer(512);
    if (readIntoBuffer(fd, &buffer) < 0){
        printf(1, "sed: read error\n");
        exit();
    }

    // print the final result to stdout
    printf(1,sed(buffer.base, str1, str2));

    exit();
}

// sed() function that replaces the old three character string with the new character string
char* sed(char *string, char *str1[], char *str2[]){
    char *result;
    int i, numElem = 0;
    int str1Len = strlen(*str1);
    int str2Len = strlen(*str2);

    // count time the old strin appears
    for (i = 0; string[i] != '\0'; i++){
        if (strstr(&string[i], *str1) == &string[i]){
            numElem++;
            // jump to index after the old string
            i += str1Len - 1;
        }
    }

    // make new string with same length
    result = (char*)malloc(i + numElem * (str2Len - str1Len) + 1);

    i = 0;

    // compare substring with result
    while(*string){
        if (strstr(string, *str1) == string){
            strcpy(&result[i], *str2);
            i += str2Len;
            string += str1Len;
        }
        else{
            result[i++] = *string++;
        }
    }

    result[i] = '\0';

    return result;

}

// finds the first occurence of string b in string a 
// by returning a pointer to the beginning
char* strstr(char* a, char* b){
    if (*b == '\0'){
        return a;
    }

    for (int i = 0; i < strlen(a); i++){
        if (*(a+i) == *b){
            char* ptr = strstr(a + i + 1, b + 1);
            return (ptr) ? ptr - 1 : NULL;
        }    
    }
    return NULL;
}

// allocate buffer using the Buffer struct created
Buffer allocBuffer(int initialSize){
    return (Buffer){
        .base = malloc(initialSize),
        .length = 0,
        .capacity = initialSize,
    };
}

// read from file into buffer using a while loop
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