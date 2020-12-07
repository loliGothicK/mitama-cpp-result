# Implementations

## is_just

!!! summary "maybe&lt;T&gt; --> bool"

    Returns `true` if the maybe has some value.

!!! info "declaration"

    ```cpp
    template <class T>
    class maybe {
        constexpr bool maybe<T>::is_just() const noexcept ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe<int> x = just(2);
  assert( x.is_just() );

  maybe<int> y = nothing;
  assert( not y.is_just() );
}
// end example
```

## is_nothing

!!! summary "maybe&lt;T&gt; --> bool"

    Returns `true` if the `maybe` is a `nothing` value.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        constexpr bool maybe<T>::is_nothing() const noexcept ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe<int> x = just(2);
  assert( ! x.is_nothing() );

  maybe<int> y = nothing;
  assert( y.is_nothing() );
}
// end example
```

## as_ref

!!! summary "maybe&lt;T&gt;& --> maybe&lt;T&&gt;"
    Converts from `maybe<T>&` to `maybe<T&>`.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        auto maybe<T>::as_ref() & -> maybe<T&> ;

        auto maybe<T>::as_ref() const& -> maybe<const T&> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
#include <iostream>
using namespace mitama;
using namespace std::string_literals;

int main() {
    maybe text = just("Hello, world!"s);
    // First, cast `maybe<T>` to `maybe<T&>` with `as_ref`,
    auto text_length = text.as_ref().map(&std::string::size);
    std::cout << "still can print text: " << text << "\n";
}
// end example
```

## expect

!!! summary "maybe&lt;T&gt; --> string_view --> T"

    Unwraps a maybe, yielding the content of a `just`.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        auto maybe<T>::expect(std::string_view msg) ${CVREF} -> T ${CVREF} ;
    };
    ```


!!! warning "Exceptions"
    Raise `mitama::runtime_panic` if the value is a `nothing` with a custom panic message provided by `msg`.

### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  {
    maybe x = just("value"s);
    assert( x.expect("the world is ending") == "value"s );
  }
  try {
    maybe<int> x = nothing;
    x.expect("the world is ending"); // panics with `the world is ending`
  }
  catch ( mitama::runtime_panic const& panic ) {
    std::cerr << panic.what() << std::endl; // runtime panicked at 'the world is ending'
  }
}
// end example
```

## unwrap

!!! summary "maybe&lt;T&gt; --> T"

    Unwraps a maybe, yielding the content of a `just`.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        auto maybe<T>::unwrap() ${CVREF} -> value_type ${CVREF} ;
    };
    ```

!!! warning "Exceptions"

    Raise `mitama::runtime_panic` if a maybe has not `just` value.


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  {
    maybe x = just("air"s);
    assert(x.unwrap() == "air"s);
  }
  try {
    maybe<int> x = nothing;
    x.unwrap(); // raise an exception
  }
  catch ( mitama::runtime_panic const& panic ) {
    std::cerr << panic.what() << std::endl; // runtime panicked at 'called `maybe::unwrap()` on a `nothing` value'
  }
}
// end example
```

## unwrap_or

!!! summary "maybe&lt;T&gt; --> T --> T"

    Returns the contained value or a default.

    Arguments passed to `unwrap_or` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `unwrap_or_else`, which is lazily evaluated.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class U>
        std::enable_if_t<
            meta::has_type<std::common_type<value_type ${CVREF}, U&&>>::value,
        std::common_type_t<value_type ${CVREF}, U&&>>
        maybe<T>::unwrap_or(U&& def) ${CVREF} ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  assert(maybe{just("car"s)}.unwrap_or("bike"s) == "car"s);
  assert(maybe<std::string>{nothing}.unwrap_or("bike"s) == "bike"s);
}
// end example
```


## unwrap_or_else

!!! summary "maybe&lt;T&gt; --> F --> T"

    Returns the contained value or computes it from a invocable object `op: F`.

!!! note "constraints"

    ```cpp
    requires (F&& f) {
        { f() } -> std::convertible_to<T>;
    }
    ```

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F>
        std::enable_if_t<
            std::conjunction_v<
            std::is_invocable<F&&>,
            meta::has_type<std::common_type<value_type ${CVREF}, std::invoke_result_t<F&&>>>>,
        std::common_type_t<value_type ${CVREF}, std::invoke_result_t<F&&>>>
        maybe<T>::unwrap_or_else(F&& f) ${CVREF} ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  int k = 10;
  assert(maybe{just(4)}.unwrap_or_else([k]{ return 2 * k; }) == 4);
  assert(maybe<int>{}.unwrap_or_else([k]{ return 2 * k; }) == 20);
}
// end example
```

## map

!!! summary "maybe&lt;T&gt; --> F --> Args... --> maybe&lt;U&gt;"

    Maps a `maybe<T>` to `maybe<U>` by applying a function to a contained value.


!!! note "constraints"

    ```cpp
    requires {
        maybe<std::invoke_result_t<F&&, T, Args&&...>>;
    }
    ```


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F, class... Args,
            std::enable_if_t<
                std::is_invocable_v<F&&, T ${CVREF}, Args&&...>, bool> = false>
        auto maybe<T>::map(F&& f, Args&&...) ${CVREF} -> maybe<std::invoke_result_t<F&&, T ${CVREF}, Args&&...>> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe maybe_some_string = just("Hello, World!"s);
  // `maybe::map` takes self *by ref*,
  // *not* consuming `maybe_some_string`
  maybe maybe_some_len = maybe_some_string.map(&std::string::size);

  assert(maybe_some_len == just(13u));
}
// end example
```

## map_or

!!! summary "maybe&lt;T&gt; --> U --> F --> Args... --> U"

    Applies a function to the contained value (if any), or returns the provided default (if not).


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class U, class F, class... Args>
        std::enable_if_t<
            std::conjunction_v<
                std::is_invocable<F&&, value_type ${CVREF}, Args&&...>,
                meta::has_type<
                    std::common_type<
                        U&&,
                        std::invoke_result_t<F&&, value_type ${CVREF}>
            >>>,
        std::common_type_t<U&&, std::invoke_result_t<F&&, value_type ${CVREF}, Args&&...>>>
        map_or(U&&, F&&, Args&&...) ${CVREF};

    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe x = just("foo"s);
  assert(x.map_or(42, &std::string::size) == 3);
  maybe<std::string> y = nothing;
  assert(y.map_or(42, &std::string::size) == 42);
}
// end example
```

## map_or_else

!!! summary "maybe&lt;T&gt; --> D -->  F --> Args... --> U"

    Applies a function to the contained value (if any), or computes a default (if not).


!!! note "constraints"

    ```cpp
    requires {
        maybe<
            std::common_type_t<
                std::invoke_result_t<D&&>,
                std::invoke_result_t<F&&, T, Args&&...>
            >
        >>;
    }
    ```

!!! info "declarations"

    ```cpp
        template <class D, class F, class... Args>
        std::enable_if_t<
            std::conjunction_v<
                    std::is_invocable<D&&>,
                    std::is_invocable<F&&, value_type ${CVREF}, Args&&...>,
                    meta::has_type<
                        std::common_type<
                            std::invoke_result_t<D&&>,
                            std::invoke_result_t<F&&, value_type ${CVREF}, Args&&...>
            >>>,
        std::common_type_t<
            std::invoke_result_t<D&&>,
            std::invoke_result_t<F&&, value_type ${CVREF}, Args&&...>
        >>
        map_or_else(D&&, F&&, Args&&...) ${CVREF};

    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  int k = 21;
  maybe x = just("foo"s);
  assert(x.map_or_else([k]{ return 2 * k; }, &std::string::size) == 3);
  maybe<std::string> y = nothing;
  assert(y.map_or_else([k]{ return 2 * k; }, &std::string::size) == 42);
}
// end example
```

## ok_or

!!! summary "maybe&lt;T&gt; --> E [default=std::monostate] --> result&lt;T, E&gt;"

    Transforms the `maybe<T>` into a `result<T, E>`, mapping `just(v)` to `success(v)` and `nothing` to `failure(err)`.

    Arguments passed to ok_or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use ok_or_else, which is lazily evaluated.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class E = std::monostate>
        auto maybe<T>::ok_or(E&& err = {}) & -> result<T, std::remove_reference_t<E>> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe x = just("foo"s);
  assert(x.ok_or(0) == success("foo"s));

  maybe<std::string> y = nothing;
  assert(y.ok_or(0) == failure(0));
  assert(y.ok_or() == failure<>());
}
// end example
```

## ok_or_else

!!! summary "maybe&lt;T&gt; --> F --> result&lt;T, E&gt;"

    Transforms the `maybe<T>` into a `result<T, E>`, mapping `just(v)` to `success(v)` and `nothing` to `failure(err())`.


!!! note "constraints"

    ```cpp
    requires {
        result<T, std::invoke_result_t<F, Args...>>
    }
    ```

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F>
        std::enable_if_t<
            std::is_invocable_v<F&&>,
        result<T, std::invoke_result_t<F&&>>>
        maybe<T>::ok_or_else(F&& err) ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe x = just("foo"s);
  assert(x.ok_or_else([]{ return 0; }) == success("foo"s));

  maybe<std::string> y = nothing;
  assert(y.ok_or_else([]{ return 0; }) == failure(0));
}
// end example
```

## conj

!!! summary "maybe&lt;T&gt; --> maybe&lt;U&gt; --> maybe&lt;U&gt;"

    Returns `nothing` if the `lhs` is `nothing`, otherwise returns `rhs`.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class U>
        auto maybe<T>::conj(maybe<U> const& rhs) -> maybe<U> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  {
    maybe x = just(2);
    maybe<std::string> y = nothing;
    assert(x.conj(y) == nothing);
  }
  {
    maybe<int> x = nothing;
    maybe y = just("foo"s);
    assert(x.conj(y) == nothing);
  }
  {
    maybe x = just(2);
    maybe y = just("foo"s);
    assert(x.conj(y) == just("foo"s));
  }
  {
    maybe<int> x = nothing;
    maybe<std::string> y = nothing;
    assert(x.conj(y) == nothing);
  }
}
// end example
```

## and_then

!!! summary "maybe&lt;T&gt; --> F --> maybe&lt;U&gt;"

    Returns `nothing` if the option is `nothing`, otherwise invokes `f` with the wrapped value and returns the result.

    Some languages call this operation flatmap.


!!! note "constraints"

    ```cpp
    requires (is_maybe_v<std::invoke_result_t<F&&, T, Args...>>)
    ```


!!! info "declarations"

    ```cpp
    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, value_type ${CVREF}, Args&&...>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, value_type ${CVREF}, Args&&...>>>>,
    std::invoke_result_t<F&&, value_type ${CVREF}, Args&&...>>
    and_then(F&& f, Args&&... args) ${CVREF}
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
    auto sq = [](int x) -> maybe<int> { return just(x * x); };
    auto nope = [](...) -> maybe<int> { return nothing; };

    assert(maybe{just(2)}.and_then(sq).and_then(sq) == just(16));
    assert(maybe{just(2)}.and_then(sq).and_then(nope) == nothing);
    assert(maybe{just(2)}.and_then(nope).and_then(sq) == nothing);
    assert(nope().and_then(sq).and_then(sq) == nothing);
}
// end example
```

## filter

!!! summary "maybe&lt;T&gt; --> Pred --> maybe&lt;T&gt;"

    Returns `nothing` if the option is `nothing`, otherwise invokes predicate with the wrapped value and returns:

    - `just(t)` if predicate returns true (where t is the wrapped value), and
    - `nothing` if predicate returns false.

!!! note "constraints"

    ```cpp
    requires std::predicate<Pred>
    ```

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class Pred>
        std::enable_if_t<
            std::is_invocable_r_v<bool, Pred&&, T>,
        maybe<T>>
        maybe<T>::filter(Pred&& predicate) ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  auto is_even = [](int n) -> bool { return n % 2 == 0; };

  assert(maybe<int>{}.filter(is_even) == nothing);
  assert(maybe{just(3)}.filter(is_even) == nothing);
  assert(maybe{just(4)}.filter(is_even) == just(4));
}
// end example
```

## disj

!!! summary "maybe&lt;T&gt; --> maybe&lt;T&gt; --> maybe&lt;T&gt;"

    Returns the maybe if it contains a value, otherwise returns rhs.

    Arguments passed to or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use or_else, which is lazily evaluated.

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        auto maybe<T>::disj(maybe<T> const& rhs) -> maybe<T> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  {
    maybe x = just(2);
    maybe<int> y = nothing;
    assert(x.disj(y) == just(2));
  }
  {
    maybe<int> x = nothing;
    maybe y = just(100);
    assert(x.disj(y) == just(100));
  }
  {
    maybe x = just(2);
    maybe y = just(100);
    assert(x.disj(y) == just(2));
  }
  {
    maybe<int> x = nothing;
    maybe<int> y = nothing;
    assert(x.disj(y) == nothing);
  }
}
// end example
```

## or_else

!!! summary "maybe&lt;T&gt; --> F --> maybe&lt;T&gt;"

    Returns the maybe if it contains a value, otherwise invokes `f` and returns the result.

!!! note "constraints"

    ```cpp
    requires {
        { f() } -> std::convertible_to<maybe<T>>
    }
    ```

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F>
        std::enable_if_t<
            std::conjunction_v<
            std::is_invocable<F&&>,
            is_maybe_with<std::decay_t<std::invoke_result_t<F&&>>, T>>,
        maybe<T>>
        maybe<T>::or_else(F&& f) & ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  auto nobody = []() -> maybe<std::string> { return nothing; };
  auto vikings = []() -> maybe<std::string> { return just("vikings"s); };

  assert(maybe{just("barbarians"s)}.or_else(vikings) == just("barbarians"s));
  assert(maybe<std::string>{}.or_else(vikings) == just("vikings"s));
  assert(maybe<std::string>{}.or_else(nobody) == nothing);
}
// end example
```

## get_or_emplace

!!! summary "maybe&lt;T&gt; --> Args... --> T&"

    Emplace constructs `T` into the maybe with expression `std::forward<Args>(args)...` if it is `nothing`, then returns a mutable reference to the contained value.

!!! note "constraints"

    ```cpp
    requires std::constructible<T, Args&&...>
    ```


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class... Args>
        std::enable_if_t<
            std::is_constructible_v<T, Args&&...>,
        T&>
        get_or_emplace(Args&&... args) & ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

struct noncopyable {
  noncopyable() = default;
  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
  noncopyable(noncopyable&&) = default;
  noncopyable& operator=(noncopyable&&) = default;

  bool operator==(noncopyable&&) & { return true; }
  bool operator==(noncopyable&&) const& { return true; }
  bool operator==(noncopyable const&) & { return true; }
  bool operator==(noncopyable const&) const& { return true; }
};

int main() {
  maybe<noncopyable> x = nothing;
  auto& y = x.get_or_emplace(noncopyable{});
  assert(y == noncopyable{});
}
// end example
```

## get_or_emplace_with

!!! summary "maybe&lt;T&gt; --> F --> Args... --> T&"

    Emplace constructs `T` into the maybe with expression `std::invoke(std::forward<F>(f), std::forward<Args>(args)...)` if it is `nothing`, then returns a mutable reference to the contained value.


!!! note "constraints"

    ```cpp
    requires std::invocable<F&&, Args&&...>,
          && std::constructible<T, std::invoke_result_t<F&&, Args&&...>>>,
    ```

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F, class... Args>
        std::enable_if_t<
            std::conjunction_v<
            std::is_invocable<F&&, Args&&...>,
            std::is_constructible<T, std::invoke_result_t<F&&, Args&&...>>>,
        T&>
        get_or_emplace_with(F&& f, Args&&... args) & ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe<int> x = nothing;
  std::ignore = x.get_or_emplace_with([]{ return 5; });
  std::ignore = x.get_or_emplace_with([](auto x){ return x; }, 5);
  auto& y = x.get_or_emplace_with(&std::string::size , "12345"s);
  assert(y == 5);
  y = 7;
  assert(x == just(7));
}
// end example
```

## replace

!!! summary "maybe&lt;T&gt; --> Args... --> maybe&lt;T&gt;"

    Replaces the actual value in the maybe by expression `std::forward<Args>(args)...`, returning the old value if present, leaving a `just` value in its place without deinitializing either one.


!!! note "constraints"

    ```cpp
    requires std::constructible<T, Args&&...>
    ```


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        maybe<T> replace(Args&&... args) & ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  {
    maybe x = just(2);
    auto old = x.replace(5);
    assert(x == just(5));
    assert(old == just(2));
  }
  {
    maybe<int> x = nothing;
    auto old = x.replace(3);
    assert(x == just(3));
    assert(old == nothing);
  }
}
// end example
```

## replace_with

!!! summary "maybe&lt;T&gt; --> F --> Args... --> maybe&lt;T&gt;"

    Replaces the actual value in the maybe by expression `std::invoke(std::forward<F>(f), std::forward<Args>(args)...)`, returning the old value if present, leaving a `just` value in its place without deinitializing either one.


!!! note "constraints"

    ```cpp
    requires std::invocable<F&&, Args&&...>
          && std::constructible<T, std::invoke_result_t<F&&, Args&&...>>  
    ```


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
    template <class F, class... Args>
        maybe<T> replace_with(F&& f, Args&&... args) & ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  {
    maybe x = just(2);
    auto old = x.replace_with([]{ return 5; });
    assert(x == just(5));
    assert(old == just(2));
  }
  {
    maybe<int> x = nothing;
    auto old = x.replace_with(&std::string::size, "foo"s);
    assert(x == just(3));
    assert(old == nothing);
  }
}
// end example
```

## cloned

!!! summary "maybe&lt;T&&gt; --> maybe&lt;T&gt;"

    Maps a `maybe<T&>` to a `maybe<T>` by deep copying the contents of the maybe.


!!! note "constraints"

    ```cpp
    requires std::copyable<T>
    ```

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        auto maybe<T&>::cloned() & -> maybe<T>;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  int x = 12;
  maybe<int&> opt_x = just(x);
  assert(opt_x == just(12));
  assert(&(opt_x.unwrap()) == &x);

  auto shallow_copied = opt_x;
  assert(&(shallow_copied.unwrap()) == &x);

  auto cloned = opt_x.cloned();
  assert(cloned == just(12));
  assert(&(cloned.unwrap()) != &x);
}
// end example
```

## unwrap_or_default

!!! summary "maybe&lt;T&gt; --> T"

    Returns the contained value or a default.

    If `just`, returns the contained value, otherwise if `nothing`, returns the default value for that type.


!!! note "constraints"
    ```cpp
    template <class T>
    class maybe {
        auto maybe<T>::unwrap_or_default() const & -> std::remove_reference_t<T> ;
    };
    ```

**Remarks**

This operator shall be defined as deleted unless `is_default_constructible_v<T>` is true.

### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe<std::string> x = nothing;
  assert(x.unwrap_or_default() == ""s);
}
// end example
```


## transpose

!!! summary "maybe&lt;basic_result&lt;\_, T, E&gt;&gt; --> basic_result&lt;\_, maybe&lt;T&gt;, E&gt;"

    Transposes a `maybe` of a `result` into a `result` of a `maybe`.

    `nothing` will be mapped to `success(nothing)`.
    `just(success(_))` and `just(failure(_))` will be mapped to `success(just(_))` and `failure(_)`　(_ is a placeholder).

!!! info "declarations"

```cpp
template <class T>
class maybe {
  maybe<basic_result<_, T, E>>::transpose() const&
    -> basic_result<_, maybe<T>, E> ;
};
```


### Examples

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


## flatten

!!! summary "maybe&lt;maybe&lt;T&gt;&gt; --> maybe&lt;T&gt;"

    Converts from `maybe<maybe<T>>` to `maybe<T>`.


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        auto maybe<maybe<T>>::flatten() -> maybe<T> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe<maybe<int>> x = just(just(6));
  assert(just(6) == x.flatten());

  maybe<maybe<int>> y = just(nothing);
  assert(nothing == y.flatten());

  maybe<maybe<int>> z = nothing;
  assert(nothing == z.flatten());

  // Flattening once only removes one level of nesting:
  maybe<maybe<maybe<int>>> nest = just(just(just(6)));
  assert(just(6) == nest.flatten().flatten());
}
// end example
```


## and_finally

!!! summary "maybe&lt;T&gt; --> void"

    Invokes the provided function with following conditions:
    - if `std::invocable<F, T>`, the provided function with contained value,
    - else if `std::invocable<F, T>`, the provided function without contained value,
    - otherwise, doing nothing.


!!! info "declarations"
    ```cpp
    template <class T>
    class maybe {
        template <class F>
        constexpr
        std::enable_if_t<
            std::disjunction_v<
            std::is_invocable<F, value_type ${CVREF}>,
            std::is_invocable<F>>>
        and_finally(F&& f) ${CVREF} ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe x = just(42);
  int hook = 0;
  assert(x.and_peek([&](int const& v){ hook = v; }) == just(42));
  assert(hook == 42);
}
// end example
```

## or_finally

!!! summary "maybe&lt;T&gt; --> F --> void"

    Invokes the provided function (if nothing), or doing nothing (if any).

!!! note "constraints"

    ```cpp
    requires std::invocable<F>
    ```


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F>
        constexpr
        std::enable_if_t<
            std::is_invocable_v<F>>
        or_finally(F&& f);
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe x = nothing;
  int hook = 0;
  assert(x.or_peek([&hook]{ hook = 42; }) == nothing);
  assert(hook == 42);
}
// end example
```

## and_peek

!!! summary "maybe&lt;T&gt; --> F --> maybe&lt;T&gt;"

    Peeks the contained value if self is `just`, then returns self.

    Invokes the provided function with the contained value and then returns self (if any), or returns self without doing anything (if not).


!!! note "constraints"

    ```cpp
    requires std::invocable<F>
    ```


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F>
        constexpr
        std::enable_if_t<
            std::disjunction_v<
            std::is_invocable<F, value_type ${CVREF}>,
            std::is_invocable<F>>,
        maybe&>
        and_peek(F&& f) ${CVREF};
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
    maybe x = just(42);
    int hook = 0;
    assert(x.and_peek([&](int const& v){ hook = v; }) == just(42));
    assert(hook == 42);
}
// end example
```

## or_peek

!!! summary "maybe&lt;T&gt; --> F --> maybe&lt;T&gt;"
    
    Invokes the provided function and then returns self (if nothing), or returns self without doing anything (if any).


!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F>
        constexpr
        std::enable_if_t<
            std::is_invocable_v<F>,
        maybe&>
        or_peek(F&& f) ${CVREF} ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe x = nothing;
  int hook = 0;
  assert(x.or_peek([&hook]{ hook = 42; }) == nothing);
  assert(hook == 42);
}
// end example
```

## range_to_maybe

!!! note
    In header `<mitama/maybe/range_to_maybe.hpp>`.


!!! summary "Range --> maybe&lt;typename Range::value_type&gt;"

    The `range_to_maybe` function returns `nothing` on an empty range or `just(v)` where `v` is the first element of the `range`.


!!! info "declarations"

    ```cpp
    template <class Range>
    auto range_to_maybe(Range&& range) -> maybe<decltype(*begin(range))> ;
    ```


### Examples

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <mitama/maybe/range_to_maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  std::vector v{1,2,3};
  maybe x = range_to_maybe(v);
  assert(x == just(1));

  maybe y = range_to_maybe(std::vector<int>{});
  assert(y == nothing);

  int a[] = {1};
  maybe z = range_to_maybe(a);
  assert(z == 1);
}
// end example
```
