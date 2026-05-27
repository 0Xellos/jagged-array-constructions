
Inspired by [a CF blog](https://codeforces.com/blog/entry/153978).

```sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./benchmark --benchmark-samples 10 --rng-seed 123
```

In case of segfault, increase stack size e.g. with `ulimit -s 256000`
since `two_step_construction::constructOriginal()` puts more data on stack.
