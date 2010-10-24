unsigned strlen(const char *p);

char *strstr(const char *haystack, const char *needle)
	{
	int i;
	int j;
	int m = strlen(needle);
	int n=strlen(haystack)-m;

	for(i=0;i<=n;i++)
		{
		for(j=0; j<m && haystack[i+j]==needle[j]; j++);
		if(j==m)return (char *)&haystack[i];
		}

	return 0;
	}
