all: lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c
	gcc lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c -o my_parser

lex.yy.c:
	flex rules.lex
#tell make that "clean" is not a file name!
.PHONY: clean 

#Clean the build directory	
clean: 
	rm -vf my_parser lex.yy.c out/*
