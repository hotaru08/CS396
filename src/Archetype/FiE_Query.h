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
		// Compare between queries ( checking the archetype bits )
		constexpr bool operator==(const tools::Bits& _otherBits) const noexcept
		{
			return true;
		};

		// 
		template < tools::traits::has_functor Function >
		constexpr void AddToQueryFromFunction(Function&& _function) noexcept
		{

		}

		template < typename... Queries >
		constexpr void AddToQueryFromTuple(const std::tuple< Queries ... >& _tuple) noexcept
		{

		}
	};
}
