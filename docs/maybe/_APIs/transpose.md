**maybe&lt;basic_result&lt;_, T, E&gt;&gt;::transpose() -> basic_result&lt;_, maybe&lt;T&gt;, E&gt;**

```cpp
template <class T>
class maybe {
  maybe<basic_result<_, T, E>>::transpose() const&
    -> basic_result<_, maybe<T>, E> ;
};
```

Transposes a `maybe` of a `result` into a `result` of a `maybe`.

`nothing` will be mapped to `success(nothing)`.
`just(success(_))` and `just(failure(_))` will be mapped to `success(just(_))` and `failure(_)`　(_ is a placeholder).

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  result<maybe<int>, std::string> x = success(just(5));
  maybe<result<int, std::string>> y = just(success(5));
  assert(x == y.transpose());
}
// end example
```