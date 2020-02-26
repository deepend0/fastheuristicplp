all: lib test

test:
	g++ -g -O0 -m64 -o bin/test src/fhplp/*.cpp src/fhplp/test/Test.cpp -Isrc -I"../lib/boost_1_60_0/" -L"../lib/boost_1_60_0/stage/lib" -lboost_filesystem -lboost_system -lboost_container -lboost_regex -std=c++14
	
lib:
	mkdir lib
	g++ -g -O0 -m64 -shared -o lib/libfastheuristicplp.so src/fhplp/*.cpp -Isrc -I"../lib/boost_1_60_0/" -L"../lib/boost_1_60_0/stage/lib" -lboost_filesystem -lboost_system -lboost_container -lboost_regex -std=c++14 -fPIC

clean:
	rm bin/test
	rm -rf lib