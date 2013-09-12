#!/bin/bash
gcc -g -Wall -o send_msg send_msg.c  -lwsock32 `pkg-config --cflags --libs gtk+-2.0` #-mwindows
