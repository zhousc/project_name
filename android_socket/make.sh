#!/bin/bash
gcc -o client client.c -Wall -lpthread
gcc -o server server.c -Wall -lpthread
chmod u+x client

