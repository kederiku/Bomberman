#include "string.h"
#include <new>
#include <iostream>

char*	id_strcpy(char* dest, const char* src)
{
	int	i;

	i = 0;
	while (src[i] != 0)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = 0;
	return dest;
}

char*	id_strncpy(char* str, const char* src, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		str[i] = src[i];
		++i;
	}
	return str;
}

int	id_strcmp(const char* s1, const char* s2)
{
	while (*s1 == *s2 && *s1 != 0 && *s2 != 0)
	{
		++s1;
		++s2;
	}
	return *s1 != *s2;
}

int	id_strncmp(const char* s1, const char* s2, size_t size)
{
	size_t	i;

	if (size == 0)
		return 0;
	i = 0;
	while (i + 1 < size && *s1 == *s2 && *s1 != 0 && *s2 != 0)
	{
		++s1;
		++s2;
		++i;
	}
	return  *s1 != *s2;
}

size_t	id_str_len(const char* str)
{
	size_t	i;

	if (str == 0)
		return 0;
	i = 0;
	while (*str != 0)
	{
		++str;
		++i;
	}
	return i;
}

size_t	id_str_len(const char* str, const char* charset)
{
	size_t	i;
	size_t	len;

	len = id_str_len(charset);
	i = 0;
	while (*str != 0 && strncmp(str, charset, len) != 0)
	{
		++str;
		++i;
	}
	return i;
}

void*	id_memset(void* ptr, int value, size_t num)
{
	size_t	i;
	char*	c;

	c = (char*)ptr;
	i = 0;
	while (i < num)
	{
		c[i] = value;
		++i;
	}
	return 0;
}

char*	id_strdup(const char* s1)
{
	size_t	i;
	char*	ret;

	i = 0;
	while (s1[i] != 0)
		++i;
	ret = new (std::nothrow) char[i + 1];
	if (ret == 0)
		return 0;
	i = 0;
	while (s1[i] != 0)
	{
		ret[i] = s1[i];
		++i;
	}
	ret[i] = 0;
	return ret;
}

char*	id_strndup(const char* s1, size_t size)
{
	size_t	i;
	char*	ret;

	i = 0;
	ret = new (std::nothrow) char[size + 1];
	if (ret == 0)
		return 0;
	i = 0;
	while (i < size && s1[i] != 0)
	{
		ret[i] = s1[i];
		++i;
	}
	ret[i] = 0;
	return ret;
}

void	id_itoa(char* ret, int n)
{
	int	i;
	int	a;

	a = 1;
	i = 0;
	while (n / a >= 10)
		a = a * 10;
	if (a == 1)
	{
		ret[0] = '0';
		++i;
	}
	while (a >= 1)
	{
		ret[i] = n / a % 10 + '0';
		++i;
		a = a / 10;
	}
	ret[i] = 0;
}

double	id_atof(const char* str)
{
	double	ret;
	int	signe;

	ret = 0;
	signe = 1;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			signe = signe * -1;
			str = str + 1;
		}
	}
	while ((*str >= '0' && *str <='9') || *str == ',' || *str == '.')
	{
		if (*str == ',' || *str == '.')
			ret = '.';
		else
			ret = ret * 10 + *str - '0';
		str = str + 1;
	}
	return (signe * ret);
}

int	id_atoi(char* str)
{
	int	i;
	int	number;

	i = 0;
	number = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 + str[i] - '0';
		i = i + 1;
	}
	return number;
}
