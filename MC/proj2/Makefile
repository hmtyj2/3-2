CC=g++

FFLAGS= -O3 -g -lm 
FFFLAGS= -O3 -g -lm -fopenmp

all: 3dew.cpp
	$(CC) $(FFLAGS) 3dew.cpp -o 3dew

test: test.cpp
	$(CC) $(FFLAGS) test.cpp -o test

ans: ans.cpp
	$(CC) $(FFFLAGS) ans.cpp -o ans

ans1: ans1.cpp
	$(CC) $(FFFLAGS) ans1.cpp -o ans1


verify:verify.cpp
	$(CC) -O3 -g verify.cpp -o verify -lm

clean:
	rm -f 3dew
	rm -f verify
