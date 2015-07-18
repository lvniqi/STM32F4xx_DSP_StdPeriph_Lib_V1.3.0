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

def LCR_Calu(freq,size,Rmin):
    """L*C = (2*pi*freq)**2
    Q = (W)*L/R
    R>>0
    L>>1**10-6
    C>>1*10-12"""
    Q = freq*1.0/size
    W = 2*pi*freq
    for L in range(1,1000):
        L_t = L*1.0*10**-6
        C = (1/W)**2/L_t
        R = W*L_t/Q
        if C <1*10**-12 or C>10**-6:
            continue
        elif R<Rmin:
            continue
        else:
            print "L:  ",L_t
            print "C:  ",C
            print "R:  ",R
            return [L_t,C,R]
    return False

if __name__ == "__main__":
    #temp = open("temp123.c",'w')
    #print>>temp,fun(4096,32)
    
    for x in range(0,30):
        if LCR_Calu(21000,4000,x):
            continue
        else:
            print x-1
            break

