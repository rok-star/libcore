TEST=$(dirname $0)/test
SRC=$(dirname $0)/src
OUT="/tmp/libcore"
FLAGS="-std=c18 -g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-misleading-indentation -Wwrite-strings"

[ ! -d $OUT ] 				 && mkdir -p $OUT
[ ! -d $OUT/obj ] 			 && mkdir -p $OUT/obj
[ ! -d $OUT/lib ] 			 && mkdir -p $OUT/lib
[ ! -d $OUT/include ] 		 && mkdir -p $OUT/include
[ ! -d $OUT/include/libcore ] && mkdir -p $OUT/include/libcore

make() {
    for path in $SRC/*.h; do
        cp $(realpath $path) $OUT/include/libcore/$(basename $path)
    done

    for path in $SRC/*.c; do
        clang -c $FLAGS -I$OUT/include $(realpath $path) -o $OUT/obj/$(basename $path .c).o
        if (( $? != 0 )); then
            exit
        fi
    done

    if [[ $OSTYPE == "linux-gnu" ]]; then
        for path in $SRC/POSIX/*.c; do
            clang -c $FLAGS -I$OUT/include $(realpath $path) -o $OUT/obj/POSIX_$(basename $path .c).o
            if (( $? != 0 )); then
                exit
            fi
        done
    elif [[ $OSTYPE == "darwin" ]]; then
        echo "..."
    else
        echo "$OSTYPE not supported"
        exit
    fi

    ar rc $OUT/lib/libcore.a $OUT/obj/*.o
}

install() {
	cp -r $OUT/include/libcore /usr/local/include
	cp $OUT/lib/libcore.a /usr/local/lib/libcore.a
}

test_() {
	clang $FLAGS -fsanitize=address $TEST/main.c /usr/local/lib/libcore.a -o $OUT/test.a
	$OUT/test.a
}

make
install

if [[ $@ == *"--test"* ]]; then
    test_
fi

if [[ $@ == *"--example"* ]]; then
    test_
fi