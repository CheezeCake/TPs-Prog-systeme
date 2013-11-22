all: tubes_nommes tubes_anonymes base

%: %.c
	cc $^ -o $@

.PHONY: mrproper

mrproper:
	@rm -f tubes_nommes
	@rm -f tubes_anonymes
	@rm -f base
