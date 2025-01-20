# Meta functions


## is_result

```cpp
template <class>
struct is_result : std::false_type {};

template <class T, class E>
struct is_result<result<T, E>> : std::true_type {};

template <class T>
inline constexpr bool is_result_v = is_result<T>::value;
```


## is_result_with

```cpp
template <class, class...>
struct is_result_with : std::false_type {};

template <class T, class E>
struct is_result_with<result<T, E>, ok_t<T>> : std::true_type {};

template <class T, class E>
struct is_result_with<result<T, E>, err_t<E>> : std::true_type {};

template <class T, class E>
struct is_result_with<result<T, E>, ok_t<T>, err_t<E>> : std::true_type {};

template <class T, class... Requires>
inline constexpr bool is_result_with_v = is_result_with<T, Requires...>::value;
```

