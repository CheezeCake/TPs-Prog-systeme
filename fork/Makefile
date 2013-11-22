all: exercice1 timeout foreach

exercice1: exercice1.c
	cc $^ -o $@

timeout: exercice2.c
	cc $^ -o $@

foreach: exercice3.c
	cc $^ -o $@

.PHONY: mrproper

mrproper:
	@rm -f exercice1 timeout foreach
