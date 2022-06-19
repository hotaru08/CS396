/******************************************************************************
filename:	FiE_Query.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype's memory Pool storing an array of pointers that points to the
	respective components the archetype refers to.

******************************************************************************/
#pragma once

namespace FireflyEngine::query
{
	// --------------------------------------------------------------------------
	// Query Types
	// --------------------------------------------------------------------------
	template < typename... Components >
	struct MustHave final 
	{ };

	template < typename... Components >
	struct OneOf final
	{ };

	template < typename... Components >
	struct NoneOf final
	{ };


	// --------------------------------------------------------------------------
	// Query Functions
	// --------------------------------------------------------------------------
	struct instance final
	{
		// Compare with archetype to see if archetype fits what is being queried
		constexpr bool operator==(const tools::Bits& _archetypeBits) const noexcept
		{
			const size_t size = _archetypeBits.m_bits.size();
			tools::Bits::bits_chunk_t archetypeQueryBits = 0;
			tools::Bits::bits_chunk_t queryBits			 = 0;

			for (size_t i = 0; i < size; ++i)
			{
				// Compare NoneOf bits
				if (m_noneOfBits.m_bits[i] & _archetypeBits.m_bits[i])
					return false;

				// Compare MustHave bits
				if ((m_mustHaveBits.m_bits[i] & _archetypeBits.m_bits[i]) != m_mustHaveBits.m_bits[i])
					return false;

				// Setup to compare OneOf bits
				archetypeQueryBits	|= (m_oneOfBits.m_bits[i] & _archetypeBits.m_bits[i]);
				queryBits			|= m_oneOfBits.m_bits[i];
			}

			// queryBits | archetypeQueryBits
			// ------
			// 0		   0				  | 1 true 
			// 0		   1				  | x error
			// 1		   0				  | 0 false
			// 1		   1				  | 1 true 
			assert(!(!queryBits && archetypeQueryBits));
			return !queryBits || archetypeQueryBits;
		};

		// Set Query with the types of the arguments passed to a function eg. systems 
		template < tools::traits::has_functor Function >
		constexpr void AddToQueryFromFunction(Function&&) noexcept
		{
			[&] < typename... TupleArgsTypes > ( std::tuple < TupleArgsTypes... >* ) noexcept
			{
				// For each types stored in tuple, deduce what kind it is and add to query
				(
					[&] < typename Type > () noexcept
					{
						// Type&
						if constexpr (std::is_reference_v< Type >)
							m_mustHaveBits.SetBitsFromComponents< Type >();

						// Type*
						else if constexpr (std::is_pointer_v< Type >)
							m_oneOfBits.SetBitsFromComponents< std::remove_pointer_t< Type > >();

						// Type
						else
							static_assert(false, "Unable to determine.");
						
					} .operator() < TupleArgsTypes > ()

				, ...);
			} 
			
			// Pass the tuple of types to lambda ( tuple* )
			( tools::traits::null_tuple_v< tools::traits::fn_traits< Function >::args_types_t > );
		}

		// Set Query with the types of the arguments of a tuple that contains query types
		// eg. std::tuple< musthave<>, ... >
		template < typename... Queries >
		constexpr void AddToQueryFromTuple(std::tuple< Queries... >*) noexcept
		{
			// For each query, add to the respective query if fits
			(
				[&] < 
						template <typename...> typename Query, // templated type (Query structs)
						typename... Components
					>
				( Query< Components... >* ) noexcept
				{
					using inner_query = Query<Components...>;

					if constexpr (std::is_same_v< inner_query, MustHave< Components... > >)
						m_mustHaveBits.SetBitsFromComponents< Components... >();

					else if constexpr (std::is_same_v < inner_query, OneOf< Components... > >)
						m_oneOfBits.SetBitsFromComponents< Components... >();

					else if constexpr (std::is_same_v < inner_query, NoneOf< Components... > >)
						m_noneOfBits.SetBitsFromComponents< Components... >();

					else
						static_assert(false, "Does not fit any of the queries.");
				} 
			
				// Pass the tuple of types to lambda ( eg. tuple< mustHave<pos, rot> > > )
			    ( tools::traits::null_tuple_v< Queries > )

			, ...);
		}

		tools::Bits m_mustHaveBits;
		tools::Bits m_oneOfBits;
		tools::Bits m_noneOfBits;
	};
}
