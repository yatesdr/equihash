gcc main.c blake2bp.c blake2b.c -o main.exe -m64 -mxop -lmsvcrt -mcx16 -mmmx -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -msse4 -m3dnow -mpopcnt -mtune=native -O2 -Wall
pause