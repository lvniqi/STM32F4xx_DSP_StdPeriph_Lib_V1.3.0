import struct 
import serial
from time import sleep
class Crc4stm32:
    Crc32Table = [0x0,0x4c11db7,0x9823b6e,0xd4326d9,0x130476dc,0x17c56b6b,0x1a864db2,0x1e475005,0x2608edb8,0x22c9f00f,0x2f8ad6d6,0x2b4bcb61,0x350c9b64,0x31cd86d3,0x3c8ea00a,0x384fbdbd,0x4c11db70,0x48d0c6c7,0x4593e01e,0x4152fda9,0x5f15adac,0x5bd4b01b,0x569796c2,0x52568b75,0x6a1936c8,0x6ed82b7f,0x639b0da6,0x675a1011,0x791d4014,0x7ddc5da3,0x709f7b7a,0x745e66cd,0x9823b6e0L,0x9ce2ab57L,0x91a18d8eL,0x95609039L,0x8b27c03cL,0x8fe6dd8bL,0x82a5fb52L,0x8664e6e5L,0xbe2b5b58L,0xbaea46efL,0xb7a96036L,0xb3687d81L,0xad2f2d84L,0xa9ee3033L,0xa4ad16eaL,0xa06c0b5dL,0xd4326d90L,0xd0f37027L,0xddb056feL,0xd9714b49L,0xc7361b4cL,0xc3f706fbL,0xceb42022L,0xca753d95L,0xf23a8028L,0xf6fb9d9fL,0xfbb8bb46L,0xff79a6f1L,0xe13ef6f4L,0xe5ffeb43L,0xe8bccd9aL,0xec7dd02dL,0x34867077,0x30476dc0,0x3d044b19,0x39c556ae,0x278206ab,0x23431b1c,0x2e003dc5,0x2ac12072,0x128e9dcf,0x164f8078,0x1b0ca6a1,0x1fcdbb16,0x18aeb13,0x54bf6a4,0x808d07d,0xcc9cdca,0x7897ab07,0x7c56b6b0,0x71159069,0x75d48dde,0x6b93dddb,0x6f52c06c,0x6211e6b5,0x66d0fb02,0x5e9f46bf,0x5a5e5b08,0x571d7dd1,0x53dc6066,0x4d9b3063,0x495a2dd4,0x44190b0d,0x40d816ba,0xaca5c697L,0xa864db20L,0xa527fdf9L,0xa1e6e04eL,0xbfa1b04bL,0xbb60adfcL,0xb6238b25L,0xb2e29692L,0x8aad2b2fL,0x8e6c3698L,0x832f1041L,0x87ee0df6L,0x99a95df3L,0x9d684044L,0x902b669dL,0x94ea7b2aL,0xe0b41de7L,0xe4750050L,0xe9362689L,0xedf73b3eL,0xf3b06b3bL,0xf771768cL,0xfa325055L,0xfef34de2L,0xc6bcf05fL,0xc27dede8L,0xcf3ecb31L,0xcbffd686L,0xd5b88683L,0xd1799b34L,0xdc3abdedL,0xd8fba05aL,0x690ce0ee,0x6dcdfd59,0x608edb80,0x644fc637,0x7a089632,0x7ec98b85,0x738aad5c,0x774bb0eb,0x4f040d56,0x4bc510e1,0x46863638,0x42472b8f,0x5c007b8a,0x58c1663d,0x558240e4,0x51435d53,0x251d3b9e,0x21dc2629,0x2c9f00f0,0x285e1d47,0x36194d42,0x32d850f5,0x3f9b762c,0x3b5a6b9b,0x315d626,0x7d4cb91,0xa97ed48,0xe56f0ff,0x1011a0fa,0x14d0bd4d,0x19939b94,0x1d528623,0xf12f560eL,0xf5ee4bb9L,0xf8ad6d60L,0xfc6c70d7L,0xe22b20d2L,0xe6ea3d65L,0xeba91bbcL,0xef68060bL,0xd727bbb6L,0xd3e6a601L,0xdea580d8L,0xda649d6fL,0xc423cd6aL,0xc0e2d0ddL,0xcda1f604L,0xc960ebb3L,0xbd3e8d7eL,0xb9ff90c9L,0xb4bcb610L,0xb07daba7L,0xae3afba2L,0xaafbe615L,0xa7b8c0ccL,0xa379dd7bL,0x9b3660c6L,0x9ff77d71L,0x92b45ba8L,0x9675461fL,0x8832161aL,0x8cf30badL,0x81b02d74L,0x857130c3L,0x5d8a9099,0x594b8d2e,0x5408abf7,0x50c9b640,0x4e8ee645,0x4a4ffbf2,0x470cdd2b,0x43cdc09c,0x7b827d21,0x7f436096,0x7200464f,0x76c15bf8,0x68860bfd,0x6c47164a,0x61043093,0x65c52d24,0x119b4be9,0x155a565e,0x18197087,0x1cd86d30,0x29f3d35,0x65e2082,0xb1d065b,0xfdc1bec,0x3793a651,0x3352bbe6,0x3e119d3f,0x3ad08088,0x2497d08d,0x2056cd3a,0x2d15ebe3,0x29d4f654,0xc5a92679L,0xc1683bceL,0xcc2b1d17L,0xc8ea00a0L,0xd6ad50a5L,0xd26c4d12L,0xdf2f6bcbL,0xdbee767cL,0xe3a1cbc1L,0xe760d676L,0xea23f0afL,0xeee2ed18L,0xf0a5bd1dL,0xf464a0aaL,0xf9278673L,0xfde69bc4L,0x89b8fd09L,0x8d79e0beL,0x803ac667L,0x84fbdbd0L,0x9abc8bd5L,0x9e7d9662L,0x933eb0bbL,0x97ffad0cL,0xafb010b1L,0xab710d06L,0xa6322bdfL,0xa2f33668L,0xbcb4666dL,0xb8757bdaL,0xb5365d03L,0xb1f740b4L]
    def __init__(self):
        self.reset()
    def reset(self):
        self.nReg = 0xFFFFFFFF
    def addData(self,pData):#CRC register
        if type(pData)== list:
            for n in range(len(pData)):
                self.nReg ^= pData[n]
                for i in range(4):
                    nTemp = Crc4stm32.Crc32Table[(self.nReg>>24)&0xff]
                    self.nReg <<= 8
                    self.nReg ^= nTemp
                    self.nReg&=0xffffffff
        elif type(pData)== int:
            for i in range(4):
                nTemp = Crc4stm32.Crc32Table[(self.nReg>>24)&0xff]
                self.nReg <<= 8
                self.nReg ^= nTemp
                self.nReg&=0xffffffff
        else:
            print "ERROR TYPE:",type(pData)
            raise ValueError
        return self.nReg
        
def devide_str(str,length):
    list_t = []
    while str != "":
        str_temp = str[:length*4]
        while len(str_temp) <length*4:
            str_temp += struct.pack('B',0)
        list_t.append(str_temp)
        str = str[length*4:]
    return list_t
def devide_file(file1,length):
    index = 0
    file1.seek(0)
    str_t = file1.read()
    list_t = devide_str(str_t,length)
    crc = Crc4stm32()
    list_result = []
    for x in list_t:
        data = str2long_list(x,length)
        data = list(data)
        crc.reset()
        crc_data = crc.addData(data)
        x += struct.pack('L',crc_data)
        list_result.append(x)
        file2 = open("Project_d_%d.bin"%(index),'wb')
        file2.write(x)
        file2.close()
        index += 1;
    return list_result
def usart_community(file1,length):
    usart = serial.Serial('com1',115200)
    while usart.readline() != "Now You Can Start Flash.\r\n":
        pass
    print 'ready to upload!'
    list_t = devide_file(file1,length)
    for x in list_t:
        usart.write(x)
        while True:
            s = usart.readline()
            print s
            if "get block at" in s:
                break
    print 'now start listening!'
    while True:
        print usart.readline()
        
def fun(str,length):
    list_t = devide_str(str,length)
    struct_Str = ""
    for x in range(length):
        struct_Str += "L"
    struct_t = struct.Struct(struct_Str)
    t4 = struct_t.unpack(list_t[0])
    
def str2long_list(str,length):
    struct_Str = ""
    for x in range(length):
        struct_Str += "L"
    struct_t = struct.Struct(struct_Str)
    return struct_t.unpack(str)
    
if __name__ == "__main__":
    temp = open("Project.bin",'rb')
    a = Crc4stm32()
    devide_file(temp,256)
    usart_community(temp,256)