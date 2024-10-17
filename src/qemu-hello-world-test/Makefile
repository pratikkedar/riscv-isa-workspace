# Makefile for RISC-V In a Nutshell course (RVVIAN) examples

# RISC-V C Compiler
# available options (the version used must support RVV intrinsics)
# clang/llvm
# RISCVCC=clang  --target=riscv64
# GNU Compiler Collection (GCC)
RISCVCC=riscv64-unknown-elf-gcc 

# architectural parameters for the simulation
# width of vector registers (VLEN)
VLEN?=128

# Original qemu command
SIMULATOR=qemu-riscv64 -cpu rv64,v=on,vext_spec=v1.0,vlen=1024,rvv_ta_all_1s=on

bench_vector_add: bench_vector_add.c
	 $(RISCVCC) -O2 -march=rv64gcv -mabi=lp64d $^ -o $@

sim_bench_vector_add: bench_vector_add
	$(SIMULATOR) $^

clean:
	rm -f *.o bench_vector_add 

.PHONY: sim_bench_vector_add clean
