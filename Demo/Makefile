CC=clang
CFLAGS=-O -Wall -I/usr/include/bpf

DEV=ens18

XDP_PROG=xdp_demo.o

all: $(XDP_PROG)

$(XDP_PROG): xdp_demo.c
	$(CC) -target bpf $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(XDP_PROG)

dump: $(XDP_PROG)
	llvm-objdump -S $(XDP_PROG)

load: $(XDP_PROG)
	sudo ip --force link set dev $(DEV) xdpgeneric obj $(XDP_PROG) sec xdp/demo

unload:
	sudo ip link set dev $(DEV) xdpgeneric off

show:
	sudo ip link show dev $(DEV)
