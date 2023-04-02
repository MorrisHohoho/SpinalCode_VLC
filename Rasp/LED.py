#!/usr/bin/python
# -*- coding: UTF-8 -*-

from gpiozero import LED
from time import sleep
from SymbolMapper import  SymbolMapper
from Encoder import Encoder
from Decoder import Decoder

raspiFreq =0.001
hi3861Freq =0.005
Freq = raspiFreq
led = LED(16)


#message format
header=[0,1,0,1]
tail  = [1,0]

# Spinal Code Encoding
c=6
k=4
precision=c
B=4
d=2
passes=3

mapper = SymbolMapper(c, precision)
map_func = lambda value: mapper.map(value)
spine_length = 0

def ledBlink(finalMes):    
    """
    Make LED blink, and the modulation is OOK.

    Args:
        mes: message is like 01011010 which contained in a array.
    """
    a = 1
    for mes in finalMes:  
        print("Transmitting pass ",a)
        for bit in mes:
            if(bit==1):
                led.on()
                sleep(Freq)
            elif(bit==0):
                led.off()
                sleep(Freq) 
        led.on()       
        a=a+1  
        sleep(0.05)
    print("All pass are transmitted,input next message") 
    led.on()
    
def spinalEncode(mes):
    """
    Spinal Encode

    Args:
        mes: original messages encoded in ASCII
        
    Returns:
        array of symbols waited to be transmitted.
        example:
        the lenth of per pass is 4:[12,32,43,12,12,32,32,32]
        [pass1,pass2]
        
    """   
    #spinal code encode
    message_hex = mes.encode().hex()
    encoder = Encoder(k, map_func, mes)
    global spine_length
    # spine length
    spine_length = len(encoder.spine)
    symbols = []
    for i in range(passes):
        for j in range(spine_length):
            symbols.append(encoder.get_symbol(j))
            
    return symbols

def manchesterEncode(symbols):
    """
    Machester Encode

    Args:
        symbols: array of symbols in decimal
        
    Returns:
        array of symbols which are encoded in manchester and in binary.
        example:
        [[1,0,0,1,1,0],[1,0,1,0,0,1]]
    """ 
    #manchester encode and tramsmitter
    reMes=[]
    mes =[]
    global spine_length
    for i in range(passes):
        for j in range(spine_length):
            #convert symbol to c bits binary format.
            symbolofBin = bin(symbols[j+i*spine_length])[2:]
            symbolofBin = symbolofBin.rjust(c,'0')
            for ch in symbolofBin:
                if ch=='1':
                    mes.append(0)
                    mes.append(1)
                elif ch=='0':
                    mes.append(1)
                    mes.append(0)
        reMes.append(mes.copy())
        mes.clear()
    return reMes

def mesFormation(manchesterMes):
    finalMes = []
    for mes in manchesterMes:
        tmp = header+mes
        tmp.extend(tail)
        finalMes.append(tmp)
    return finalMes

led.on()
while True:
    
    #get input message encoded in ASCII(8 bits per char) from terminal
    message = input()
    
    symbols = spinalEncode(message)
            
    print(symbols)
    
    manchesterMes = manchesterEncode(symbols)
    finalMes = mesFormation(manchesterMes)
    
    ledBlink(finalMes)
            
            