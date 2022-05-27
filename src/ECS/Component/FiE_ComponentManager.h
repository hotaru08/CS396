/******************************************************************************
filename:	FiE_ComponentManager.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with components.
	Actions includes:
		- Registering of components
******************************************************************************/
#pragma once

namespace FireflyEngine::component
{
	// ------------------------------------------------------------------------
	// Manager that handles the components
	// ------------------------------------------------------------------------
	class ComponentMgr
	{
		sharedinfo::component_uid_t m_uniqueComponents;	//<! How many unqiue components are registered

		// For internally registering component
		template < typename Component >
		void RegisterComponent() noexcept;

	public:

		// Exposed to register all possible components
		template < typename... Components >
		void RegisterComponents() noexcept;
	};
}

#include <Component\FiE_ComponentManager.hpp>
