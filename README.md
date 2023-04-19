# avl_cpp

When I was coursing Computer Science the only algorithm that I couldn't finish during my studies at the time was the AVL tree remotion. 5 years later I decided to give it another spin and it finally came out.

I tried to apply DRY extensively and played a lot with templates and `<type_traits>` to achieve this, my goal was to practice more intermediate C++ functionalities while implementing the AVL.

The bulk of the implementation lies in the `avl::Node` class located at `src/avl/Node.h`, the `avl::Tree` being merely a wrapper for ergonomics during usage.

## Building

```bash
# At project root
mkdir build && cd build
cmake ..
make
```

## Running
There is a pretty simple executable called `avl_cpp` that creates a hardcoded tree with a few insertions and displays it. The fun part lies in the `avl_cpp_test` that I wrote before the implementation itself with Test-Driven Development in mind and a few stress tests.
