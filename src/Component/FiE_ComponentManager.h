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
		// Component Functions
		// ------------------------------------------------------------------------

		// Registers components defined
		template < typename... Components >
		void RegisterComponents() noexcept;

	private:

		//<! How many unqiue components are registered
		inline static sharedinfo::component_uid_t m_numComponents = 0;

		// For internally registering component
		template < typename Component >
		void RegisterComponent() noexcept;
	};
}
