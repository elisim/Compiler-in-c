all: lex.yy.c Actions/actions.c Token/Token.c
	gcc lex.yy.c Actions/actions.c Token/Token.c -o scanner
	
lex.yy.c:
	flex rules.lex

#tell make that "clean" is not a file name!
.PHONY: clean run

#Clean the build directory	
clean: 
	rm -vf scanner lex.yy.c
run:
	./scanner
