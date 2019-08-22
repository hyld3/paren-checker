

default: clang

clang:
	clang -Ofast -o stack stack.c

clean:
	$(RM) -f stack
