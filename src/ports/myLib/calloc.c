extern void *malloc(int s);
extern void *memset(void *dest, int c, unsigned n);

void *calloc(unsigned num, unsigned size)
	{
	char *tmp;

	tmp = malloc(num*size);
	memset(tmp,0,num*size);
	return tmp;
	}

