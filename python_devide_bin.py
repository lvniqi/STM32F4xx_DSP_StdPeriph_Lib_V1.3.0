import struct 
def devide_str(str,length):
    list_t = []
    while str != "":
        str_temp = str[:length*4]
        while len(str_temp) <length*4:
            str_temp += struct.pack('L',0)
        list_t.append(str_temp)
        str = str[length*4:]
    return list_t
def devide_file(file1,length):
    index = 0
    file1.seek(0)
    str_t = file1.read()
    list_t = devide_str(str_t,length)
    for x in list_t:
        file2 = open("Project_d_%d.bin"%(index),'wb')
        file2.write(x)
        file2.close()
        index += 1;
        
def fun(str,length):
    list_t = devide_str(str,length)
    struct_Str = ""
    for x in range(length):
        struct_Str += "L"
    struct_t = struct.Struct(struct_Str)
    t4 = struct_t.unpack(list_t[0])
if __name__ == "__main__":
    temp = open("Project.bin",'rb')
    devide_file(temp,256)
    
