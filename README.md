
```sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./benchmark --benchmark-samples 10 --rng-seed 123
```
