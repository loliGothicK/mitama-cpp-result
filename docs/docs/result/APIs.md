# API reference

## is_ok

!!! summary "basic_result&lt;_, T, E&gt; --> bool"

    Returns true if the result is `success`.


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        constexpr bool is_ok() const noexcept ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    result<unsigned, std::string> x = success(3);
    assert(x.is_ok() == true);
    result<unsigned, std::string> y = failure("Some error message"s);
    assert(y.is_ok() == false);
}
// end example
```

## is_err

!!! summary "basic_result&lt;_, T, E&gt; --> bool"

    Returns true if the result is failure.


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        constexpr bool is_err() const noexcept ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    result<unsigned, std::string> x = success(3);
    assert(x.is_err() == false);

    result<unsigned, std::string> y = failure("Some error message"s);
    assert(y.is_err() == true);
}
// end example
```

## ok

!!! summary "basic_result&lt;_, T, E&gt; --> maybe&lt;T&gt;"

    Converts from `basic_result` to `maybe`.

    Converts self into a `maybe`, and discarding the failure value, if any.

    Note that these functions propagate mutability to element type of `maybe`.

!!! hint "Remarks"

    If self is rvalue and `T` is a reference type,
    this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<T>>>>`.


!!! info "declarations"

    ```cpp
    template <class T, class E>
    class basic_result<mutability::immut, T, E> {
        constexpr auto result<T, E>::ok() &
            -> maybe<const T> ;

        constexpr auto result<T, E>::ok() const &
            -> maybe<const T> ;

        constexpr auto result<T, E>::ok() &&
            -> maybe<const T> ;
    };

    template <class T, class E>
    class basic_result<mutability::mut, T, E> {
        constexpr auto mut_result<T, E>::ok() &
            -> maybe<T> ;

        constexpr auto mut_result<T, E>::ok() const &
            -> maybe<const T> ;

        constexpr auto mut_result<T, E>::ok() &&
            -> maybe<T> ;
    };
    ```



### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    result<unsigned, std::string> x = success(2);
    assert(x.ok() == just(2));
    result<unsigned, std::string> y = failure("Nothing here"s);
    assert(y.ok() == nothing);
}
// end example
```


## err

!!! summary "basic_result&lt;_, T, E&gt; --> maybe&lt;E&gt;"

    Converts from `basic_result` to `maybe`.

    Converts self into a `maybe`, and discarding the success value, if any.

    Note that these functions propagate mutability to element type of `maybe`.


!!! hint "remarks"

    If self is rvalue and `E` is a reference type,
    this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<E>>>>`.


!!! info "declarations"

    ```cpp
    template <class T, class E>
    class basic_result<mutability::immut, T, E> {
        constexpr auto result<T, E>::err() &
            -> maybe<const E> ;

        constexpr auto result<T, E>::err() const &
            -> maybe<const E> ;

        constexpr auto result<T, E>::err() &&
            -> maybe<const E> ;
    };

    template <class T, class E>
    class basic_result<mutability::mut, T, E> {
        constexpr auto mut_result<T, E>::err() &
            -> maybe<E> ;

        constexpr auto mut_result<T, E>::err() const &
            -> maybe<const E> ;

        constexpr auto mut_result<T, E>::err() &&
            -> maybe<E> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    result<unsigned, std::string> x = success(2);
    assert(x.err() == nothing);
    result<unsigned, std::string> y = failure("Nothing here"s);
    assert(y.err() == just("Nothing here"s));
}
// end example
```


## map

!!! summary "basic_result&lt;_, T, E&gt; --> O --> basic_result&lt;_, U, E&gt;"

    Maps a `result<T, E>` to `result<U, E>` by applying a function to a contained `success` value, leaving an `failure` value untouched.

    This function can be used to compose the results of two functions.


!!! note "constraints"

    ```cpp
    requires std::invocable<O, T>
    ```


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        constexpr auto map(O && op)const &
            -> std::enable_if_t<std::is_invocable_v<O, T>, result<std::invoke_result_t<O, T>, E>> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
#include <boost/lambda/lambda.hpp>
using namespace mitama;
using namespace std::string_literals;
using boost::lambda::_1;

int main() {
    result<int, int> ok = success(2);
    assert(ok.map(_1 * 2) == success(4));

    result<int, int> err = failure(2);
    assert(err.map(_1 * 2) == failure(2));
}
// end example
```


## map_or_else

!!! summary "basic_result&lt;_, T, E&gt; --> Fallback --> Map --> U, <br>where Fallback: E --> U, Map: T --> U"

    Maps a `result<T, E>` to `U` by applying a function to a contained `success` value, or a fallback function to a contained `failure` value.

    This function can be used to unpack a successful result while handling an error.


!!! note "constraints"

    ```cpp
    requires {
        typename std::common_type<std::invoke_result_t<Map, T>,
                                  std::invoke_result_t<Fallback, E> >::type;
    }
    ```

!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class Map, class Fallback>
        constexpr auto map_or_else(Fallback&& _fallback, Map&& _map) const&
            -> std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>> ;
    };
    ```


### Examples

Basic usages:

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    auto k = 21;
    {
        result<std::string, std::string> x = success("foo"s);
        assert(x.map_or_else([k](auto){ return k * 2; }, [](auto v) { return v.length(); }) == 3);
    }
    {
        result<std::string, std::string> x = failure("bar"s);
        assert(x.map_or_else([k](auto){ return k * 2; }, [](auto v) { return v.length(); }) == 42);
    }
}
// end example
```


## map_anything_else __[since v7.8.0]__

!!! summary "basic_result&lt;_, T, E&gt; --> Map --> U, <br>where<br>&ensp;Map: T --> U,<br>&ensp;Map: E --> U"

    Maps a `result<T, E>` to `U` by applying a function `_map` to a contained either `success` or `failure` value.

    This function is syntax sugar for `res.map_or_else(_map, _map)`.


!!! note "constraints"

    ```cpp
    requires (Map map, T ok, E err) {
        { map(ok) };
        { map(err) };
        typename std::common_type<std::invoke_result_t<Map, T>,
                                  std::invoke_result_t<Map, E> >::type;
    }
    ```


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class Map>
        constexpr auto map_or_else(Map&& _map) const&
            -> std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>> ;
    };
    ```


### Examples

Basic usage:

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    {
        result<std::string, std::string> x = success("foo"s);
        assert(x.map_anything_else([](auto v) { return v.length(); }) == 3);
    }
    {
        result<std::string, std::string> x = failure("bar"s);
        assert(x.map_anything_else([](auto v) { return v.length(); }) == 3);
    }
}
// end example
```


## map_err

!!! summary "basic_result&lt;_, T, E&gt; --> O --> basic_result&lt;_, T, F&gt; <br>where O: E -> F"

    Maps a `result<T, E>` to `result<T, F>` by applying a function to a contained failure value, leaving an success value untouched.

    This function can be used to pass through a successful result while handling an error.


!!! note "constraints"

    ```cpp
    requires (O&& op, E err) {
        typename std::invoke_result<O&&, E>::type;
    }
    ```


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class O>
        constexpr auto map_err(O && op) const &
            -> std::enable_if_t<std::is_invocable_v<O, E>, result<T, std::invoke_result_t<O, E>>> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    auto stringify = [](unsigned x) -> std::string{
        return "error code: "s + std::to_string(x);
    };
    result<unsigned, unsigned> x = success(2);
    assert(x.map_err(stringify) == success(2u));
    result<unsigned, unsigned> y = failure(13);
    assert(y.map_err(stringify) == failure("error code: 13"s));
}
// end example
```


## conj

!!! summary "basic_result&lt;_, T, E&gt; --> basic_result&lt;_, U, E&gt; --> basic_result&lt;_, U, E&gt;"

    `self.conj(res)` returns `res` if the result is `success`, otherwise returns the `failure` value of self.

!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <mutability _, class U>
        constexpr auto basic_result<_, T, E>::conj(basic_result<_, U, E> const&) const &
            -> basic_result<U, E> ;

        template <mutability _, class U>
        constexpr auto basic_result<_, T, E>::operator&&(basic_result<_, U, E> const &) const &
            -> basic_result<U, E> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    {
        result<unsigned, std::string> x = success(2);
        result<std::string, std::string> y = failure("late error"s);
        assert((x && y) == failure("late error"s));
    }
    {
        result<unsigned, std::string> x = failure("early error"s);
        result<std::string, std::string> y = success("foo"s);
        assert((x && y) == failure("early error"s));
    }
    {
        result<unsigned, std::string> x = failure("not a 2"s);
        result<std::string, std::string> y = failure("late error"s);
        assert((x && y) == failure("not a 2"s));
    }
    {
        result<unsigned, std::string> x = success(2);
        result<std::string, std::string> y = success("different result type"s);
        assert((x && y) == success("different result type"s));
    }
}
// end example
```

## and_then

!!! summary "basic_result&lt;_, T, E&gt; --> O --> basic_result&lt;_, U, E&gt;, <br>where O: T --> basic_result&lt;_, U, E&gt;"

    `self.and_then(op)` calls `op` if the result is success, otherwise returns the failure value of self.

    This function can be used for control flow based on result values.

!!! note "constraints"

    ```cpp
    requires (is_result_v<std::invoke_result_t<O, T>>
          and std::convertible_to<typename std::invoke_result_t<O, T>::err_type, E>)
    ```


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class O,
            std::enable_of>
        constexpr auto and_then(O&& op) &
            -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O&&, T&>, failure<E>>,
            std::invoke_result_t<O, T>> ;

        template <class O>
        constexpr auto and_then(O&& op) const&
            -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O&&, T const&>, failure<E>>,
            std::invoke_result_t<O, T>> ;

        template <class O>
        constexpr auto and_then(O&& op) &&
            -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O&&, T&&>, failure<E>>,
            std::invoke_result_t<O, T>> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    auto sq = [](unsigned x) -> result<unsigned, unsigned> { return success(x * x); };
    auto err = [](unsigned x) -> result<unsigned, unsigned> { return failure(x); };

    result<int, int> x = success(2u);
    result<int, int> y = failure(3u);

    assert(x.and_then(sq).and_then(sq) == success(16u));
    assert(x.and_then(sq).and_then(err) == failure(4u));
    assert(x.and_then(err).and_then(sq) == failure(2u));
    assert(y.and_then(sq).and_then(sq) == failure(3u));
}
// end example
```

## disj

!!! summary "basic_result&lt;_, T, E&gt; --> basic_result&lt;_, T, F&gt; --> basic_result&lt;_, T, F&gt;"

    `self.disj(res)` returns `res` if the result is `failure`, otherwise returns the `success` value of self.

    Arguments passed to or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `or_else`, which is lazily evaluated.


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class F>
        constexpr auto basic_result<_, T, E>::disj(basic_result<_, T, F> const& res) const &
            -> basic_result<_, T, F> ;

        template <class F>
        constexpr auto basic_result<_, T, E>::operator||(basic_result<_, T, F> const& res) const &
            -> basic_result<_, T, F> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    {
        result<unsigned, std::string> x = success(2);
        result<unsigned, std::string> y = failure("late error"s);
        assert(x.disj(y) ==  success(2u));
        assert((x || y) ==  success(2u));
    }
    {
        result<unsigned, std::string> x = failure("early error"s);
        result<unsigned, std::string> y = success(2);
        assert(x.disj(y) ==  success(2u));
        assert((x || y) ==  success(2u));
    }
    {
        result<unsigned, std::string> x = failure("not a 2"s);
        result<unsigned, std::string> y = failure("late error"s);
        assert(x.disj(y) ==  failure("late error"s));
        assert((x || y) ==  failure("late error"s));
    }
    {
        result<unsigned, std::string> x = success(2);
        result<unsigned, std::string> y = success(100);
        assert(x.disj(y) ==  success(2u));
        assert((x || y) ==  success(2u));
    }
}
// end example
```


## or_else

!!! summary "basic_result&lt;_, T, E&gt; --> O --> basic_result&lt;_, T, F&gt;, <br>where O: E -> basic_result&lt;_, T, F&gt;"

    `self.or_else(op)` calls `op` if the result is `failure`, otherwise returns the `success` value of self.

    This function can be used for control flow based on result values.


!!! note "constraints"

    ```cpp
    requires (is_result_v<std::invoke_result_t<O, T>>
          and std::convertible_to<typename std::invoke_result_t<O, T>::ok_type, T>)
    ```


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class O>
        constexpr auto basic_result<_, T, E>::or_else(O && op) const &
            -> std::invoke_result<O&&, E> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    auto sq = [](unsigned x) -> result<unsigned, unsigned> { return success(x * x); };
    auto err = [](unsigned x) -> result<unsigned, unsigned> { return failure(x); };

    result<int, int> x = success(2u);
    result<int, int> y = failure(3u);

    assert(x.or_else(sq).or_else(sq) == success(2u));
    assert(x.or_else(err).or_else(sq) == success(2u));
    assert(y.or_else(sq).or_else(err) == success(9u));
    assert(y.or_else(err).or_else(err) == failure(3u));
}
// end example
```


## unwrap_or

!!! summary "basic_result&lt;_, T, E&gt; --> T const& --> T"

    `self.unwrap_or(optb)` unwraps a result, yielding the content of an `success`. Else, it returns `optb`.

    Arguments passed to unwrap_or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `unwrap_or_else`, which is lazily evaluated.


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        auto basic_result<_, T, E>::unwrap_or(T const& optb) const noexcept
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    result<unsigned, unsigned> ok = success(2);
    result<unsigned, unsigned> err = failure(2);
    assert(ok.unwrap_or(1u) == 2u);
    assert(err.unwrap_or(1u) == 1u);
}
// end example
```

## unwrap_or_else

!!! summary "basic_result&lt;_, T, E&gt; --> O --> T, <br>where O: E --> T"

    `self.unwrap_or_else(op)` unwraps a result, yielding the content of an `success`. If the value is an `failure` then it invokes `op` with its value.


!!! ntoe "constraints"

    ```cpp
    requires (O&& op, E err) {
        { std::invoke(op, err) } -> std::convertible_to<T>;
    }
    ```


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class O>
        auto basic_result<_, T, E>::unwrap_or_else(O && op) const noexcept
            -> std::enable_if_t<std::is_invocable_r_v<T, O, E>, T> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    result<int, std::string> ok = success(2);
    result<int, std::string> err = failure("foo"s);
    assert(ok.unwrap_or_else(&std::string::size) == 2);
    assert(err.unwrap_or_else(&std::string::size) == 3);
}
// end example
```

## unwrap

!!! summary "basic_result&lt;_, T, E&gt; --> E"

    Unwraps a result, yielding the content of an `success`.


!!! warning "exceptions"

    Raise `mitama::runtime_panic` if a result is containing `failure` value.


!!! hint "remarks"

    If self is rvalue and `T` is a reference type,
    this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<T>>>>`.


!!! info "declarations"

    ```cpp
    template <class T, class E>
    class basic_result<mutability::immut, T, E> {
        auto unwrap() & -> const T ;

        auto unwrap() const& -> const T ;

        auto unwrap() && -> const T ;
    };

    template <class T, class E>
    class basic_result<mutability::mut, T, E> {
        auto unwrap() & -> T ;

        auto unwrap() const& -> const T ;

        auto unwrap() && -> T ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    {
        result<unsigned, std::string> x = success(2);
        assert(x.unwrap() == 2);
    }
    try {
        result<unsigned, std::string> x = failure("emergency failure"s);
        x.unwrap(); // panics with `emergency failure`
    }
    catch ( mitama::runtime_panic const& panic ) {
        std::cerr << panic.what() << std::endl;
    }
}
// end example
```

## unwrap_err

!!! summary "basic_result&lt;_, T, E&gt; --> E"

    Unwraps a result, yielding the content of an `failure`.


!!! warning "exceptions"

    Raise `mitama::runtime_panic` if a result is containing `success` value.


!!! hint "remarks"

    If self is rvalue and `E` is a reference type,
    this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<E>>>>`.


!!! info "declarations"

    ```cpp
    template <class T, class E>
    class basic_result<mutability::immut, T, E> {
        auto unwrap_err() & -> const E ;

        auto unwrap_err() const& -> const E ;

        auto unwrap_err() && -> const E ;
        };

    template <class T, class E>
        class basic_result<mutability::mut, T, E> {
        auto mut_unwrap_err() & -> E ;

        auto mut_unwrap_err() const& -> const E ;

        auto mut_unwrap_err() && -> E ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    try {
        result<unsigned, std::string> x = success(2);
        x.unwrap_err(); // panics with `2`
    }
    catch (runtime_panic const &panic)
    {
        std::cerr << panic.what() << std::endl;
    }
    {
        result<unsigned, std::string> x = failure("emergency failure"s);
        assert(x.unwrap_err() == "emergency failure"s);
    }
}
// end example
```


## unwrap_or_default

!!! summary "basic_result&lt;_, T, E&gt; --> T"

    Returns the contained value or a default.

    If `success`, returns the contained value, otherwise if `failure`, returns the default value for that type.


!!! note "constraints"

    ```cpp
    requires std::default_initializable<T>
    ```


!!! hint "remarks"

    This operator shall be defined as deleted unless `is_default_constructible_v<T>` is true.


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        auto basic_result<_, T, E>::unwrap_or_default() const & -> T ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
    result<int> good = success(1909);
    result<int> bad = failure<>();
    auto good_year = good.unwrap_or_default();
    auto bad_year = bad.unwrap_or_default();
    assert(1909 == good_year);
    assert(0 == bad_year);
}
// end example
```


## transpose

!!! summary "basic_result&lt;_, maybe&ltT&gt;, E&gt; --> maybe&lt;basic_result&lt;_, T, E&gt;&gt;"

    Transposes a `result` of a `maybe` into a `maybe` of a `result`.

    `success(nothing)` will be mapped to `nothing`.
    `success(just(v))` and `failure(v)` will be mapped to `just(success(v))` and `just(failure(v))`.

!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        auto basic_result<_, maybe<T>, E>::transpose()
            -> maybe<basic_result<_, T, E>> ;

        auto basic_result<_, maybe<T>, E>::transpose()
            -> maybe<basic_result<_, T, E>> ;
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;

int main() {
    result<maybe<int>, std::string> x = success(just(5));
    maybe<result<int, std::string>> y = just(success(5));
    assert(x.transpose() == y);
}
// end example
```


## and_finally

!!! summary "basic_result&lt;T, E&gt; --> F --> void"

    Invokes the provided function with the contained success value (if success), or doing nothing (if failure).
    The return value of F will be discarded whether void or not.


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class F>
        constexpr
        std::enable_if_t<std::is_invocable_v<F&&, ok_type ${CVREF}>>
        and_finally(F&& f) ${CVREF}
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
using namespace mitama;

int main() {
  result<int, std::string> x = success(42);
  int hook = 0;
  x.and_finally([&](int const& v){ hook = v; });
  assert(hook == 42);
}
// end example
```


## or_finally

!!! summary "basic_result&lt;T, E&gt; --> F --> void"

    Invokes the provided function with contained failure value (if failure), or doing nothing (if success).
    The return value of F will be discarded whether void or not.


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class F>
        constexpr
        std::enable_if_t<std::is_invocable_v<F&&, err_type ${CVREF}>>
        and_finally(F&& f) ${CVREF}
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <string>
#include <cassert>
using namespace mitama;

int main() {
    using namespace std::literals;

    std::string hook = "default";
    result<int, std::string> x = success(42);

    x.or_finally([&hook](std::string v){
        hook = v;
    });

    assert(hook == "default"s);

    result<int, std::string> y = failure("error"s);

    y.or_finally([&hook](std::string v){
        hook = v;
    });

    assert(hook == "error"s);
}
// end example
```


## and_peek

!!! summary "basic_result&lt;T, E&gt; --> F --> basic_result&lt;T, E&gt;"

    Peeks the contained success value and then returns self.

    Invokes the provided function with the contained value and then return self (if success), or return self without doing anything (if failure).


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class F>
        constexpr
        std::enable_if_t<
            std::disjunction_v<
            std::is_invocable<F, ok_type ${CVREF}>,
            std::is_invocable<F>>,
        basic_result&>
        and_peek(F&& f) ${CVREF}
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
using namespace mitama;

int main() {
    result<int, std::string> x = success(42);
    int hook = 0;
    assert(x.and_peek([&](int const& v){ hook = v; }) == success(42));
    assert(hook == 42);
}
// end example
```


## or_peek

!!! summary "basic_result&lt;T, E&gt; --> F --> basic_result&lt;T, E&gt;"

    Peeks the contained failure value and then returns self.

    Invokes the provided function and then return self (if failure), or return self without doing anything (if success).


!!! info "declarations"

    ```cpp
    template <mutability _mu, class T, class E>
    class basic_result {
        template <class F>
        constexpr
        std::enable_if_t<
            std::disjunction_v<
            std::is_invocable<F, err_type ${CVREF}>,
            std::is_invocable<F>>,
        basic_result&>
        or_peek(F&& f) ${CVREF}
    };
    ```


### Examples

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
using namespace mitama;

int main() {
    maybe x = nothing;
    int hook = 0;
    assert(x.or_peek([&]{ hook = 42; }) == nothing);
    assert(hook == 42);
}
// end example
```
