/*!
@file
Forward declares `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_MAYBE_HPP
#define BOOST_HANA_MAYBE_MAYBE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/operators.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monad/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents an optional value.
    //!
    //! A `Maybe` either contains a value (represented as `just(x)`), or it
    //! is empty (represented as `nothing`).
    //!
    //! ## Instance of
    //! `Comparable`, `Functor`, `Applicative`, `Monad`, `Foldable`,
    //! `Searchable` and `Traversable`.
    struct Maybe { struct hana_enabled_operators : Comparable, Monad { }; };

    namespace maybe_detail {
        template <bool is_valid, typename T, typename = operators::enable_adl>
        struct maybe {
            using hana_datatype = Maybe;

            template <typename Default, typename F>
            constexpr auto maybe_impl(Default d, F) const
            { return d; }
        };

        template <typename T>
        struct maybe<true, T> {
            using hana_datatype = Maybe;
            T val;

            template <typename Default, typename F>
            constexpr auto maybe_impl(Default, F f) const
            { return f(val); }
        };

        template <typename T>
        using just = maybe<true, T>;
        using nothing = maybe<false, void>;
    }

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Create an optional value containing `x`.
    //! @relates Maybe
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp just
    constexpr auto just = [](auto x) {
        return unspecified;
    };

    //! An empty optional value.
    //! @relates Maybe
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp nothing
    constexpr unspecified nothing{};
#else
    constexpr auto just = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto x) {
        return maybe_detail::just<decltype(x)>{x};
    };

    constexpr maybe_detail::nothing nothing{};
#endif

    //! Create a `Maybe` with the result of a function, but only if a
    //! predicate is satisfied.
    //! @relates Maybe
    //!
    //! Specifically, returns `just(f(x))` if `predicate(x)` is a true-valued
    //! `Logical`, and `nothing` otherwise.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)` and returning a true-valued
    //! `Logical` if `just(f(x))` should be the resulting value, and a
    //! false-valued one if `nothing` should be the resulting value.
    //! In the current version of the library, the result of `predicate`
    //! has to be a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! @param f
    //! A function called as `f(x)` if the `predicate` returns a true-valued
    //! `Logical`, and not called at all otherwise. If the `predicate` returns
    //! a false-valued `Logical`, the `f(x)` expression is not even required
    //! to be well-formed.
    //!
    //! @param x
    //! The value to either transform and put in a `just`, or discard.
    //!
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp only_when
    constexpr auto only_when = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto predicate, auto f, auto x) {
        return eval_if(predicate(x),
            [=](auto _) { return just(_(f)(x)); },
            [](auto _) { return nothing; }
        );
    };

    //! Apply a function to the contents of a `Maybe`, with a fallback
    //! result.
    //! @relates Maybe
    //!
    //! Specifically, `maybe` takes a default value, a function and an
    //! optional value. If the optional value is `nothing`, the default
    //! value is returned. Otherwise, the function is applied to the
    //! content of the `just`.
    //!
    //!
    //! @param default_
    //! A default value returned if `m` is `nothing`.
    //!
    //! @param f
    //! A function called as `f(x)` if and only if `m` is an optional value
    //! of the form `just(x)`. In that case, the result returend by `maybe`
    //! is the result of `f`.
    //!
    //! @param m
    //! An optional value.
    //!
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp maybe
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto maybe = [](auto default_, auto f, auto m) {
        unspecified;
    };
#else
    constexpr auto maybe = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto default_, auto f, auto m) {
        return m.maybe_impl(default_, f);
    };
#endif

    //! Return whether a `Maybe` contains a value.
    //! @relates Maybe
    //!
    //! Specifically, returns a [compile-time](@ref Logical_terminology)
    //! true-valued `Logical` if `m` is of the form `just(x)` for some `x`,
    //! and a false-valued one otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp is_just
    constexpr auto is_just = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto m) {
        return maybe(false_, [](auto) { return true_; }, m);
    };

    //! Return whether a `Maybe` is empty.
    //! @relates Maybe
    //!
    //! Specifically, returns a [compile-time](@ref Logical_terminology)
    //! true-valued `Logical` if `m` is of the form `nothing`, and a
    //! false-valued one otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp is_nothing
    constexpr auto is_nothing = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto m) {
        return maybe(true_, [](auto) { return false_; }, m);
    };

    //! Return the contents of a `Maybe`, with a fallback result.
    //! @relates Maybe
    //!
    //! Specifically, returns `x` if `m` of the form `just(x)`, and `default_`
    //! if `m` is of the form `nothing`.
    //!
    //!
    //! @param default_
    //! The default value to return if `m` is `nothing`.
    //!
    //! @param m
    //! The optional value to try to retrieve the value from.
    //!
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp from_maybe
    constexpr auto from_maybe = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto default_, auto m) {
        return maybe(default_, [](auto x) { return x; }, m);
    };

    //! Extract the content of a `Maybe` or fail at compile-time.
    //! @relates Maybe
    //!
    //! Specifically, returns `x` if the optional value is of the form
    //! `just(x)`, and triggers a static assertion otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp from_just
    constexpr auto from_just = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto m) {
        auto err = [](auto ...dum) {
            constexpr bool always_false = sizeof...(dum) != 0;
            static_assert(always_false,
            "trying to extract the value inside a boost::hana::nothing "
            "with boost::hana::from_just");
        };
        return maybe(err, [](auto x) { return [=] { return x; }; }, m)();
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_MAYBE_HPP
