#include <stdio.h>

int foo(int a) {
	return a + 7;
}

int bar(int a, int b) {
	return foo(a) + b;
}

int main() {
	printf("Hello World: %d\n", bar(4, 7));

	return 0;
}
