/******************************************************************************
filename:	FiE_ComponentManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with components.
	This file contains the definition of declarations in .h file.

	Actions includes:
		- Registering of components

******************************************************************************/
namespace FireflyEngine::component
{
	template < typename Component >
	inline void Manager::RegisterComponent() noexcept
	{
		// Reference to component's information
		constexpr auto& uid = info_v< Component >.m_UID;
		if (uid != FireflyEngine::sharedinfo::invalid_info_v)
			return;

		// Set UID of component, adding to unique components count
		uid = m_numComponents++;
	}

	template < typename... Components >
	inline void Manager::RegisterComponents() noexcept
	{
		(RegisterComponent< Components >(), ...);
	}
}
