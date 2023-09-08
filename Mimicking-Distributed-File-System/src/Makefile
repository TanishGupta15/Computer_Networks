all:
	mkdir -p ../bin
	mkdir -p ../logs
	g++ -O3 ./controller.cpp ./client.cpp ./clientburst.cpp ./clientrecv.cpp ./clientbroadcast.cpp ./driver.cpp -pthread -o ../bin/client
	./bin/client

compile:
	mkdir -p ../bin
	mkdir -p ../logs
	g++ -O3 ./controller.cpp ./client.cpp ./clientburst.cpp ./clientrecv.cpp ./clientbroadcast.cpp ./driver.cpp -pthread -o ../bin/client

clean:
	rm -rf ../bin/
	rm -rf ../logs/

run:
	../bin/client

plot:
	mkdir -p ../plots
	python3 plot.py ../logs/latency.csv

debug:
	mkdir -p ../bin
	mkdir -p ../logs
	g++ -g -Wall ./controller.cpp ./client.cpp ./clientburst.cpp ./clientrecv.cpp ./clientbroadcast.cpp ./driver.cpp -pthread -o ../bin/client
	gdb ../bin/client