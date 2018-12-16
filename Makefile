FLAGS = -std=c++11
COMPILER = g++
IN1 = Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris vitae elit sit amet dolor mollis congue at dapibus ipsum.
IN2 = Quisque ac scelerisque purus. Curabitur posuere eros quis bibendum lobortis. Vivamus sollicitudin sapien quis diam scelerisque congue ut a est.
IN3 = Sed sit amet risus at tellus posuere luctus. Mauris ac ante.

all: input_generator.exe kshingles.exe minhash_v1.exe minhash_v2.exe lsh_v1.exe lsh_v2.exe lsh_v3.exe lsh_v4.exe

input_generator.exe:
	$(COMPILER) $(FLAGS) input_generator.cpp -o input_generator.exe
	mkdir kshingles_input
	echo $(IN1) > kshingles_input/1.in
	echo $(IN2) >> kshingles_input/1.in
	echo $(IN3) >> kshingles_input/1.in

kshingles.exe:
	$(COMPILER) kshingles.cpp -o kshingles.exe

minhash_v1.exe:
	$(COMPILER) minhash_v1.cpp -o minhash_v1.exe

minhash_v2.exe:
	$(COMPILER) minhash_v2.cpp -o minhash_v2.exe

lsh_v1.exe:
	$(COMPILER) lsh_v1.cpp -o lsh_v1.exe

lsh_v2.exe:
	$(COMPILER) lsh_v2.cpp -o lsh_v2.exe

lsh_v3.exe:
	$(COMPILER) lsh_v3.cpp -o lsh_v3.exe

lsh_v4.exe:
	$(COMPILER) lsh_v4.cpp -o lsh_v4.exe

clean:
	rm -f *.exe
	rm -rf kshingles_input