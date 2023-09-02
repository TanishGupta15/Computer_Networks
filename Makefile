all:
	g++ -Wall controller.cpp client.cpp clientburst.cpp clientrecv.cpp clientbroadcast.cpp driver.cpp -pthread -o client
	./client

clean:
	rm -rf client
	rm *.txt

run:
	./client

debug:
	g++ -g -Wall controller.cpp client.cpp clientburst.cpp clientrecv.cpp clientbroadcast.cpp driver.cpp -pthread -o client
	gdb client