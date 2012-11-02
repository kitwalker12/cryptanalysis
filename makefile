CC = g++
CFLAGS = -Wall -g

hw4 : hw4.o crypt.o histo.o solve.o
	${CC} ${CFLAGS} hw4.o crypt.o histo.o solve.o -o hw4

crypt.o: crypt.cpp crypt.h
	${CC} ${CFLAGS} -c crypt.cpp

histo.o: histo.cpp histo.h
	${CC} ${CFLAGS} -c histo.cpp

solve.o: solve.cpp solve.h
	${CC} ${CFLAGS} -c solve.cpp

hw4.o: hw4.cpp crypt.h
	${CC} ${CFLAGS} -c hw4.cpp

clean:
	rm -rf *.o hw4

