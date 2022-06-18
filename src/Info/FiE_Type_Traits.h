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
		// Type Traits
		// --------------------------------------------------------------------------

		template < typename T >
		using base_type_t = std::remove_pointer_t< std::decay_t< T > >;

		template < typename T >
		concept is_base_type = requires
		{
			{ std::is_same_v< T, base_type_t< T > > };
		};
		

		// --------------------------------------------------------------------------
		// Function Traits
		// --------------------------------------------------------------------------
		namespace details
		{
			//<! Base class containing information of callback / function
			//   Default = void(void::*)(void)
			template
			<
				typename ReturnType,
				typename ClassType,
				typename... Args
			>
			struct fn_traits_info
			{
				using return_type_t = ReturnType;
				using class_type_t  = ClassType;
				using args_types_t  = std::tuple< Args... >;

				static constexpr auto args_count_v = sizeof...(Args);
			};
		}

		//<! Base template
		//   eg. fn_traits <int>
		template < typename NonFunction >
		struct fn_traits;

		//<! Specialized template for functors / callbacks / lambdas
		template < typename T >
		concept has_functor = requires (std::remove_pointer_t< T > _value)
		{
			{ &_value.operator() };
		};

		template < has_functor Callback >
		struct fn_traits < Callback >
			: fn_traits < decltype(&base_type_t< Callback >::operator()) >
		{
		};

		//<! Specialized template for normal functions
		template < typename ReturnType, typename... Args>
		struct fn_traits < ReturnType(Args...) >
			: details::fn_traits_info < ReturnType, void, Args...  >
		{
		};

		//<! Specialized template for pointers to functions
		template < typename ReturnType, typename... Args >
		struct fn_traits < ReturnType(*)(Args...) >
			: details::fn_traits_info < ReturnType, void, Args... >
		{
		};

		//<! Specialized template for member functions
		template < typename ReturnType, typename ClassType, typename... Args >
		struct fn_traits < ReturnType(ClassType::*)(Args...) >
			: details::fn_traits_info < ReturnType, ClassType, Args... >
		{
		};

		template < typename ReturnType, typename ClassType, typename... Args >
		struct fn_traits < ReturnType(ClassType::*)(Args...) const >
			: details::fn_traits_info < ReturnType, ClassType, Args... >
		{
		};


		// --------------------------------------------------------------------------
		// Function traits concepts
		// --------------------------------------------------------------------------

		//<! Defines the constraints for checking if Type is a function
		template < typename CallbackType >
		concept is_function = requires
		{
			{ std::is_function_v< CallbackType > | has_functor< CallbackType > };
		};

		//<! Defines the constraints for a void function ( void(Args ...) )
		template < typename CallbackType >
		concept is_return_void_fn = requires
		{
			{ std::is_function_v< CallbackType > | has_functor< CallbackType > };
			{ std::is_same_v <typename fn_traits< CallbackType >::return_type_t, void > };
		};

		//<! Defines the constraints for an empty function ( void(void) )
		template < typename CallbackType >
		concept is_empty_fn = requires
		{
			{ has_functor< CallbackType > };
			{ std::is_same_v < fn_traits< CallbackType >::return_type_t, void > };
			{ fn_traits< CallbackType >::args_count_v == 0 };
		};


		// --------------------------------------------------------------------------
		// Tuple Expressions
		// --------------------------------------------------------------------------
		namespace details
		{
			//<! Get the index of a type in a tuple
			template < typename SearchType, typename NonTuple >
			struct tuple_to_index;

			template < typename SearchType, typename... OtherTypes >
			struct tuple_to_index < SearchType, std::tuple < SearchType, OtherTypes... > >
			{
				static constexpr auto value = 0;
			};

			template < typename SearchType, typename CurrentType, typename... OtherTypes >
			struct tuple_to_index < SearchType, std::tuple< CurrentType, OtherTypes... > >
			{
				static constexpr auto value = 1U + tuple_to_index < SearchType, std::tuple< OtherTypes... > >::value;
			};

			//<! Templated Function Method
			template < typename Tuple, typename SearchType, size_t Count = 0 >
			consteval int get_index_from_tuple(const Tuple& _tupleToSearch)
			{
				static_assert(Count < std::tuple_size_v< Tuple >);

				// Found first instance of type  
				if constexpr (std::is_same_v < CurrentType, std::tuple_element< Count, Tuple >::type)
					return Count;

				// Go to next element to search for type
				return get_index_from_tuple< Tuple, SearchType, Count + 1 >(_tupleToSearch);
			}
		}

		template < typename SearchType, typename Tuple >
		constexpr auto tuple_to_index_v = tuple_to_index < SearchType, Tuple >::value;

		template < typename Tuple >
		constexpr auto null_tuple_v = static_cast< Tuple* >(nullptr);

		template < typename... TupleArgs >
		constexpr auto make_null_tuple_from_args_v = static_cast< std::tuple< TupleArgs... >* >(nullptr);
	}
}
