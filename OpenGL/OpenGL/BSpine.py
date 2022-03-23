
import math
from xml.etree.ElementTree import PI
from PIL import Image
from numpy import mat
from pylab import *
def B_spline(p_list):
	"""
	:param p_list: (list of list of int:[[x0, y0], [x1, y1], ...])point set of p
	result: (list of list of int:[[x0, y0], [x1, y1], ...])point on curve
	绘制三次(四阶)均匀B样条曲线
	"""
	result = []
	n = len(p_list)
	k = 0
	u = k-1
	while (u < n+1):
		x, y = 0, 0
		#calc P(u)
		for i in range(0, n):
			B_ik = deBoor_Cox(u, k, i)
			x += B_ik * p_list[i][0]
			y += B_ik * p_list[i][1]
		result.append((int(x+0.5), int(y+0.5)))
		u += 1/10 #2020/09/27
	return result

def deBoor_Cox(u, k, i):
	if k==1:
		if i <= u and u <= i+1:
			return 1
		else:
			return 0
	else:
		coef_1, coef_2 = 0, 0
		coef_3=0
		
		if (u-i == 0) and (i+k-1-i == 0):
			coef_1 = 0
		else:
			coef_1 = (u-i) / (i+k-1-i)
		if (i+k-u == 0) and (i+k-i-1 == 0):
			coef_2 = 0
		else:
			coef_2 = (i+k-u) / (i+k-i-1)
		if (i+k-1-i == 0):
			coef_1 = 0
			coef_2 = 0
	return coef_1 * deBoor_Cox(u, k-1, i) + coef_2 * deBoor_Cox(u, k-1, i+1)
input = [[-7,0],[9.0,0],[16,8],[18.0,10],[20,13],[22,0],[27,0],[28,0]]
input = [[0,0],[11.0,0],[17,8],[24.0,10],[26,13],[28,0],[40,0],[45,0]]

n = len(input)


k = 4#阶数
u = float(k-1+0.001)
count=3
set1=0
while(1):
	x1 = []
	y1 = []
	x2 = []
	y2 = []		
	input[3][1]+=1.0
	
	for i in range(0, n):
		x2.append(input[i][0])
		y2.append(input[i][1])
		temp=0
	while (u <= n):
		x, y = 0, 0
		#calc P(u)
		for i in range(0, n):
				B_ik = deBoor_Cox(u, k, i)
				x += B_ik * input[i][0]
				y += B_ik * input[i][1]
		#temp+=abs(u-(n+k-1-0.001)/2.0-1.0)		
		#x=(u-(n+k-1-0.001)/2.0)
		#x=(1-abs(pow(((u-n)/(n-k+1)),2)))*(n-k+1)
		#temp1=(u-n)/(n-k+1)/4.0
		#x=math.sin(temp1*math.pi)*(n-k+1)
		#x=u
		x1.append((x))
		y1.append((y))
		u += (n-k+1)/64.0 #2020/09/27 我有64个非均匀的采样点,如何映射到uv,答案是非均匀采样可以再康康采样定理啥的
		#u+=0.001
	print(x1)
	print(y1)
	plt.plot(x1,y1)
	plt.plot(x1,y1,'bo',ms=5)
	#plt.plot(x2,y2)
	for i in range(0, n):
			B_ik = deBoor_Cox(2, k, i)
			print(B_ik)
	plt.show()
	u = float(k-1+0.001)
        



