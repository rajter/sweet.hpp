all:
	g++ -Wall --std=c++11 -lsqlite3 -lboost_regex sweetqltest.cpp -o qltest \
	--std=c++11 -march=native -ggdb -I.. -lwtdbo -lwtdbosqlite3 -lmysqlclient \
	-DMAIN -I../sweetqlimpl
	./qltest
