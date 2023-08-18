make all:
	g++ -o execu controller.cpp client.cpp clientburst.cpp clientrecv.cpp clientbroadcast.cpp driver.cpp -pthread
	./execu