#ifndef MITAMA_RESULT_RESULT_IO_HPP
#define MITAMA_RESULT_RESULT_IO_HPP
#include <mitama/result/result.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
namespace mitama {
/// @brief
///   ostream output operator
///
/// @requires
///   Format<T>;
///   Format<E>
///
/// @note
///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
template <mutability _, class T, class E>
std::enable_if_t<std::conjunction_v<trait::formattable<T>, trait::formattable<E>>,
std::ostream&>
operator<<(std::ostream& os, basic_result<_, T, E> const& res) {
  using namespace std::literals::string_literals;
  auto inner_format = boost::hana::fix(boost::hana::overload_linearly(
      [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
        return boost::hana::overload_linearly(
          [](std::monostate) { return "()"s; },
          [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
          [](auto const& x) { return (boost::format("%1%") % x).str(); })
        (x);        
      },
      [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
        if (x.empty()) return "{}"s;
        using std::begin, std::end;
        auto iter = begin(x);
        std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
        while (++iter != end(x)) {
          str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
        }
        return str += "}";
      },
      [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
        if (x.empty()) return "[]"s;
        using std::begin, std::end;
        auto iter = begin(x);
        std::string str = "["s + _fmt(*iter);
        while (++iter != end(x)) {
          str += (boost::format(",%1%") % _fmt(*iter)).str();
        }
        return str += "]";
      },
      [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
        if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
          return "()"s;
        }
        else {
          return std::apply(
            [_fmt](auto const& head, auto const&... tail) {
              return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
            }, x);
        }
      }));
  return res.is_ok() ? os << boost::format("success(%1%)") % inner_format( res.unwrap() )
                     : os << boost::format("failure(%1%)") % inner_format( res.unwrap_err() );
}

}

#endif
