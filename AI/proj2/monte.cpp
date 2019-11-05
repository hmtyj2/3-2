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
	struct Node *parent;
	std::vector<Node*> child;
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

	if(check(board)!=0){
		printf("win:%d\n",check(board));
		print(board);
		return check(board);
	}
	if(full(board)==1){
		return 0;
	}
	if(node->turn==-1){		// next move is players
		if(node->child.size()==0){	//first visit
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
						node1->parent=node;
						node->child.push_back(node1);
					}
				}
			}
			return rand_simul(board);
		}
		else{		// not first visit
			double t1=0;
			int temp1;
			for(int temp=0;temp<(node->child.size());temp++){	// if unvisited
																		//child exist
				if(node->child[temp]->UCT==-1){
					board[node->child[temp]->row][node->child[temp]->col]='o';
					node->child[temp]->visit+=1;
					int t=monte(node->child[temp],board);
					if(t==1){
						node->child[temp]->win+=1;
					}
					node->child[temp]->UCT=(node->child[temp]->win/
						node->child[temp]->visit +
						sqrt(2*log((double)node->visit/node->child[temp]->visit)));
					
					board[node->child[temp]->row][node->child[temp]->col]='_';
					return t;
				}
			}
			for(int temp=0;temp<(node->child.size());temp++){	//if all visited
				if(node->child[temp]->UCT>t1){		// find bigist UCT val
					t1=node->child[temp]->UCT;
					temp1=temp;
				}
			}
			board[node->child[temp1]->row][node->child[temp1]->col]='o';
			node->child[temp1]->visit+=1;
			int t=monte(node->child[temp1],board);
			if(t==1){
				node->child[temp1]->win+=1;
			}
			node->child[temp1]->UCT=(node->child[temp1]->win/
			node->child[temp1]->visit +
			sqrt(2*log((double)node->visit/node->child[temp1]->visit)));		
			board[node->child[temp1]->row][node->child[temp1]->col]='_';
			return t;
		}
	}
	else{		// next move is enemies
		if(node->child.size()==0){	//first visit
			for(int temp=0;temp<n;temp++){
				for(int temp1=0;temp1<n;temp1++){
					if(board[temp][temp1]=='_'){
						Node *node1=(Node*)malloc(sizeof(Node));
						node1->turn=-1;
						node1->row=temp;
						node1->col=temp1;
						node1->visit=1;
						node1->win=0;
						//double UCT=-1;
						node1->parent=node;
						node->child.push_back(node1);
					}
				}
			}
			return rand_simul(board);	///////???????
		}
		else{		// not first visit
			int temp=rand()%(node->child.size());
			board[node->child[temp]->row][node->child[temp]->col]='x';
			node->child[temp]->visit+=1;
			int t=monte(node->child[temp],board);
			board[node->child[temp]->row][node->child[temp]->col]='_';
			return t;
		}
	}
}

int main(void){
	// 4x4 board
	char board[n][n]={
		{'_','_','_','_'},
		{'_','_','_','_'},
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
	root->parent=NULL;
	for(int temp=0;temp<30;temp++){
		monte(root,board);
	}
	for(int temp=0;temp<root->child.size();temp++){
		printf("row: %d col: %d visit:%d win:%d\n",
			root->child[temp]->row,root->child[temp]->col,
			root->child[temp]->visit,root->child[temp]->win);
	}
	printf("total visit: %d\n",visit);
}