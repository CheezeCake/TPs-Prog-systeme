all: lanceur ordo

lanceur: lanceur.c utils.o
	cc $^ -o $@

ordo: ordo.c utils.o
	cc $^ -o $@

utils.o: utils.c utils.h
	cc -c $<

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf lanceur ordo ordo.dat pid.dat
