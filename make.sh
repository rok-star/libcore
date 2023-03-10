TEST=$(dirname $0)/test
SRC=$(dirname $0)/src
OUT="/tmp/libcore"
FLAGS="-std=c18 -g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-misleading-indentation -Wwrite-strings"

[ ! -d $OUT ] 				 && mkdir -p $OUT
[ ! -d $OUT/obj ] 			 && mkdir -p $OUT/obj
[ ! -d $OUT/lib ] 			 && mkdir -p $OUT/lib
[ ! -d $OUT/include ] 		 && mkdir -p $OUT/include
[ ! -d $OUT/include/libcore ]      && mkdir -p $OUT/include/libcore
[ ! -d $OUT/include/libcore/core ] && mkdir -p $OUT/include/libcore/core
[ ! -d $OUT/include/libcore/gfx ]  && mkdir -p $OUT/include/libcore/gfx
[ ! -d $OUT/include/libcore/ui ]   && mkdir -p $OUT/include/libcore/ui

make_core() {
    for path in $SRC/core/*.h; do
        cp $(realpath $path) $OUT/include/libcore/core/$(basename $path)
    done

    for path in $SRC/core/*.c; do
        clang -c $FLAGS -I$OUT/include $(realpath $path) -o $OUT/obj/core_$(basename $path .c).o
        if (( $? != 0 )); then
            exit
        fi
    done

    if [[ $OSTYPE == "linux-gnu"* || $OSTYPE == "darwin"* ]]; then
        for path in $SRC/core/POSIX/*.c; do
            clang -c $FLAGS -I$OUT/include $(realpath $path) -o $OUT/obj/core_POSIX_$(basename $path .c).o
            if (( $? != 0 )); then
                exit
            fi
        done
    elif [[ $OSTYPE == "darwin"* ]]; then
        for path in $SRC/core/apple/*.m; do
            clang -c $FLAGS -I$OUT/include $(realpath $path) -o $OUT/obj/core_apple_$(basename $path .m).o
            if (( $? != 0 )); then
                exit
            fi
        done
    else
        echo "$OSTYPE not supported"
        exit
    fi

    ar rc $OUT/lib/libcore-core.a $OUT/obj/core_*.o
}

install() {
	cp -r $OUT/include/libcore /usr/local/include
	cp $OUT/lib/libcore-core.a /usr/local/lib/libcore-core.a
}

test_() {
	clang $FLAGS -fsanitize=address $TEST/main.c /usr/local/lib/libcore.a -o $OUT/test.a
	$OUT/test.a
}

make_core
install

if [[ $@ == *"--test"* ]]; then
    test_
fi

if [[ $@ == *"--example"* ]]; then
    test_
fi