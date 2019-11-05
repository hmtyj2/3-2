#include <bits/stdc++.h>

using namespace std;

int ans[3][3]={{1,2,3},{4,5,6},{7,8,0}};

struct puzzle_str{
	int puz_map[3][3];
	int row;
	int col;
	int len;
};

int dir[4][2]={{-1,0},{0,-1},{1,0},{0,1}};

int cal(struct puzzle_str p){
	int count=0;
	for(int temp=0;temp<3;temp++){
		for(int temp1=0;temp1<3;temp1++){
			if(p.puz_map[temp][temp1]!=0 && p.puz_map[temp][temp1]!=ans[temp][temp1]){
				int row=(p.puz_map[temp][temp1]-1)/3;
				int col=(p.puz_map[temp][temp1]-1)%3;
				if(row-temp>0){
					count+=row-temp;
				}
				else{
					count+=temp-row;
				}
				if(col-temp1>0){
					count+=col-temp1;
				}
				else{
					count+=temp1-col;
				}
			}
		}
	}
	return count;
}

bool operator < (puzzle_str p1, puzzle_str p2){
	return p1.len+cal(p1)>p2.len+cal(p2);
}

int compare(struct puzzle_str p1, struct puzzle_str p2){
	if(p1.row!=p2.row || p1.col!=p2.col){
		return 0;
	}
	for(int temp=0;temp<3;temp++){
		for(int temp1=0;temp1<3;temp1++){
			if(p1.puz_map[temp][temp1]!=p2.puz_map[temp][temp1]){
				return 0;
			}
		}
	}
	return 1;	// same struct
}

int test(struct puzzle_str p){
	int temp,temp1;
	for(temp=0;temp<3;temp++){
		for(temp1=0;temp1<3;temp1++){
			if(temp==2 && temp1==2){
				return 1;
			}
			else{
				if(p.puz_map[temp][temp1]!=temp*3+(temp1+1)){
					return 0;
				}
			}
		}
	}
}

int main(){
	int temp,temp1,temp2;
	int visit=0;
	int open=0;
	puzzle_str p_init,p_cur;

	for(temp=0;temp<3;temp++){		// make puzzle_str map based on the input
		for(temp1=0;temp1<3;temp1++){
			scanf("%d",&p_init.puz_map[temp][temp1]);
			if(p_init.puz_map[temp][temp1]==0){
				p_init.row=temp;
				p_init.col=temp1;
				p_init.len=1;
			}
		}
	}
	priority_queue<puzzle_str> my_queue;
	vector<puzzle_str> visited;
	my_queue.push(p_init);
	visited.push_back(p_init);
	while(!my_queue.empty()){			/////////////
		visit++;		// calculate visit and open
		if(my_queue.size()>open){
			open=my_queue.size();
		}
		p_cur=my_queue.top();
		my_queue.pop();
		printf("visit:%d len:%d\n",visit,p_cur.len);	/////////////
		printf("row:%d col:%d\n",p_cur.row,p_cur.col);
		for(int temp=0;temp<3;temp++){
			for(int temp1=0;temp1<3;temp1++){
				printf("%d ",p_cur.puz_map[temp][temp1]);
			}
			printf("\n");
		}
		printf("\n");				///////////////
		if(p_cur.puz_map[2][2]==0){		// test if current position is goal
			int t=test(p_cur);
			if(t==1){
				printf("visit=%d open=%d len=%d",visit,open,p_cur.len);
				return 0;
			}
		}
		for(int temp=0;temp<4;temp++){
			int cur_row=p_cur.row+dir[temp][0];
			int cur_col=p_cur.col+dir[temp][1];
			if(cur_row>=0 && cur_row<=2 && cur_col>=0 && cur_col<=2){
				puzzle_str p_temp;
				memcpy(&p_temp,&p_cur,sizeof(struct puzzle_str));
				p_temp.puz_map[p_cur.row][p_cur.col]=p_temp.puz_map[cur_row][cur_col];				
				p_temp.puz_map[cur_row][cur_col]=0;
				p_temp.row=cur_row;
				p_temp.col=cur_col;
				p_temp.len=p_cur.len+1;
				for(int temp1=0;temp1<visited.size();temp1++){
					if(compare(visited[temp1],p_temp)==1){
						break;
					}
					else if(temp1==visited.size()-1){
						my_queue.push(p_temp);
						visited.push_back(p_temp);
					}
				}
			}
		}
	}
}