daum=89000
naver=751000
total=100*daum+20*naver

print("5% fall: ",total-(100*daum+20*(1-0.05)*naver))
print("10% fall: ",total-(100*daum+20*(1-0.1)*naver))
