from scapy.all import*
packet = fragment(IP(dst = "8.8.0.1", src = "127.0.0.1",fragsize= 8)) 
# packet = IP(dst = "8.8.0.1", src = "127.0.0.1",) 
with open('__test/input/TCP_8801.bin', 'wb') as f:
    f.write(bytes(packet))

# with open('__test/input/TCP_8801.bin', 'rb')as f:
#     packet = IP(f.read())
# packet.show() 