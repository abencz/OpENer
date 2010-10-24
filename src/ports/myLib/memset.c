void *memset(void *dest, int c, unsigned n)
	{
	char *d = dest;

	while(n--)*d++ = c;

	return dest;	
	}
