#include <bits/stdc++.h>
#define n 4		//// board size: choose 3 or 4

using namespace std;

int visit=0;
struct Node{
	int turn;
	int row;
	int col;
	int visit;
	int win;
	double UCT;
	Node* child;
	Node* collegue;
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
int rand_simul(char board[n][n]){
	srand((unsigned int)time(NULL));
	char board1[n][n];
	for(int temp=0;temp<n;temp++){
		for(int temp1=0;temp1<n;temp1++){
			board1[temp][temp1]=board[temp][temp1];
		}
	}
	int count=1;
	while(count++){
		if(check(board1)!=0){
			return check(board1);
		}
		if(full(board1)==1){
			return 0;
		}
		while(1){
			int rnd=rand()%(n*n);
			if(board1[rnd/n][rnd%n]=='_'){
				if(count%2==1){
					board1[rnd/n][rnd%n]='o';
				}
				else{
					board1[rnd/n][rnd%n]='x';
				}
				break;
			}
		}
	}
}

int monte(Node* node,char board[n][n]){
	srand((unsigned int)time(NULL));
	visit++;
	node->visit++;

	if(check(board)!=0){
		printf("win:%d\n",check(board));
		print(board);
		return check(board);
	}
	if(full(board)==1){
		return 0;
	}
	if(node->turn==-1){		// next move is players
		if(node->child==NULL){	//first visit
			for(int temp=0;temp<n;temp++){
				for(int temp1=0;temp1<n;temp1++){
					if(board[temp][temp1]=='_'){
						Node *node1=(Node*)malloc(sizeof(Node));
						node1->turn=1;
						node1->row=temp;
						node1->col=temp1;
						node1->visit=0;
						node1->win=0;
						node1->UCT=-1;
						node1->child=NULL;
						node1->collegue=NULL;
						if(node->child==NULL){
							node->child=node1;
						}
						else{
							Node *cur=node->child;
							while(cur->collegue!=NULL){
								cur=cur->collegue;
							}
							cur->collegue=node1;
						}
					}
				}
			}
			return rand_simul(board);
		}
		else{		// not first visit
			double t1=0;
			int temp1;
			int count=0;
			Node *cur=node->child;
			while(cur!=NULL){
				count++;
				if(cur->UCT>t1){
					t1=cur->UCT;
					temp1=count;
				}
				if(cur->UCT==-1){
					board[cur->row][cur->col]='o';
					//cur->visit+=1;
					int t=monte(cur,board);
					if(t==1){
						cur->win+=1;
					}
					cur->UCT=((double)cur->win/(double)cur->visit)+
						sqrt(2*log((double)node->visit)/(double)cur->visit);
					board[cur->row][cur->col]='_';
					return t;
				}
				cur=cur->collegue;
			}
			cur=node->child;
			for(int temp=0;temp<temp1-1;temp++){
				cur=cur->collegue;
			}
			board[cur->row][cur->col]='o';
			//cur->visit+=1;
			int t=monte(cur,board);
			if(t==1){
				cur->win+=1;
			}
			board[cur->row][cur->col]='_';
			cur=node->child;
			while(cur!=NULL){
				if(cur->UCT!=-1){
					cur->UCT=((double)cur->win/(double)cur->visit)+
						sqrt(2*log((double)node->visit)/(double)cur->visit);
					}
				cur=cur->collegue;
			}
			return t;
		}
	}
	else{		// next move is enemies
		if(node->child==NULL){	//first visit
			for(int temp=0;temp<n;temp++){
				for(int temp1=0;temp1<n;temp1++){
					if(board[temp][temp1]=='_'){
						Node *node1=(Node*)malloc(sizeof(Node));
						node1->turn=-1;
						node1->row=temp;
						node1->col=temp1;
						node1->visit=1;
						node1->win=0;
						node1->UCT=-1;
						node1->child=NULL;
						node1->collegue=NULL;
						if(node->child==NULL){
							node->child=node1;
						}
						else{
							Node *cur=node->child;
							while(cur->collegue!=NULL){
								cur=cur->collegue;
							}
							cur->collegue=node1;
						}
					}
				}
			}
			return rand_simul(board);	///////???????
		}
		else{		// not first visit
			int temp=1;
			Node* cur=node->child;
			while(cur->collegue!=NULL){
				temp++;
				cur=cur->collegue;
			}
			int temp1=rand()%temp;
			for(cur=node->child,temp=0;temp<temp1;temp++){
				cur=cur->collegue;
			}
			board[cur->row][cur->col]='x';
			//cur->visit+=1;
			int t=monte(cur,board);
			board[cur->row][cur->col]='_';
			return t;
		}
	}
}
int main(void){
	// 4x4 board
	char board[n][n]={
		{'_','_','_','_'},
		{'_','x','_','_'},
		{'_','_','_','_'},
		{'_','_','_','_'}
	};
	// 3x3 board
/*	char board[n][n]={
		{'_','_','_'},
		{'_','_','_'},
		{'_','_','_'}
	};*/
	Node *root=(Node*)malloc(sizeof(Node));
	root->turn=-1;
	root->visit=1;
	root->child=NULL;
	root->collegue=NULL;
	for(int temp=0;temp<10000;temp++){
		monte(root,board);
	}
	Node *cur=root->child;
	while(cur!=NULL){
		printf("row: %d, col: %d, visit: %4d, win: %4d, UCT: %.3f, win-rate: %.3f\n",
			cur->row,cur->col,cur->visit,cur->win,cur->UCT,
			((double)cur->win/(double)cur->visit));
		cur=cur->collegue;
	}
	printf("total visit: %d\n",visit);
}