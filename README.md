# Neco

Call-With-Current-Continuation, aka call/cc, is an advanced control flow operator in Scheme and several other languages.

Neco emulates call/cc in C++14 using macros, may serves as a building block for asynchronos program.

It's not fully emulated, e.g. continuation cannot return a value.

See demo.cc for details.

Neco is a toy project just for fun. In no way it can be used in production. C++20 Coroutine or Boost Fiber are better choices.
