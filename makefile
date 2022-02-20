yourcode.out: ublas.o umkl.o main.o
	g++ umkl.o ublas.o main.o -o yourcode.out -L /opt/intel/mkl/lib -L /home/sidawhiz/Desktop/COP290/libr/lib -lopenblas -lpthread -lgfortran -lmkl_intel_ilp64 -lmkl_intel_thread -lmkl_core -liomp5 -lm -ldl

main.o: main.cpp
	g++ -c main.cpp -I /opt/intel/mkl/include -I /home/sidawhiz/Desktop/COP290/libr/include 

ublas.o: ublas.cpp
	g++ -c ublas.cpp -I /home/sidawhiz/Desktop/COP290/libr/include -L /home/sidawhiz/Desktop/COP290/libr/lib -lopenblas -lpthread -lgfortran

umkl.o: umkl.cpp
	g++ -c umkl.cpp -I /opt/intel/mkl/include -L /opt/intel/mkl/lib -lmkl_intel_ilp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl

clean:
	rm *.o yourcode.out	
