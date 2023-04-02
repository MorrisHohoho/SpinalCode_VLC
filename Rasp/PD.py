import smbus
import platform
import time
import numpy as np
from SymbolMapper import  SymbolMapper
from Encoder import Encoder
from Decoder import Decoder


DEBUG_TIME = 0      #set to 1 to show time cost.

address = 0x48  #PCF8591 base address
A0 = 0x40       #A0 output address
bus = smbus.SMBus(1)


# Keep the same message format as the implementation in car
#message format: [0101](header) + [valid bits](12bits)+[10](tail)
mes =[]
perPassLen = 8
recBuf =[]

highThres = 2.5  #high signal
lowThres = 0.3  #low signal
normalThres = 1.000   #no signal

highStart = False
lowStart = False
mesStart = False

one2twoThres = 0.0007   #identify one or two bits are received


# Spinal Code Encoding
c=6
k=4
precision=c
B=4
d=2
passes=3

mapper = SymbolMapper(c, precision)
map_func = lambda value: mapper.map(value)


def splitSymbols(mes,passlenth,symbolLen):
    """
    Split up the received signal into array of symbols in manchester encode.

    Args:
        mes: received signal
        passlenth: the lenth of each pass
        symbolLen: the lenth of each symbol, which is equal to precision.
    Returns:
        array of symbols which are encoded in manchester and in binary.
        example:
        [[0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0], [1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0], ...]
    """    
    symbols=[]
    totalLen =passlenth*symbolLen*2
    tmp = mes[4:len(mes)-1] #signal without header and tail
    #print(tmp)
    if(len(tmp)<totalLen):
        print("this pass is invaild!")
    else:
            for i in range(passlenth):
                symbols.append(tmp[i*symbolLen*2:i*symbolLen*2+symbolLen*2])
    #print(symbols)
    return symbols
    

def manchesterDecode(symbols):
    """
    Manchester decoder.

    Args:
        symbols: 2D array of symbols encoded in manchester.
    Returns:
        2D array of original symbols in binary.
        example:
        [[1, 0, 1, 1, 1, 1], [1, 0, 1, 1, 1, 0], ...]
    """  
    decodeMes = []
    for i in symbols:
        decodeMes.append(i[1::2])
    return decodeMes

def binToDecimal(symbols):
    """
    Convert binary symbols to decimal symbols.

    Args:
        symbols: 2D array of symbols in binary.
        passlenth: the lenth of each pass
    Returns:
        2D array of original symbols in decimal
        example: 
        passlenth = 4
        [34, 48, 4,3]
    """ 
    res=[]
    tmp = np.array(symbols)
    for i in tmp:
        res.append(i.dot(2**np.arange(i.size)[::-1]))
    return res

def spinalDecode(symbols):
    """
    Spinal code decoder.

    Args:
        symbols: 2D array of symbols in decimal.
        example:
        [[1,2,3],[3,1,2],[3,3,3]]
        (pass1),(pass2),(pass3)
    Returns:
        decoed message in ASCII.
    """
    decoder = Decoder(k, B, d, map_func)
    for i in range(perPassLen):
        tmp =[]
        for mes in symbols:
            tmp.append(mes[i])
        decoder.advance(tmp)
    decoded_message = decoder.get_most_likely()
    print("decoded hex:", decoded_message.encode().hex())
    print("message decoded: ", decoded_message)
    return decoded_message


def initMes():
    """
    Initialize flag and global variable.
    """
    global highStart
    global lowStart
    global mesStart
    global mes
    
    highStart=False
    lowStart=False
    mesStart=False
    mes.clear()
    
    



'''
main process for decode message.
'''
while True:
    bus.write_byte(address,A0)                  #Get value from the A0 Channel of PCF8591
    value = 5*bus.read_byte(address)/255;       #Reference Voltage is 5V，and the precision is 8 bits.
    
    
    #The voltage of PD returns to normalThres(approximately 1V)
    if(abs(value-normalThres)<=0.1 and mes):
        #print(mes)
        
        machesterSymbols = splitSymbols(mes,perPassLen,precision)
        
        if DEBUG_TIME:
            T_MesEnd = time.perf_counter()
            print("get symbols",T_MesEnd-T_MesStart)
        
        if(machesterSymbols):
            
            if DEBUG_TIME:
                T_StartMache=time.perf_counter();
                
            symbolsInBin = manchesterDecode(machesterSymbols)
            
            if DEBUG_TIME:
                T_EndMache=time.perf_counter();
                print("decode manchester:",T_EndMache-T_StartMache)
            
            #print(symbolsInBin)
            if DEBUG_TIME:
                T_StartBin=time.perf_counter()
                
            symbols = binToDecimal(symbolsInBin)
            
            if DEBUG_TIME:
                T_EndBin=time.perf_counter()
                print("bin to dec",T_EndBin-T_StartBin)
            
            recBuf.append(symbols)
            print("get pass",len(recBuf),symbols)
            if(len(recBuf)>=passes):
                if DEBUG_TIME:
                    T_StartSpinal=time.perf_counter()
                    
                decodedMes=spinalDecode(recBuf)
                
                if DEBUG_TIME:
                    T_EndSpinal = time.perf_counter()
                    print("Spinal decode:", T_EndSpinal-T_StartSpinal)
                    
                recBuf.clear()
            
        initMes()
        
    '''
    Get symbols from signal in OOK.
    Start!
    '''
    if(value<lowThres and not lowStart):
        
        if DEBUG_TIME:
            T_MesStart = time.perf_counter()
            
        lowStart=True
        mesStart=True
        T1 = time.perf_counter()
        if(highStart):
            T4 = time.perf_counter()
            highLast = T4-T3
            #print("high",highLast)
            if(abs(highLast-0.001)>=one2twoThres):
                mes.append(1)
                mes.append(1)
                #print("get 11");
            elif(abs(highLast-0.001)<one2twoThres):
                mes.append(1)
                #print("get 1");
            highStart=False
            
    if(value>highThres and not highStart and mesStart):
        T3 = time.perf_counter()
        if(lowStart):
            T2 =time.perf_counter()
            lowLast = T2-T1
            
            #print("low",lowLast)
            if(abs(lowLast-0.001)>=one2twoThres):
                mes.append(0)
                mes.append(0)
                #print("get 00");
            elif(abs(lowLast-0.001)<one2twoThres):
                mes.append(0)
                #print("get 0");
        lowStart=False
        highStart=True
    
    '''
    Get symbols from signal in OOK.
    End!
    '''
        
    