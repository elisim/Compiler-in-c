all: lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c
	gcc lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c -o scanner
	
lex.yy.c:
	flex rules.lex

#tell make that "clean" is not a file name!
.PHONY: clean run debug

#Clean the build directory	
clean: 
	rm -vf scanner lex.yy.c scanner.txt parse.txt
run:
	./scanner ass2_test.txt
debug: 
	./scanner ass2_test.txt -d 