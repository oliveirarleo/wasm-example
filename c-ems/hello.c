#include <stdio.h>
#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE void hello(){
    printf("Hello Woraald\n");
}

int main() {
    hello();
}