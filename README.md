Implementation of a basic Feistel network pseudorandom permutation

The following commands can be run to generate the seed and key files respectively.

xxd -l 16 -p /dev/random > seed.txt
xxd -l 16 -p /dev/urandom > keyschedule.txt

The first command gets a string of 16 truly random bytes from the hardware, whereas the second command 
gets some psuedorandom bytes for the program to use to decide whether to return left or right 
in the expansion portion. Should 512 bit numbers be used, swap 16 for 64 in the command.