/******************************************************************************
filename:	FiE_FunctionTraits.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Helper class that contains functions to retrieve information from callable
	objects and function types.

******************************************************************************/
#pragma once

namespace FireflyEngine::tools
{
	namespace traits
	{
		// --------------------------------------------------------------------------
		// Function Traits
		// --------------------------------------------------------------------------
		namespace details
		{
			//<! Base class containing information of callback / function
			//   Default = void(void::*)(void)
			template
				<
				typename ReturnType = void,
				typename ClassType = void,
				typename... Args
				>
				struct fn_traits_info
			{
				using return_type_t = ReturnType;
				using class_type_t = ClassType;
				using args_types_t = std::tuple< Args... >;

				static constexpr auto args_count_v = sizeof...(Args);
			};

			//<! Base template
			//   eg. fn_traits <int>
			template < typename NonFunction >
			struct fn_traits;

			//<! Specialized template for normal functions
			template < typename ReturnType, typename... Args>
			struct fn_traits < ReturnType(Args...) >
				: fn_traits_info < ReturnType, void, Args...  >
			{
			};

			//<! Specialized template for pointers to functions
			template < typename ReturnType, typename... Args >
			struct fn_traits < ReturnType(*)(Args...) >
				: fn_traits_info < ReturnType, void, Args... >
			{
			};

			//<! Specialized template for member functions
			template < typename ReturnType, typename ClassType, typename... Args >
			struct fn_traits < ReturnType(ClassType::*)(Args...) >
				: fn_traits_info < ReturnType, ClassType, Args... >
			{
			};

			template < typename ReturnType, typename ClassType, typename... Args >
			struct fn_traits < ReturnType(ClassType::*)(Args...) const >
				: fn_traits_info < ReturnType, ClassType, Args... >
			{
			};

			//<! Specialized template for functors / callbacks / lambdas
			template < typename T >
			concept has_functor = requires (std::remove_pointer_t< T > _value)
			{
				{ &_value.operator() };
			};

			template < has_functor Callback >
			struct fn_traits < Callback >
				: fn_traits < decltype(&std::remove_pointer_t< Callback >::operator()) >
			{
			};
		}

		// --------------------------------------------------------------------------
		// Function traits concepts
		// --------------------------------------------------------------------------
		
		//<! Defines the constraints for a void function ( void(Args ...) )
		template < typename CallbackType >
		concept is_void_fn = requires
		{
			{ std::is_same_v < details::fn_traits<CallbackType>::return_type_t, void > };
		};

		//<! Defines the constraints for an empty function ( void(void) )
		template < typename CallbackType >
		concept is_empty_fn =
			std::is_invocable_v < CallbackType > &&
			std::is_same_v < std::invoke_result_t < CallbackType >, void >;


		// --------------------------------------------------------------------------
		// Expressions
		// --------------------------------------------------------------------------
	}
}
