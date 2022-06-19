/******************************************************************************
filename:	FiE_ComponentManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with components.

******************************************************************************/
namespace FireflyEngine::component
{
	Manager::Manager() noexcept
	{
		// Register the entity as a component, for easier interaction
		// between entity and its information
		RegisterComponents<entity::Entity>();
	}

	template < typename Component >
	inline void Manager::RegisterComponent() noexcept
	{
		// Reference to component's information
		if constexpr (info_v< Component >.m_uid != sharedinfo::invalid_info_v)
			return;

		// Set UID of component, adding to unique components count
		info_v< Component >.m_uid = m_numComponents++;
	}

	template < typename... Components >
	inline void Manager::RegisterComponents() noexcept
	{
		// Register Entity as a component so as to allow for easier access between
		// entity and its info
		(RegisterComponent< Components >(), ...);
	}
}
