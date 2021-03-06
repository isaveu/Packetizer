#pragma once

#ifndef ARX_TYPE_TRAITS_TYPE_TRAITS_H
#define ARX_TYPE_TRAITS_TYPE_TRAITS_H

#ifdef ARX_TYPE_TRAITS_DISABLED

namespace std {

    using nullptr_t = decltype(nullptr);

    // integral_constant

    template<class T, T v>
    struct integral_constant
    {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;


    template <class T>
    T declval(); // no implementation


    template <bool, typename T = void>
    struct enable_if;
    template <typename T>
    struct enable_if <true, T> { using type = T; };


    template<bool, class T, class F>
    struct conditional { using type = T; };
    template<class T, class F>
    struct conditional<false, T, F> { using type = F; };


    template <class T> struct remove_cv                   { using type = T; };
    template <class T> struct remove_cv<const T>          { using type = T; };
    template <class T> struct remove_cv<volatile T>       { using type = T; };
    template <class T> struct remove_cv<const volatile T> { using type = T; };

    template <class T> struct remove_const                { using type = T; };
    template <class T> struct remove_const<const T>       { using type = T; };

    template <class T> struct remove_volatile             { using type = T; };
    template <class T> struct remove_volatile<volatile T> { using type = T; };

    template <class T> struct remove_pointer                    { using type = T; };
    template <class T> struct remove_pointer<T*>                { using type = T; };
    template <class T> struct remove_pointer<T* const>          { using type = T; };
    template <class T> struct remove_pointer<T* volatile>       { using type = T; };
    template <class T> struct remove_pointer<T* const volatile> { using type = T; };

    template <class T> struct remove_reference      { using type = T; };
    template <class T> struct remove_reference<T&>  { using type = T; };
    template <class T> struct remove_reference<T&&> { using type = T; };

    template<class T> struct remove_extent                 { typedef T type; };
    template<class T> struct remove_extent<T[]>            { typedef T type; };
    template<class T, size_t N> struct remove_extent<T[N]> { typedef T type; };


    template<class T>
    T&& move(T& t){ return static_cast<T&&>(t); }


    template <class T>
    constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }
    template <class T>
    constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
    {
        return static_cast<T&&>(t);
    }


    namespace detail
    {
        template <class T>
        struct type_identity { using type = T; };
        template <class T>
        auto try_add_pointer(int) -> type_identity<typename remove_reference<T>::type*>;
        template <class T>
        auto try_add_pointer(...) -> type_identity<T>;
    }
    template <class T>
    struct add_pointer : decltype(detail::try_add_pointer<T>(0)) {};


    template <typename T, typename U>
    struct is_same : false_type {};
    template <typename T>
    struct is_same <T, T> : true_type {};


    template <typename T>
    struct is_integral : false_type {};
    template <> struct is_integral <bool> : true_type {};
    template <> struct is_integral <char> : true_type {};
    template <> struct is_integral <signed char> : true_type {};
    template <> struct is_integral <unsigned char> : true_type {};
    template <> struct is_integral <char16_t> : true_type {};
    template <> struct is_integral <char32_t> : true_type {};
    template <> struct is_integral <wchar_t> : true_type {};
    template <> struct is_integral <short> : true_type {};
    template <> struct is_integral <unsigned short> : true_type {};
    template <> struct is_integral <int> : true_type {};
    template <> struct is_integral <unsigned> : true_type {};
    template <> struct is_integral <long> : true_type {};
    template <> struct is_integral <unsigned long> : true_type {};
    template <> struct is_integral <long long> : true_type {};
    template <> struct is_integral <unsigned long long> : true_type {};


    template <typename T>
    struct is_floating_point : false_type {};
    template <> struct is_floating_point<float> : true_type {};
    template <> struct is_floating_point<double> : true_type {};
    template <> struct is_floating_point<long double> : true_type {};


    template <typename T>
    struct is_arithmetic
    : conditional<
        is_integral<T>::value || is_floating_point<T>::value,
        true_type,
        false_type
    >::type
    {};


    namespace detail
    {
        template <typename T, bool = is_arithmetic<T>::value>
        struct is_signed : integral_constant<bool, T(-1) < T(0)> {};
        template <typename T>
        struct is_signed<T, false> : false_type {};
    }
    template <typename T>
    struct is_signed : detail::is_signed<T>::type {};


    namespace detail
    {
        template<typename T, bool = is_arithmetic<T>::value>
        struct is_unsigned : integral_constant<bool, T(0) < T(-1)> {};
        template<typename T>
        struct is_unsigned<T, false> : false_type {};
    }
    template<typename T>
    struct is_unsigned : detail::is_unsigned<T>::type {};


    template <class T> struct is_pointer_helper     : false_type {};
    template <class T> struct is_pointer_helper<T*> : true_type {};
    template <class T> struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};


    template<class T>
    struct is_array : false_type {};
    template<class T>
    struct is_array<T[]> : true_type {};
    template<class T, size_t N>
    struct is_array<T[N]> : true_type {};


    namespace details
    {
        template <class... Ts>
        struct Tester { using type = void; };
        template <class... Ts>
        using void_t = typename Tester<Ts...>::type;
        template<template<class...>class Z, class, class...Ts>
        struct can_apply : false_type{};
        template<template<class...>class Z, class...Ts>
        struct can_apply<Z, void_t<Z<Ts...>>, Ts...> : true_type{};

        template<class From, class To>
        using try_convert = decltype(To{declval<From>()} );
    }
    template<template<class...>class Z, class...Ts>
    using can_apply = details::can_apply<Z, void, Ts...>;
    template<class From, class To>
    struct is_convertible : can_apply <details::try_convert, From, To> {};
    template<>
    struct is_convertible<void,void> : true_type{};


    // primary template
    template<class>
    struct is_function : false_type { };
    // specialization for regular functions
    template<class Ret, class... Args>
    struct is_function<Ret(Args...)> : true_type {};
    // specialization for variadic functions such as std::printf
    template<class Ret, class... Args>
    struct is_function<Ret(Args......)> : true_type {};
    // specialization for function types that have cv-qualifiers
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) volatile> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const volatile> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) volatile> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const volatile> : true_type {};
    // specialization for function types that have ref-qualifiers
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) volatile &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const volatile &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) volatile &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const volatile &> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) &&> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const &&> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) volatile &&> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const volatile &&> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) &&> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const &&> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) volatile &&> : true_type {};
    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const volatile &&> : true_type {};


    template <class T>
    class decay
    {
        typedef typename remove_reference<T>::type U;
    public:
        typedef typename conditional<
            is_array<U>::value,
            typename remove_extent<U>::type*,
            typename conditional<
                is_function<U>::value,
                typename add_pointer<U>::type,
                typename remove_cv<U>::type
            >::type
        >::type type;
    };


    namespace details
    {
        template<class T> struct tag { using type=T; };
        template<class Tag> using type_t = typename Tag::type;

        template<class G, class...Args>
        using invoke_t = decltype( declval<G>()(declval<Args>()...) );

        template<class Sig, class = void>
        struct result_of {};
        template<class G, class...Args>
        struct result_of<G(Args...), void_t<invoke_t<G, Args...>>>
        : tag <invoke_t<G, Args...>>
        {};
    }
    template<class Sig>
    using result_of = details::result_of<Sig>;

} // namespace std

#include "tuple.h"
#include "functional.h"

#endif // ARX_TYPE_TRAITS_DISABLE_STL


#if __cplusplus < 201402L // C++11

namespace std {

    template <bool B, typename T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;

    template <typename T>
    using decay_t = typename std::decay<T>::type;


    template<typename T, T ...Ts>
    struct integer_sequence
    {
        using type = integer_sequence;
        using value_type = T;
        static constexpr size_t size() noexcept { return sizeof...(Ts); }
    };
    template <size_t ...Is>
    using index_sequence = integer_sequence<size_t, Is...>;

    // https://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence

    template<typename S1, typename S2>
    struct concat_impl;
    template<typename S1, typename S2>
    using concat = typename concat_impl<S1, S2>::type;

    template<size_t... I1, size_t... I2>
    struct concat_impl <index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
    template<size_t N>
    struct make_index_sequence_impl;
    template<size_t N>
    using make_index_sequence = typename make_index_sequence_impl<N>::type;
    template<size_t N>
    struct make_index_sequence_impl
    : concat<make_index_sequence <N/2>, make_index_sequence <N - N/2>> {};
    template<>
    struct make_index_sequence_impl <0> : index_sequence<>{};
    template<>
    struct make_index_sequence_impl <1> : index_sequence<0>{};

} // namespace std

#endif // C++11


#if __cplusplus < 201703L // C++14

namespace std {

    template <class... Ts>
    struct Tester { using type = void; };
    template <class... Ts>
    using void_t = typename Tester<Ts...>::type;

    template <typename ...Args>
    struct disjunction : std::false_type {};
    template <typename Arg>
    struct disjunction <Arg> : Arg::type {};
    template <typename Arg, typename ...Args>
    struct disjunction <Arg, Args...> : std::conditional<Arg::value, Arg, disjunction<Args...>>::type {};

    template <typename ...Args>
    struct conjunction : std::true_type {};
    template <typename Arg>
    struct conjunction <Arg> : Arg::type {};
    template <typename Arg, typename ...Args>
    struct conjunction <Arg, Args...> : std::conditional<Arg::value, conjunction<Args...>, Arg>::type {};

    template <typename T>
    struct negation : std::integral_constant<bool, !T::value> {};


    // https://qiita.com/_EnumHack/items/92e6e135174f1f781dbb
    // without decltype(auto)

    template <class F, class Tuple, size_t... I>
    constexpr auto apply_impl(F&& f, Tuple&& t, index_sequence<I...>)
    -> decltype(f(std::get<I>(std::forward<Tuple>(t))...))
    {
        return f(std::get<I>(std::forward<Tuple>(t))...);
    }
    template <class F, class Tuple>
    constexpr auto apply(F&& f, Tuple&& t)
    -> decltype(apply_impl(
        std::forward<F>(f),
        std::forward<Tuple>(t),
        make_index_sequence<std::tuple_size<decay_t<Tuple>>::value>{}
    ))
    {
        return apply_impl(
            std::forward<F>(f),
            std::forward<Tuple>(t),
            make_index_sequence<std::tuple_size<decay_t<Tuple>>::value>()
        );
    }

} // namespace std

#endif // C++14


#if __cplusplus >= 201703L // C++17, C+=2a

namespace std {

} // namespace std

#endif // C++17, C++2a


namespace arx { // others

    template <class AlwaysVoid, template<class...> class Check, class... T>
    struct is_detected_impl : std::false_type {};
    template <template <class...> class Check, class... T>
    struct is_detected_impl <std::void_t<Check<T...>>, Check, T...> : std::true_type {};
    template <template <class...> class Check, class... T>
    using is_detected = is_detected_impl<void, Check, T...>;


    template <typename T>
    struct is_callable {
        template <typename U, decltype(&U::operator()) = &U::operator()>
        struct checker {};
        template <typename U> static std::true_type  test(checker<U> *);
        template <typename>   static std::false_type test(...);
        static constexpr bool value = decltype(test<T>(nullptr))::value;
    };
    template <typename R, typename ... Arguments>
    struct is_callable<R(*)(Arguments ...)> {
        static constexpr bool value = true;
    };
    template <typename R, typename ... Arguments>
    struct is_callable<R(*&)(Arguments ...)> {
        static constexpr bool value = true;
    };
    template <typename R, typename ... Arguments>
    struct is_callable<R(&)(Arguments ...)> {
        static constexpr bool value = true;
    };
    template <typename R, typename ... Arguments>
    struct is_callable<R(Arguments ...)> {
        static constexpr bool value = true;
    };
    template <typename R, typename ... Arguments>
    struct is_callable<std::function<R(Arguments ...)>> {
        static constexpr bool value = true;
    };


    namespace detail {
        template <typename ret, typename ... arguments>
        struct function_traits {
            static constexpr size_t arity = sizeof...(arguments);
            using result_type = ret;
            using arguments_types_tuple = std::tuple<arguments ...>;
            // template <size_t index>
            // using argument_type = type_at<index, arguments ...>;
            using function_type = std::function<ret(arguments ...)>;
            template <typename function_t>
            static constexpr function_type cast(function_t f) {
                return static_cast<function_type>(f);
            }
        };
    };
    template <typename T>
    struct function_traits : public function_traits<decltype(&T::operator())> {};
    template <typename class_type, typename ret, typename ... arguments>
    struct function_traits<ret(class_type::*)(arguments ...) const>
    : detail::function_traits<ret, arguments ...> {};

    template <typename class_type, typename ret, typename ... arguments>
    struct function_traits<ret(class_type::*)(arguments ...)>
    : detail::function_traits<ret, arguments ...> {};

    template <typename ret, typename ... arguments>
    struct function_traits<ret(*)(arguments ...)>
    : detail::function_traits<ret, arguments ...> {};

    template <typename ret, typename ... arguments>
    struct function_traits<ret(*&)(arguments ...)>
    : detail::function_traits<ret, arguments ...> {};

    template <typename ret, typename ... arguments>
    struct function_traits<ret(arguments ...)>
    : detail::function_traits<ret, arguments ...> {};

    template <typename ret, typename ... arguments>
    struct function_traits<std::function<ret(arguments ...)>>
    : detail::function_traits<ret, arguments ...> {};

} // namespace arx

#endif // ARX_TYPE_TRAITS_TYPE_TRAITS_H
