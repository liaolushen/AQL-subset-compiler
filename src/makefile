CXX = g++
OBJECTS = main.o code_token.o lexer.o common.o
CXXFLAGS = -std=c++0x

run: build
	./AQL ../dataset/Revenue.aql ../dataset/revenue/Revenue.input

build: clean compile
	$(CXX) -o AQL $(OBJECTS)

compile: $(OBJECTS)

clean:
	rm -f *.o AQL
