dbj_pack_objs = debug_beej_pack.o beej_pack.o
btc_ver_objs = btc_version.o beej_pack.o

debug_beej_pack.out: $(dbj_pack_objs)
	gcc -o debug_beej_pack.out $(dbj_pack_objs)
debug_beej_pack.o: debug_beej_pack.c beej_pack.h
	gcc -c debug_beej_pack.c
beej_pack.o: beej_pack.c beej_pack.h
	gcc -c beej_pack.c

btc_version.out: $(btc_ver_objs)
	cc -o btc_version.out $(btc_ver_objs)
$(btc_ver_objs): beej_pack.h


clean:
	rm debug_beej_pack.out $(dbj_pack_objs) $(btc_ver_objs)