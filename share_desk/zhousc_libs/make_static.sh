#/bin/bash
gcc -c open_libs.c -o open_libs.o
gcc -c open_files.c -o open_files.o
ar rcs libopen_libs.a open_libs.o open_files.o
rm open_libs.o open_files.o
#strip libopen_libs.a
cp libopen_libs.a /usr/lib/
cp open_source.h /usr/include

