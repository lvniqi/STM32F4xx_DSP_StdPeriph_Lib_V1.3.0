from math import pi,sin
def fun(Vmax,size):
	a = []
	for x in range(size):
		a.append((int)((Vmax/2)+((Vmax-1)/2)*sin(2*pi*x/size)))
	result = str(a)
	result = '{' + result[1:]
	result = result[:-1] + '}'
	return result
def change_color(in_data):
    t_r = 0
    t_r |= (in_data&0x0000f8)>>3
    t_r |= (in_data&0x00f800)>>6
    t_r |= (in_data&0xf00000)>>10
    return hex(t_r)

if __name__ == "__main__":
    temp = open("temp123.c",'w')
    print>>temp,fun(4096,32)
    
