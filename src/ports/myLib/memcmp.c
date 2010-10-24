int memcmp(const void *vs1, const void *vs2, unsigned n)
	{
	int res=0;
	int i=0;
	const char *s1 = vs1;
	const char *s2 = vs2;

	while(i<n && (res=*s1-*s2)==0)
		{
		s1++;
		s2++;
		i++;
		}

	return res;
	}
