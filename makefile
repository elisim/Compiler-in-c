all: lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c
	gcc lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c -o scanner
	
lex.yy.c:
	flex rules.lex

#tell make that "clean" is not a file name!
.PHONY: clean run

#Clean the build directory	
clean: 
	rm -vf scanner lex.yy.c out.txt
run:
	./scanner ass1_test.txt
