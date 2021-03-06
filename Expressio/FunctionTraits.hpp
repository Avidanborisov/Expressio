//--------------------------------------
// FunctionTraits: Function type traits
//--------------------------------------
//
//          Copyright kennytm (auraHT Ltd.) 2011.
//              Modified Avidanborisov 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file doc/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/*

This module provides additional type traits and related functions, missing from
the standard library.

*/

#ifndef TRAITS_HPP_9ALQFEFX7TO
#define TRAITS_HPP_9ALQFEFX7TO 1

#include <cstdlib>
#include <tuple>
#include <functional>
#include <type_traits>

namespace Expressio {
namespace Detail {

/*
.. type:: struct FunctionTraits<F>

	Obtain compile-time information about a function object *F*.

	This template currently supports the following types:

	* Normal function types (``R(T...)``), function pointers (``R(*)(T...)``)
	  and function references (``R(&)(T...)`` and ``R(&&)(T...)``).
	* Member functions (``R(C::*)(T...)``)
	* ``std::function<F>``
	* Type of lambda functions, and any other types that has a unique
	  ``operator()``.
	* Type of ``std::mem_fn`` (only for GCC's libstdc++ and LLVM's libc++).
	  Following the C++ spec, the first argument will be a raw pointer.
*/
template <typename T>
struct FunctionTraits
	: public FunctionTraits<decltype(&T::operator())>
{};

template <typename C, typename R, typename... A>
struct MemberFunctionType
{
	typedef typename std::conditional<
		std::is_const<C>::value,
		typename std::conditional<
			std::is_volatile<C>::value,
			R (C::*)(A...) const volatile,
			R (C::*)(A...) const
		>::type,
		typename std::conditional<
			std::is_volatile<C>::value,
			R (C::*)(A...) volatile,
			R (C::*)(A...)
		>::type
	>::type type;
};

template <typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType(Args...)>
{
	/*
	.. type:: type result_type

		The type returned by calling an instance of the function object type *F*.
	*/
	typedef ReturnType result_type;

	/*
	.. type:: type function_type

		The function type (``R(T...)``).
	*/
	typedef ReturnType function_type(Args...);

	/*
	.. type:: type member_function_type<OwnerType>

		The member function type for an *OwnerType* (``R(OwnerType::*)(T...)``).
	*/
	template <typename OwnerType>
	using member_function_type = typename MemberFunctionType<
		typename std::remove_pointer<typename std::remove_reference<OwnerType>::type>::type,
		ReturnType, Args...
	>::type;

	/*
	.. data:: static const size_t arity

		Number of arguments the function object will take.
	*/
	enum { arity = sizeof...(Args) };

	/*
	.. type:: type arg<n>::type

		The type of the *n*-th argument.
	*/
	template <size_t i>
	struct arg
	{
		typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	};
};

template <typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType(*)(Args...)>
	: public FunctionTraits<ReturnType(Args...)>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType(ClassType::*)(Args...)>
	: public FunctionTraits<ReturnType(Args...)>
{
	typedef ClassType& owner_type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType(ClassType::*)(Args...) const>
	: public FunctionTraits<ReturnType(Args...)>
{
	typedef const ClassType& owner_type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType(ClassType::*)(Args...) volatile>
	: public FunctionTraits<ReturnType(Args...)>
{
	typedef volatile ClassType& owner_type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType(ClassType::*)(Args...) const volatile>
	: public FunctionTraits<ReturnType(Args...)>
{
	typedef const volatile ClassType& owner_type;
};

template <typename FunctionType>
struct FunctionTraits<std::function<FunctionType>>
	: public FunctionTraits<FunctionType>
{};

#if defined(_GLIBCXX_FUNCTIONAL)
#define MEM_FN_SYMBOL_XX0SL7G4Z0J std::_Mem_fn
#elif defined(_LIBCPP_FUNCTIONAL)
#define MEM_FN_SYMBOL_XX0SL7G4Z0J std::__mem_fn
#endif

#ifdef MEM_FN_SYMBOL_XX0SL7G4Z0J

template <typename R, typename C>
struct FunctionTraits<MEM_FN_SYMBOL_XX0SL7G4Z0J<R C::*>>
	: public FunctionTraits<R(C*)>
{};
template <typename R, typename C, typename... A>
struct FunctionTraits<MEM_FN_SYMBOL_XX0SL7G4Z0J<R(C::*)(A...)>>
	: public FunctionTraits<R(C*, A...)>
{};
template <typename R, typename C, typename... A>
struct FunctionTraits<MEM_FN_SYMBOL_XX0SL7G4Z0J<R(C::*)(A...) const>>
	: public FunctionTraits<R(const C*, A...)>
{};
template <typename R, typename C, typename... A>
struct FunctionTraits<MEM_FN_SYMBOL_XX0SL7G4Z0J<R(C::*)(A...) volatile>>
	: public FunctionTraits<R(volatile C*, A...)>
{};
template <typename R, typename C, typename... A>
struct FunctionTraits<MEM_FN_SYMBOL_XX0SL7G4Z0J<R(C::*)(A...) const volatile>>
	: public FunctionTraits<R(const volatile C*, A...)>
{};

#undef MEM_FN_SYMBOL_XX0SL7G4Z0J
#endif

template <typename T>
struct FunctionTraits<T&> : public FunctionTraits<T> {};
template <typename T>
struct FunctionTraits<const T&> : public FunctionTraits<T> {};
template <typename T>
struct FunctionTraits<volatile T&> : public FunctionTraits<T> {};
template <typename T>
struct FunctionTraits<const volatile T&> : public FunctionTraits<T> {};
template <typename T>
struct FunctionTraits<T&&> : public FunctionTraits<T> {};
template <typename T>
struct FunctionTraits<const T&&> : public FunctionTraits<T> {};
template <typename T>
struct FunctionTraits<volatile T&&> : public FunctionTraits<T> {};
template <typename T>
struct FunctionTraits<const volatile T&&> : public FunctionTraits<T> {};

}}

#endif
