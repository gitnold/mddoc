buildtype=$1

compileSourceFile() {
    if [ $1 = "debug" ];
    then
        echo "[ -INFO- ] Compiling debug build...."
        gcc -Wall -Wextra notes.c -o notes.out -g
    else
        echo "[ -INFO- ] Compiling source file...."
        gcc -Wall -Wextra  notes.c -o notes
    fi

    if [ $? -eq 0 ];
    then
        echo "Program compiled successfully ^_^"
    fi
}

#calling our compilation function 
compileSourceFile $buildtype


