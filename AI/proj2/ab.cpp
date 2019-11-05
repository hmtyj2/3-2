#include <bits/stdc++.h>
#define n 4		//// choose 3 or 4

using namespace std;
int visit=0;
struct result{
	int row;
	int col;
	int expected_result;
};
int full(char board[n][n]){
	for(int temp=0;temp<n;temp++){
		for(int temp1=0;temp1<n;temp1++){
			if(board[temp][temp1]=='_'){
				return 0;
			}
		}
	}
	return 1;
}
void print(char board[n][n]){
	for(int temp=0;temp<n;temp++){
		for(int temp1=0;temp1<n;temp1++){
			printf("%c ",board[temp][temp1]);
		}
		printf("\n");
	}
}
int check(char board[n][n]){
	int temp,temp1;
	for(temp=0;temp<n;temp++){		//horizental
		char cur=board[temp][0];
		for(temp1=0;temp1<n;temp1++){
			if(board[temp][temp1]!=cur){
				break;
			}
			if(temp1==n-1){
				if(cur=='o'){
					return 1;
				}
				else if(cur=='x'){
					return -1;
				}
			}
		}
	}
	for(temp=0;temp<n;temp++){		//vertical
		char cur=board[0][temp];
		for(temp1=0;temp1<n;temp1++){
			if(board[temp1][temp]!=cur){
				break;
			}
			if(temp1==n-1){
				if(cur=='o'){
					return 1;
				}
				else if(cur=='x'){
					return -1;
				}
			}
		}
	}
	for(temp=0;temp<n;temp++){		//diagonal
		char cur=board[0][0];
		if(board[temp][temp]!=cur){
			break;
		}
		if(temp==n-1){
				if(cur=='o'){
					return 1;
				}
				else if(cur=='x'){
					return -1;
				}
			}
	}
	for(temp=0;temp<0;temp++){
		char cur=board[n-1][0];
		if(board[n-temp-1][temp]!=cur){
			break;
		}
		if(temp==n-1){
				if(cur=='o'){
					return 1;
				}
				else if(cur=='x'){
					return -1;
				}
			}
	}
	return 0;
}
void clean(char board[n][n]){
	for(int temp=0;temp<n;temp++){
		for(int temp1=0;temp1<n;temp1++){
			board[temp][temp1]='_';
		}
	}
}
int minimax(char board[n][n],int count,int turn, int alpha, int beta){
	visit++;
	int t;
	if((t=check(board))!=0 && count!=0){
		return t;
	}
	if((t=full(board))==1){
		return 0;
	}
	int M,m;
	int row,col;
	if(turn==1){
		M=-10;
		for(row=0;row<n;row++){
			for(col=0;col<n;col++){
				if(board[row][col]=='_'){
					board[row][col]='o';
					m=minimax(board,count+1,turn*(-1),max(M,alpha),beta);
					if(m>=beta){
						board[row][col]='_';
						return beta;
					}
					if(m>M){
						M=m;
					}
					board[row][col]='_';
				}
			}
		}
		return M;
	}
	else{
		M=10;
		for(row=0;row<n;row++){
			for(col=0;col<n;col++){
				if(board[row][col]=='_'){
					board[row][col]='x';
					m=minimax(board,count+1,turn*(-1),alpha,min(M,beta));
					if(m<=alpha){
						board[row][col]='_';
						return alpha;
					}
					if(m<M){
						M=m;
					}
					board[row][col]='_';
				}
			}
		}
		return M;
	}
}

result search(char board[n][n]){
	result r;
	int score=-100;
	int t;

	for(int temp=0;temp<n;temp++){
		for(int temp1=0;temp1<n;temp1++){
			if(board[temp][temp1]=='_'){
				board[temp][temp1]='o';
				t=minimax(board,1,-1,-10,10);
				if(t>score){
					r.row=temp;
					r.col=temp1;
					r.expected_result=t;
					score=t;
					print(board);
					printf("expected value: %d\n",t);
					printf("\n");
				}
				board[temp][temp1]='_';		
			}
		}
	}
	return r;
}

int main(){
	// 4x4 board
	char board[n][n]={
		{'_','_','_','_'},
		{'_','_','_','_'},
		{'_','_','_','_'},
		{'_','_','_','_'}
	};
	// 3x3 board
/*	char board[n][n]={
		{'x','_','_'},
		{'_','_','_'},
		{'_','_','_'}
	};*/
	result my=search(board);
	minimax(board,0,1,-100,100);
	printf("row: %d, col: %d, expected score: %d\n",
		my.row,my.col,my.expected_result);
	printf("total visit=%d\n",visit);
}