all: exercice1 timeout foreach

exercice1: exercice1.c
	cc $^ -o $@

timeout: exercice2.c
	cc $^ -o $@

foreach: exercice3.c
	cc $^ -o $@

exercice4: exercice4.c
	cc $^ -o $@

.PHONY: mrproper clean

clean: mrproper

mrproper:
	@rm -f exercice1 timeout foreach exercice4
