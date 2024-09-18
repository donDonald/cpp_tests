# parralel_parsing
An example of parsing huge amount of files.\
Each file contains a set of uint16_t values separated by comma.




### Prerequisites
 * Ubuntu 20.04, older versions shall work as well
 * cmake 3.10 or newver
 * build-essential package
 * boost dev libraries




### Building
```
$ git clone https://github.com/donDonald/cpp_tests
$ cd cpp_tests/parralel_parsing
$ mkdir build && cd build
$ cmake ..
$ make
```




### Testing




#### 1st - create a bunch of files to be parsed
Create destination folder.
```
mkdir output
```

Generate, let say, 1000 files with 1 to 1000000 records per file.
```
./generator --dst-folder=output --num-files=1000 --file-prefix=data --min-size=1 --max-size=1000000 --index-file=data.index
```
The generation results shall be stored into output folder, data.index shall contain <value>:<count> pairs for every generated value.




#### 2-nd parse the generated data
```
./parser --src-folder=output --buffer-size=4096 --workers-count=4 --result-file=result-4096-4.index
```



#### 3-rd compare index files produced by generator and parser

```
diff data.index result-4096-4.index
```
diff shall report nothing since these 2 files shall be identical.
