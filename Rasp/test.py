import smbus
import platform
import time

address = 0x48  #PCF8591 base address
A0 = 0x40       #A0 output address


highBegin = False
lowBegin = False
mes =[]

highThres = 2.5
lowThres = 0.2

lowCount=0;
f=open("record.txt","w+")
bus = smbus.SMBus(1)
while True:
    bus.write_byte(address,A0)
    #Reference Voltage is 5V，and the precision is 8 bits.
    value = 5*bus.read_byte(address)/255; 
    
    
    
    #####READ VLT ##########
    # f.write(str(value)+'\n')
    # if(value<lowThres or value > highThres):
    #     print("%.3f" % value)
    #     f.write(str(value)+'\n')
    
    
    ######READ LOW SAMPLE COUNT#####   
    # if(value<lowThres and not lowBegin):
    #     lowCount = lowCount+1
    #     lowBegin=True
    
    # if(value<lowThres and lowBegin):
    #     lowCount = lowCount+1
        
    # if(value>highThres and lowBegin):
    #     print(lowCount)
    #     lowCount=0
    #     lowBegin=False
        
        
    ######READ HIGH SAMPLE COUNT#####    
    # if(value>highThres and not lowBegin):
    #     lowCount = lowCount+1
    #     lowBegin=True
    
    # if(value>highThres and lowBegin):
    #     lowCount = lowCount+1
        
    # if(value<lowThres and lowBegin):
    #     print(lowCount)
    #     lowCount=0
    #     lowBegin=False
    
    
    
    
    ####READ HIGH TIMMING#####
    # if(value>highThres and not lowBegin):
    #     T1 =time.perf_counter()
    #     lowBegin=True
        
    # if(value<lowThres and lowBegin):
    #     T2 = time.perf_counter()
    #     print(T2-T1)
    #     lowBegin=False
        
      
    ###READ LOW TIMMING###  
    if(value<lowThres and not lowBegin):
        T1 =time.perf_counter()
        lowBegin=True
        
    if(value>highThres and lowBegin):
        T2 = time.perf_counter()
        print(T2-T1)
        lowBegin=False
    
    