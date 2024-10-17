# Makefile for RISC-V In a Nutshell course (RVVIAN) examples

# RISC-V C Compiler
# available options (the version used must support RVV intrinsics)
# clang/llvm
# RISCVCC=clang  --target=riscv64
# GNU Compiler Collection (GCC)
RISCVCC=riscv64-unknown-elf-g++ 

# architectural parameters for the simulation
# width of vector registers (VLEN)
VLEN?=128

# path to proxy-kernel (pk)
PK_PATH=/home/mips/test-pratik/riscv-built-tools/riscv64-unknown-elf/bin/pk64 

# SIMULATOR
# Available options in the Docker (uncomment one)

#Original spike command
#SIMULATOR=spike --isa=rv64gcv_zicntr_zihpm --varch=vlen:$(VLEN),elen:64 $(PK_PATH)

#varch option doesnt work, so removing it from spike 
#SIMULATOR=spike --isa=rv64gcv_zicntr_zihpm $(PK_PATH)
# Original qemu command
SIMULATOR=qemu-riscv64 -cpu rv64,v=on,vext_spec=v1.0,vlen=1024,rvv_ta_all_1s=on

#New qemu command to enable sys register reads for cycles and instructions
#SIMULATOR=qemu-system-riscv64 -cpu rv64gcv -machine virt -nographic -bios none -kernel 

vector_add_intrinsics.o: vector_add_intrinsics.cpp
	$(RISCVCC) -O2 -march=rv64gcv -mabi=lp64d -c -o $@ $^

vector_add_ref.o: vector_add_ref.cpp
	$(RISCVCC) -O2 -march=rv64gcv -mabi=lp64d -c -o $@ $^
	
bench_vector_add: bench_vector_add.cpp vector_add_intrinsics.o vector_add_ref.o
	 $(RISCVCC) -O2 -march=rv64gcv -mabi=lp64d $^ -o $@

sim_bench_vector_add: bench_vector_add
	$(SIMULATOR) $^

clean:
	rm -f *.o bench_vector_add 

.PHONY: sim_bench_vector_add clean
