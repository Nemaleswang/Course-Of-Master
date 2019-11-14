test : Adder.o CAdder.o
	g++ -o test Adder.o CAdder.o
	rm *.o

Adder.o : Adder.cpp CAdder.h
	g++ -o Adder.o -c Adder.cpp

CAdder.o : CAdder.cpp CAdder.h
	g++ -o CAdder.o -c CAdder.cpp

