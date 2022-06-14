/******************************************************************************
filename:	FiE_ComponentManager.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with components.

******************************************************************************/
#pragma once

namespace FireflyEngine::component
{
	// ------------------------------------------------------------------------
	// Manager that handles the components
	// ------------------------------------------------------------------------
	class Manager final
	{
		inline static sharedinfo::component_uid_t m_numComponents;		  //<! How many unqiue components are registered

		// For internally registering component
		template < typename Component >
		void RegisterComponent() noexcept;

	public:

		// Rule of 5 - constructor, copy, move
		Manager()												= default;
		Manager(const Manager& _otherMgr) noexcept				= delete; // Unable to copy, only 1 manager
		Manager& operator=(const Manager& _otherMgr) noexcept	= delete;

		Manager(const Manager&& _otherMgr) noexcept				= delete; // Unable to move, only 1 manager
		Manager& operator=(const Manager&& _otherMgr) noexcept	= delete;


		// Exposed to register all possible components
		template < typename... Components >
		void RegisterComponents() noexcept;
	};
}

#include <Component\FiE_ComponentManager.hpp>
