#ifndef DYNAMIC_STRING_LIBRARY
#define DYNAMIC_STRING_LIBRARY

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

typedef struct {
	char* str;
	size_t size;
	size_t memsize;
} string;

void clean_str(string** a) {
	free((*a)->str);
	free(*a);
}

#define strEmpty(strn) string* __attribute__((__cleanup__(clean_str))) strn = malloc(sizeof(*strn)); \
											strn->str = malloc(8);\
											strn->size = 0; \
											strn->memsize = 8;
											
#define strNew(strn, chrp) string* __attribute__((__cleanup__(clean_str))) strn = malloc(sizeof(*strn)); \
											strn->str = malloc(strlen(chrp)+1);\
											strcpy(strn->str, chrp);\
											strn->size = strlen(chrp);\
											strn->memsize = strn->size+1;
void strFree(string* a) {
	free(a->str);
	free(a);
}

#ifdef WIN32
void strUsableBytes(string* a) {
	printf("%zu\n", _msize(a->str));
}
#endif

void strPut(string* a) {
	puts(a->str);
}
void strPutln(string* a) {
	puts(a->str);
	putchar('\n');
}

char* strChr(string* arg) {
	return arg->str;
}

void strAdd(string* arg, char chr) {
    if (arg->size + 1 >= arg->memsize) {
        size_t new_memsize = (arg->memsize == 0 ? 8 : arg->memsize * 2);
        char* new_str = realloc(arg->str, new_memsize);
        arg->str = new_str;
        arg->memsize = new_memsize;
    }
    arg->str[arg->size] = chr;
    arg->size++;
    arg->str[arg->size] = '\0';
}

void strInput(string* arg) {
    
    char* input = NULL;
    size_t size = 0;
    ssize_t len = getline(&input, &size, stdin);
    
    
    arg->str = malloc(len + 1);
    arg->size = len;
    arg->memsize = len + 1;
    
   
    memcpy(arg->str, input, len + 1);
    
    free(input);
}

size_t strSubLit(string* arg, const char* chr) {
	size_t ret = arg->size+1;
	char* a = strstr(arg->str, chr);
	if (a != NULL)
		ret = (size_t)(a-arg->str);
	return ret;
}

size_t strSub(string* arg, string* sub) {
	size_t ret = arg->size+1;
	char* a = strstr(arg->str, sub->str);
	if (a != NULL)
		ret = (size_t)(a-arg->str);
	return ret;
}

void strRefresh(string* arg) {
	arg->size = strlen(arg->str);
	arg->memsize = _msize(arg->str);
}

char strIndex(string* arg, size_t ind) {
	return arg->str[ind];
}

void strReplace(string* arg, size_t ind, char chr) {
	arg->str[ind] = chr;
}

size_t strFind(string* arg, char chr) {
	char* ret;
	ret = strchr(arg->str, chr);
	return (size_t)(ret - arg->str);
}

void strReplaceChr(string* arg, char a, char b) {
	int l = strFind(arg, a);
	arg->str[l] = b;
}

size_t strSize(string* arg) {
	return arg->size;
}

void strReverse(string* arg) {
	strrev(arg->str);
}

char** strSplitc(string* arg, char chr) {
	
}

void strCpy(string* a, string* b) {
	if (a->size >= b->size) {
		strcpy(a->str, b->str);
		a->size = b->size;
	}
	else {
		a->str = realloc(a->str, b->memsize);
		a->size = b->size;
		strcpy(a->str, b->str);
		a->memsize = b->memsize;
	}
}
 
int strCmp(string* a, string* b) {
	return !strcmp(a->str, b->str);
}

int strForm(string* str, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    int size = vsnprintf(NULL, 0, format, args);
	
    if (size < 0) {
        va_end(args);
        return -1;
    }

    if (str->memsize < (size_t)size+1) {
        char* tmp = realloc(str->str, size + 1);
        if (tmp == NULL) {
            va_end(args);
            return -1;
        }
        str->str = tmp;
        str->memsize = size + 1;
    }

    int ret = vsnprintf(str->str, str->memsize, format, args);
    va_end(args);

    if (ret < 0) {
        return -1;
    }
    
    str->size = ret;

    return ret;
}

#endif
