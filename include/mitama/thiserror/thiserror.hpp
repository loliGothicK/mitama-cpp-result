#include <mitama/anyhow/error.hpp>

#include <boost/metaparse/string.hpp>
#include <fmt/core.h>

#include <cstddef>
#include <string_view>
#include <tuple>
#include <optional>
#include <iostream>
#include <utility>

#define MITAMA_ERROR(MSG) BOOST_METAPARSE_STRING(MSG)

namespace mitama::thiserror { inline namespace v1 {
    
    template <class String, class ...Sources>
    struct error;

    template <char... String, class ...Sources>
    struct error<boost::metaparse::string<String...>, Sources...> final : anyhow::error {
        std::tuple<Sources...> sources;

        explicit error(Sources const&... sources): sources{sources...} {}

        // impl Display for thiserror::error
        friend std::ostream& operator<<(std::ostream& os, error const& err) {
            return os << std::apply([&](auto&&... src){
                return fmt::format(std::string{String...}, std::forward<decltype(src)>(src)...);
            }, err.sources);
        }

        std::string what() const override {
          std::stringstream ss;
          ss << *this;
          return ss.str();
        }
    };
}}
#if __cplusplus > 202000
namespace mitama::thiserror:: inline v2 {
    template<unsigned N>
    struct fixed_string {
        char buf[N + 1]{}; // null-terminated string

        // explicitly deleted
        fixed_string() = delete;

        // constexpr context only
        consteval fixed_string(char const* s) {
            for (std::size_t i = 0; i != N; ++i) buf[i] = s[i];
        }

        // implicit conversion operator
        constexpr operator char const*() const { return buf; }
    };

    // string literal -> fixed_string 
    template<std::size_t N> fixed_string(char const (&)[N]) -> fixed_string<N - 1>;

    // any string literal in non-template argument
    template<fixed_string Fmt, class ...Sources>
    struct error {
        inline static constexpr char const* fmt = Fmt;
        std::tuple<Sources...> sources;

        // impl Display for thiserror::error
        friend std::ostream& operator<<(std::ostream& os, error const& err) {
            return os << std::apply([&](auto&&... src){
                return fmt::format(fmt, std::forward<decltype(src)>(src)...);
            }, err.sources);
        }
    };
}
#endif
