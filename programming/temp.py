def p(word):
	t=len(word)-1
	i=0
	while(i<=t):
		if(word[i]!=word[t]):
			return False
		i+=1
		t-=1
	return True



word1=input("input word: ");
print(p(word1))


