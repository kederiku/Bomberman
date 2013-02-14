#ifndef _ID_STRING_H_
#define _ID_STRING_H_

#include <string.h>

char*	id_strcpy(char* dest, const char* src);
char*	id_strncpy(char* str, const char* src, int size);
int	id_strcmp(const char* s1, const char* s2);
int	id_strncmp(const char* s1, const char* s2, size_t size);
size_t	id_str_len(const char* str);
size_t	id_str_len(const char* str, const char* charset);
void*	id_memset(void* ptr, int value, size_t num);
char*	id_strdup(const char* s1);
char*	id_strndup(const char* s1, size_t size);
void	id_itoa(char* ret, int n);
int	id_atoi(char* str);
double	id_atof(const char* str);
int	base64(char* dest, const char* src, int len);
int	id_atoi(char* str);

#endif
