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

		// For registering all the systems
		template < typename... Systems >
		void RegisterSystems(ECS::Manager& _ecsManager) noexcept;
		

		// ------------------------------------------------------------------------
		// Other Functions
		// ------------------------------------------------------------------------

		// Run all the systems that are registered to system manager
		void RunAllSystems() noexcept;

	private:
		
		std::vector < system::Info > m_systems; //<! Container storing all the systems

		// Internal Register
		template < typename System >
			requires std::derived_from< System, BaseSystem >
		void RegisterSystem(ECS::Manager& _ecsManager) noexcept;
	};
}
