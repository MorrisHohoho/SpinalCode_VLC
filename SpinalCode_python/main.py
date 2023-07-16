# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
'''
This is an example of the python implementation of spinal codes. A simple
    "hello world" message is encoded, white gaussian noise with some SNR is
    added, and the noisy data is then fed into the decoder.

This example produces constellation points with 8-bit precision. The noisy
    symbols are quantized to integers, and all path metrics are done with
    integer arithmetic.
'''

# some constants:
k = 3
c = 6
precision = 6
B = 4
d = 3

# Message to be encoded:
message = "SCU!"

# expected encoder output
# expected_encoder_output = [184, 108, 36, 108, 253, 68, 204, 119, 243, 141, 170, 56, 101, 97, 252, 79, 95, 236, 207, 191,
#                            158, 89, 82, 151, 141, 255, 100, 112, 233, 220, 20, 146, 16, 108, 24, 117, 178, 175, 39, 210,
#                            134, 224, 220, 75, 231, 4, 182, 189, 29, 59, 129, 105, 60, 64, 207, 253, 161, 41, 146, 10,
#                            249, 210, 175, 121, 121, 37, 46, 239, 208, 18, 42, 101, 61, 67, 136, 166, 62, 192, 44, 43,
#                            240, 97, 196, 228, 91, 94, 242, 9, 130, 218, 243, 208, 16, 248, 57, 194]

if __name__ == '__main__':
    from Encoder import Encoder
    from Decoder import Decoder
    from SymbolMapper import SymbolMapper
    import random
    import math

    mapper = SymbolMapper(c, precision)
    map_func = lambda value: mapper.map(value)

    # Instantiate an encoder
    print
    'Message string: "%s"' % message
    print
    'Message hex:', message.encode().hex()
    encoder = Encoder(k, map_func, message)

    # spine length
    n = 8 * len(message)
    spine_length = (n + (k - 1)) / k
    spine_length=int(spine_length)

    # encode a short message, making 3 passes
    print
    "Producing 3 passes."
    symbols=[]
    for i in range(3):
        for i in range(spine_length):
            symbols.append(encoder.get_symbol(i))
    print("symbols: ", symbols)

    # make sure we got the expected result
    # assert (expected_encoder_output == symbols)

    # get average signal power
    signal_power = mapper.get_signal_average_power()
    #
    # # what is the noise power and standard deviation when SNR is 10dB?
    noise_power = signal_power / 10.0
    noise_std_dev = math.sqrt(noise_power)
    #
    # # initialize random number generator. this seed is fixed constant in order
    # # to get deterministic results in this example.
    random.seed(314159265359)
    #
    # # add white gaussian noise at 10dB to signal
    # print
    # "Adding white gaussian noise at 10dB."

    print("len:",len(symbols))
    for i in range(500):
        noisy_symbols = [sym + random.gauss(0, noise_std_dev) for sym in symbols]
        # # round to closest integer
        noisy_symbols = [int(x + 0.5) for x in noisy_symbols]

        with open('./k3.txt','a') as f:
            for sym in noisy_symbols:
                if(sym<0):
                    sym&=0xff
                f.write(str(sym)+' ')
            f.write('\n')
        print("noisy symbols:", noisy_symbols)
    # # instantiate decoder
    decoder = Decoder(k, B, d, map_func)
    # # update decoder with gathered points
    # noisy_symbols=[28, 65, 29, 42, 52, 18, 8, 41, 33, 35, 21, 58, -4, 45, -5, 50, 47, 6, 2, 40, 55, 62, 46, 34, 34, 19, 24, 42, 35, 24, 10, 14, 48, 39, 46, 55, 42, 52, 69, 10, 26, 14, 30, 45, 48, 62, 54, 27, 39, 55, 4, 52, 53, 32, 59, -1, 14, 16, 47, 17, 69, 61, 29, 44, 7, 40, 51, 60, 49, 6, 25, 65, 46, 45, 3, 16, 49, 3, 1, 26, 28, 11, 35, 53, 23, 12, 60, 33, 25, 53, 23, 39, 46, 40, 8, 72]
    for i in range(spine_length):
        decoder.advance([symbols[i],
                         symbols[i + spine_length],
                         symbols[i + 2 * spine_length]])

    print(decoder.get_most_likely())
    print(decoder.get_most_likely().encode().hex())


    # make sure we got the message we started with
    # assert (decoder.get_most_likely() == message)