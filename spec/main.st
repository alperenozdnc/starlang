# These are comments,
# they should be ignored.

## These are multiple line comments,
and should be ignored too. ##

@import starlang>io.st;
@import namespace>module.st;

decl func(): nothing {
    fn();

    for (mut int x = 0; x < 1000; x++) {
        printlnf("Iteration {x}");
    }

    if (nothing == nothing) {
        println("nothing is nothing!");
    }
}

func();
