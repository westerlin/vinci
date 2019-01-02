#http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
CFLAGS=-I.
DEPS = utility.h logica.h dramasim.h lcaparser.h
OBJ = main.o utility.o logica.o dramasim.o lcaparser.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ -std=c++11 $< $(CFLAGS)

drama: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

old: 
	g++ -o drama -std=c++11 *.cpp 

.PHONY: clean

clean:
	rm -f drama