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
	constexpr bool operator==(const tools::)
}
