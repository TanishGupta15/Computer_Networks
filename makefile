make all:
	g++ -o execu controller.cpp client.cpp clientburst.cpp  -pthread
	./execu