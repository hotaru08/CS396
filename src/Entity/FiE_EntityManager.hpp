/******************************************************************************
filename:	FiE_EntityManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype defines the components that an entities has. It also provides
	functionality to interact with entities and their components.

******************************************************************************/
namespace FireflyEngine::entity
{
	template
	<
		typename... Components,
		tools::traits::is_return_void_fn CallbackType
	>
	inline entity::Entity Manager::CreateEntity(CallbackType&& _callbackFunc) noexcept
	{


		return {};
	}


}
