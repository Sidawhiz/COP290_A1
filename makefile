yourcode.out: main.o
	g++ main.o -o yourcode.out

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o yourcode.out	
