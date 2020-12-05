# Meta functions

## is_just

```cpp
template <class>
struct is_just : std::false_type {};

template <class T>
struct is_just<just_t<T>> : std::true_type {};

template <class T>
inline constexpr bool is_just_v = is_just<T>::value;
```

## is_nothing

```cpp
template <class>
struct is_nothing : std::false_type {};

template <class T>
struct is_nothing<nothing_t<T>> : std::true_type {};

template <class T>
inline constexpr bool is_nothing_v = is_nothing<T>::value;
```


## is_maybe

```cpp
template <class>
struct is_maybe : std::false_type {};

template <class T>
struct is_maybe<maybe<T>> : std::true_type {};

template <class T>
inline constexpr bool is_maybe_v = is_maybe<T>::value;
```

