import xmlrpc.client

s = xmlrpc.client.ServerProxy('http://localhost:12345')
print("*** PROCES 1 ***")

while 1:
    msg = input('Enter message: \n> ')
    print(s.getMess(msg))