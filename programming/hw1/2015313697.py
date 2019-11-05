#2015313697
# hwang yeong jun
#1
cost=int(input("Cost of a package: "))
num=int(input("Number of a packages: "))
if num>=0 and num<=9:
	print("Total amount",(cost*num)*1.0)
elif num>=10 and num<=19:
	print("Total amount",(cost*num)*0.9)
elif num>=20 and num<=39:
	print("Total amount",(cost*num)*0.8)
elif num>=0 and num<=0:
	print("Total amount",(cost*num)*0.7)
else:
	print("Total amount",(cost*num)*0.6)

#2
temp=1
while(temp==1):
	n=int(input("Please input a number between 1 and 10: "))
	if n==1:
		print("I")
	elif n==2:
		print("II")
	elif n==3:
		print("III")
	elif n==4:
		print("IV")
	elif n==5:
		print("V")
	elif n==6:
		print("VI")
	elif n==7:
		print("VII")
	elif n==8:
		print("VIII")
	elif n==9:
		print("IX")
	elif n==10:
		print("X")
	else:
		print("End")
		temp=0;

#3
square=[[8,3,5],[1,5,9],[6,7,2]]
count=[0]*9
check=0
temp1=0
#test 1~9
while(temp1<3):
	temp2=0
	while(temp2<3):
		count[square[temp1][temp2]-1]=1
		temp2+=1
	temp1+=1
temp=0
while(temp<9 and check==0):
	if(count[temp]!=1):
		print("The square is not Lo shu Magic Square")
		check=1
	temp+=1
#test row
temp1=0
while(temp1<3 and check==0):
	ssum=0
	temp2=0
	while(temp2<3):
		ssum+=square[temp1][temp2]
		temp2+=1
	if(ssum!=15):
		print("The square is not Lo shu Magic Square")
		check=1
	temp1+=1
#test col
temp1=0
while(temp1<3 and check==0):
	ssum=0
	temp2=0
	while(temp2<3):
		ssum+=square[temp2][temp1]
		temp2+=1
	if(ssum!=15):
		print("The square is not Lo shu Magic Square")
		check=1
	temp1+=1
#test diagonal
temp1=0
ssum=0
while(temp1<3 and check==0):
	ssum+=square[temp1][temp1]
	temp1+=1
if(ssum!=15 and check==0):
	print("The square is not Lo shu Magic Square")
	check=1

temp1=0
ssum=0
while(temp1<3 and check==0):
	ssum+=square[2-temp1][2-temp1]
	temp1+=1
if(ssum!=15 and check==0):
	print("The square is not Lo shi Magic Square")
	check=1

if(check==0):
	print("The square is Lo shu Magic Square")

		
