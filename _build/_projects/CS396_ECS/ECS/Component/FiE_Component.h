/******************************************************************************
filename:	FiE_Component.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:

Description:

	Contains information regarding components and functionalities to interact
	with them.
******************************************************************************/
#pragma once

#include <cstdint>
#include <cstddef>

// std::function?
namespace FireflyEngine::component
{
	// ------------------------------------------------------------------------
	// Holds important information of components to work with when interacting
	// with the different components
	// ------------------------------------------------------------------------
	class info final
	{
		// Aliases of function type declaractions
		using constructor_fn    = void(std::byte* _src) noexcept;
		using destructor_fn     = void(std::byte* _src) noexcept;
		using move_fn		    = void(std::byte* _src, std::byte* _dest) noexcept;

	public:

		// Aliases of function pointers
		using constructor_ptrFn = constructor_fn*;
		using destructor_ptrFn	= destructor_fn*;
		using move_ptrFn		= move_fn*;

		std::uint64_t		m_UID;	//<! Unique ID of component (for tracking)
		std::uint64_t		m_size;	//<! Size of component (

		constructor_ptrFn	m_pConstructor; //<! Pointer to a constructor function
		destructor_ptrFn	m_pDestructor;  //<! Pointer to a destructor function
		move_ptrFn			m_pMoveFunc;	//<! Pointer to a move function
	};

	// ------------------------------------------------------------------------
	// Holds the information of the component
	// ------------------------------------------------------------------------
	namespace
	{
		template < typename T >
		consteval info CreateInfo() noexcept
		{
			return info
			{
				.m_pConstructor = 
			}
		}

		template < typename T >
		constexpr auto& info_v = CreateInfo< T >();

	}






}
