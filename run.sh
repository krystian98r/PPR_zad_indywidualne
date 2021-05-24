 #!/bin/bash

gcc `xmlrpc-c-config  abyss-server --cflags`    -O2 -Wall  -c proc2.c
gcc `xmlrpc-c-config abyss-server --libs`       -s proc2.c -o proc2

host="127.0.0.1";
port=1234;

sudo fuser -k ${port}/tcp
konsole -e php -S ${host}:${port} proc4.php &
konsole -e dotnet run &
konsole -e ./proc2 &
python3 proc1.py

