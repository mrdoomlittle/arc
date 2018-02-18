sh clean.sh
gcc -c arc.c
gcc -c hash.c
gcc -c lat/lat.c
gcc main.c arc.o lat.o hash.o
