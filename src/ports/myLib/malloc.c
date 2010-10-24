// primitive malloc

void *sbrk(int);

void *malloc(int s)
	{
	return sbrk(s);
	}


