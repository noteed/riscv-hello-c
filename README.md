# Bare metal RISC-V hello world in C

This is a copy of the [riscv-probe](https://github.com) repository stripped
from everything but the 64-bit hello world program for the `sifive_u` QEMU
machine.

The directory structure is kept. This makes this repository a good starting
point to dive into riscv-probe. In particular we keep the `crt.s` and `start.c`
files although they are almost empty. They are used in the original repository
to initialize libfemto. Here libfemto just provides a `puts` function.

Having such a stripped down hello world program written in C was necessary for
me to learn how to create a [bare metal RISC-V assembly hello
world](https://github.com) version because I couldn't find a complete example
program in assembly only that was writing to the UART.


## Building

Assuming the default prefix for the GNU toolchain is correct, and the toolchain
is in the `$PATH`, running `make` produce our `hello` program. Refer to the
original repository otherwise.

```
$ make
AS.rv64imac build/obj/rv64imac/env/qemu-sifive_u/crt.o
CC.rv64imac build/obj/rv64imac/examples/hello/hello.o
CC.rv64imac build/obj/rv64imac/libfemto/arch/riscv/start.o
CC.rv64imac build/obj/rv64imac/libfemto/std/putchar.o
AR.rv64imac build/lib/rv64imac/libfemto.a
LD.rv64imac build/bin/rv64imac/qemu-sifive_u/hello
```

The result is a 64-bit RISC-V binary.

```
$ file build/bin/rv64imac/qemu-sifive_u/hello
build/bin/rv64imac/qemu-sifive_u/hello: ELF 64-bit LSB executable, UCB RISC-V,
version 1 (SYSV), statically linked, not stripped
```


## Running

Run it with:

```
$ qemu-system-riscv64 -nographic -machine sifive_u \
  -kernel build/bin/rv64imac/qemu-sifive_u/hello 
Hello.
QEMU: Terminated
```

Note: the program enters an infinite loop after producing the `Hello.` text.
Type `ctrl-a x` to stop QEMU.


## Assembly

To dissamble the program:

```
$ riscv64-unknown-elf-objdump -d build/bin/rv64imac/qemu-sifive_u/hello
build/bin/rv64imac/qemu-sifive_u/hello:     file format elf64-littleriscv

Disassembly of section .text:

0000000080000000 <_start>:
    80000000:	0040006f          j	80000004 <main>

0000000080000004 <main>:
    80000004:	1141              addi	sp,sp,-16
    80000006:	e022              sd	s0,0(sp)
    80000008:	e406              sd	ra,8(sp)
    8000000a:	00000417          auipc	s0,0x0
    8000000e:	02e40413          addi	s0,s0,46 # 80000038 <putchar+0x16>
    80000012:	00044503          lbu	a0,0(s0)
    80000016:	e111              bnez	a0,8000001a <main+0x16>
    80000018:	a001              j	80000018 <main+0x14>
    8000001a:	0405              addi	s0,s0,1
    8000001c:	006000ef          jal	ra,80000022 <putchar>
    80000020:	bfcd              j	80000012 <main+0xe>

0000000080000022 <putchar>:
    80000022:	100137b7          lui	a5,0x10013
    80000026:	4398              lw	a4,0(a5)
    80000028:	02071693          slli	a3,a4,0x20
    8000002c:	fe06cde3          bltz	a3,80000026 <putchar+0x4>
    80000030:	0ff57513          andi	a0,a0,255
    80000034:	c388              sw	a0,0(a5)
    80000036:	8082              ret
```

I used the above listing to guide modifications to the `hello.S` program
provided in rv8 (I think) to finally create a pure assembly hello world.
