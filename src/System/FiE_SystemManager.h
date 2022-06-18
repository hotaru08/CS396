/******************************************************************************
filename:	FiE_SystemManager.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with systems, running when every
	update loop to update logic in the systems.

******************************************************************************/
#pragma once

namespace FireflyEngine::system
{
	// ------------------------------------------------------------------------
	// Manager that handles the components
	// ------------------------------------------------------------------------
	struct Manager final
	{
		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------
		
		Manager() noexcept										= default;
		Manager(const Manager& _otherMgr) noexcept				= delete; // Unable to copy, only 1 manager
		Manager& operator=(const Manager& _otherMgr) noexcept	= delete;

		Manager(const Manager&& _otherMgr) noexcept				= delete; // Unable to move, only 1 manager
		Manager& operator=(const Manager&& _otherMgr) noexcept	= delete;


		// ------------------------------------------------------------------------
		// System Functions
		// ------------------------------------------------------------------------

		// Registers systems defined
		template < typename... Components >
		void RegisterSystems() noexcept;


		// ------------------------------------------------------------------------
		// Other Functions
		// ------------------------------------------------------------------------

		// Run all the systems that are registered to system manager
		void RunAllSystems() noexcept { }

	private:

		

		// For internally registering component
		template < typename Component >
		void RegisterSystem() noexcept;
	};
}

#include <System\FiE_SystemManager.hpp>
