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


**Example**

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

    Returns the contained value or computes it from a invocable object `op`.

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

{!maybe/_APIs/ok_or_else.md!}

## conj

{!maybe/_APIs/conj.md!}

## and_then

!!! summary
    Returns `nothing` if the option is `nothing`, otherwise invokes `f` with the wrapped value and returns the result.

    Some languages call this operation flatmap.

!!! info "declarations"

    ```cpp
    template <class T>
    class maybe {
        template <class F>
        std::enable_if_t<
            std::conjunction_v<
            std::is_invocable<F&&, T&>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, T&>>>>,
        std::invoke_result_t<F&&, T&> >
        and_then(F&& f) & ;

        template <class F>
        std::enable_if_t<
            std::conjunction_v<
            std::is_invocable<F&&, T const&>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, T const&>>>>,
        std::invoke_result_t<F&&, T const&> >
        and_then(F&& f) const& ;

        template <class F>
        std::enable_if_t<
            std::conjunction_v<
            std::is_invocable<F&&, T&>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, T&&>>>>,
        std::invoke_result_t<F&&, T&> >
        and_then(F&& f) && ;
    };
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

{!maybe/_APIs/filter.md!}

## disj

{!maybe/_APIs/disj.md!}

## or_else

{!maybe/_APIs/or_else.md!}

## get_or_emplace

{!maybe/_APIs/get_or_emplace.md!}

## get_or_emplace_with

{!maybe/_APIs/get_or_emplace_with.md!}

## replace

{!maybe/_APIs/replace.md!}

## replace_with

{!maybe/_APIs/replace_with.md!}

## cloned

{!maybe/_APIs/cloned.md!}

## unwrap_or_default

{!maybe/_APIs/unwrap_or_default.md!}

## transpose

{!maybe/_APIs/transpose.md!}

## flatten

{!maybe/_APIs/flatten.md!}

## and_finally

!!! summary
    Invokes the provided function with the contained value (if any), or doing nothing (if not).

!!! info "declarations"
    ```cpp
    template <class T>
    class maybe {
        template <class F>
        constexpr
        std::enable_if_t<
            std::disjunction_v<
            std::is_invocable<F, T&>,
            std::is_invocable<F>>>
        and_finally(F&& f) & ;

        template <class F>
        std::enable_if_t<
            std::disjunction_v<
            std::is_invocable<F, T const&>,
            std::is_invocable<F>>>
        and_finally(F&& f) const& ;

        template <class F>
        std::enable_if_t<
            std::disjunction_v<
            std::is_invocable<F, T&&>,
            std::is_invocable<F>>>
        and_finally(F&& f) && ;
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

{!maybe/_APIs/or_finally.md!}

## and_peek

{!maybe/_APIs/and_peek.md!}

## or_peek

{!maybe/_APIs/or_peek.md!}

## range_to_maybe

{!maybe/_APIs/range_to_maybe.md!}
