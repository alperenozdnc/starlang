# These are comments,
# they should be ignored.

## These are multiple line comments,
and should be ignored too. ##

@import namespace>module_two.st;
@import starlang>io.st;
@import namespace>module.st;

type num = int | float;

decl sum(num x, num y): num {
    ret x + y;
}

decl func(): nothing {
    float pi = 3.14159265358979323;

    printlnf("pi is approx. {pi}");

    fn();

    for (mut int x = 0; x < 1000; x++) {
        printlnf("Iteration {x}");
    }

    if (nothing == nothing) {
        println("nothing is nothing!");
    } else if (nothing != nothing) {
        println("nothing isn't nothing!");
    }
}

func();
