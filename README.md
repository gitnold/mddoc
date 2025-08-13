**Why not have *"jupyter notebooks"* for terminal C programs.**
- Welcome to **mddoc** all your beautiful markdwon notes , documentation and C code in oe place. Write one file use it everywhere.
- Support is limitedto POSIX compliant systems only.
### Installation.
#### 1.Install from source.
- Clone this Github repo.
```
$ git clone https://github.com/gitnold/mddoc.git
```
- Change to the directory and make the build script executable.
```
cd mddoc
chmod +x build.sh
```
- Run `mddoc`.
```
./mddoc <arguments>
```

### Usage.
- The program takes the following arguments or run `mddoc -h` to see available options.
- It takes one mandatory argument which is the markdown file with the code to be exctracted. 
- **NOTE:** *The code should be in sequential order in the way its supposed to appear in the source file. Any compiler errors generated will have to be handled manually.*
- ` ./mddoc <markdown file> [ filename.c ] [ binaryfile name ]`
- **sample usage : ** `./mddoc test1.md  main.c main`.
- Above command reads code blocks from *test1.md*, creates a new *main.c* file and compiles the C code into a binary file named *main*.
- Arguments in square brackets are optional. If no binary file name is provided its name is extracted from provided C filename.
