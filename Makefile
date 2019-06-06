all:
	g++ -g -O0 -m64 -o bin/test src/*.cpp src/test/Test.cpp -Isrc -I"../lib/boost_1_60_0/" -L"../lib/boost_1_60_0/stage/lib" -lboost_filesystem -lboost_system -lboost_container -lboost_regex -std=c++14

clean:
	rm bin/test