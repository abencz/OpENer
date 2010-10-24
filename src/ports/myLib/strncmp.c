int strncmp(const char *s1, const char *s2, unsigned n)
	{
	int res=0;
	int i=0;

	while(i<n && (res=*s1-*s2)==0 && *s1 && *s2)
		{
		s1++;
		s2++;
		i++;
		}

	return res;
	}
