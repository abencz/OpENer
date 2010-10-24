void *memcpy(void *dest, const void *src, unsigned n)
	{
	char *d = dest;
	const char *s = src;

	while(n--)*d++ = *s++;

	return dest;
	}
