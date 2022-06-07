/******************************************************************************
filename:	FiE_SystemManager.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with systems.
	Actions includes:
		- Registering of systems

******************************************************************************/
#pragma once

namespace FireflyEngine::system
{
	// ------------------------------------------------------------------------
	// Manager that handles the components
	// ------------------------------------------------------------------------
	class Manager final
	{
		//<! How many unqiue systems are registered
		inline static sharedinfo::system_uid_t m_numSystems; 


		// Rule of 5 - constructor, copy, move
		Manager(const Manager& _otherMgr) noexcept				= delete; // Unable to copy, only 1 manager
		Manager& operator=(const Manager& _otherMgr) noexcept	= delete;

		Manager(const Manager&& _otherMgr) noexcept				= delete; // Unable to move, only 1 manager
		Manager& operator=(const Manager&& _otherMgr) noexcept	= delete;


		// For internally registering a system
		template < typename System >
		void RegisterSystem() noexcept;

	public:

		Manager()												= default;

		// Exposed to register all possible systems
		template < typename... Systems >
		void RegisterSystems() noexcept;
	};
}

#include <System\FiE_SystemManager.hpp>
