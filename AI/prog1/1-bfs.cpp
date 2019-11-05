#include <bits/stdc++.h>

using namespace std;

struct puzzle_str{
	int puz_map[3][3];
	int row;
	int col;
	int len;
};

int dir[4][2]={{-1,0},{0,-1},{1,0},{0,1}};

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
	queue<puzzle_str> my_queue;
	my_queue.push(p_init);
	while(!my_queue.empty()){			/////////////
		visit++;		// calculate visit and open
		if(my_queue.size()>open){
			open=my_queue.size();
		}
		p_cur=my_queue.front();
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
				my_queue.push(p_temp);
			}
		}
	}
}