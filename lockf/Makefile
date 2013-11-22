all: Init Reserve-v1 Reserve-v2

Init: exercice1.c
	cc $^ -o $@

Reserve-v1: exercice2.c
	cc $^ -o $@

Reserve-v2: exercice3.c
	cc $^ -o $@

.PHONY: mrproper

mrproper:
	@rm -f Init Reserve-v1 Reserve-v2
