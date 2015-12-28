CXX = g++
OBJECTS = main.o code_token.o lexer.o common.o doc_token.o tokenizer.o parser.o regex.o
CXXFLAGS = -std=c++0x

run: build
	./AQL ../dataset/PerLoc.aql ../dataset/perloc/PerLoc.input > ../dataset/perloc/PerLoc.output
	./AQL ../dataset/Revenue.aql ../dataset/revenue/Revenue.input > ../dataset/revenue/Revenue.output
	./AQL ../dataset/Art.aql ../dataset/art/Art.input > ../dataset/art/Art.output

build: clean compile
	$(CXX) -o AQL $(OBJECTS)

compile: $(OBJECTS)

clean:
	rm -f *.o AQL
