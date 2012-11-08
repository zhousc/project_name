#/bin/bash
gcc -fPIC -c -Wall open_libs.c -o open_libs.o
gcc -fPIC -c -Wall open_files.c -o open_files.o
gcc -shared -o libopen_libs.so open_libs.o open_files.o
rm open_libs.o
rm open_files.o
#strip libopen_libs.a
cp libopen_libs.so /usr/lib/
