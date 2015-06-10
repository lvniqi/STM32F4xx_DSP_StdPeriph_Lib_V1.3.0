from math import pi,sin
def fun(Vmax,size):
	a = []
	for x in range(size):
		a.append((int)((Vmax/2)+((Vmax-1)/2)*sin(2*pi*x/size)))
	result = str(a)
	result = '{' + result[1:]
	result = result[:-1] + '}'
	return result

if __name__ == "__main__":
    temp = open("temp123.c",'w')
    print>>temp,fun(4096,32)
    
