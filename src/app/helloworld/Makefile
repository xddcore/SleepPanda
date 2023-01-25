all:
	g++ -std=c++17 helloworld.cpp -o helloworld
	g++ -std=c++17 helloworld_test.cpp -lcppunit -o helloworld_test
	chmod +x helloworld
	./helloworld_test

clean:
	$(RM) helloworld helloworld_test
