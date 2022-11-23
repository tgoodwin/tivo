# tivo
research project exploring record and replay at the LLVM level

## Building docker image
```
docker build -t tivo .
```

## Build Tivo LLVM Pass

```sh
cd build
cmake ..
make # generates `./build/tivo-pass/libTivoPass.so`

# compile with the pass
clang-14 -O0 -flegacy-pass-manager -Xclang -load -Xclang ./build/tivo-pass/libTivoPass.so ./foo.c
```
