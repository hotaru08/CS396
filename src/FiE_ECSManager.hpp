/******************************************************************************
filename:	FiE_ECSManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that provides the user with the functionalities to use the ECS.

******************************************************************************/
namespace FireflyEngine::ECS
{
	Manager::Manager() noexcept
		: m_entityManager		{ std::make_unique<entity::Manager>()		},
		  m_componentManager	{ std::make_unique<component::Manager>()	},
		  m_systemManager		{ std::make_unique<system::Manager>()		}
	{
	}

	template < typename... Components >
		requires (!std::is_pointer_v< Components > && ...) &&
				 (!std::is_reference_v< Components > && ...)
	entity::Entity Manager::CreateEntity() noexcept
	{
		return m_entityManager->CreateEntity< Components... >();
	}

	template < typename CallbackType>
		requires tools::traits::has_functor< CallbackType > &&
				 std::is_same_v< typename tools::traits::fn_traits< CallbackType >::return_type_t, void >
	entity::Entity Manager::CreateEntity(CallbackType&& _callback) noexcept
	{
		return
		[&] < typename... Components > ( std::tuple< Components... >* ) noexcept -> entity::Entity
		{
			return m_entityManager->CreateEntity< Components... >(_callback);

		} (tools::traits::null_tuple_v< tools::traits::fn_traits< CallbackType >::args_types_t >);
	}

	void Manager::DeleteEntity(entity::Entity& _entity) noexcept
	{
		m_entityManager->DeleteEntity(_entity);
	}

	template < typename CallbackType >
		requires tools::traits::has_functor< CallbackType >&&
				 std::is_same_v< typename tools::traits::fn_traits< CallbackType >::return_type_t, void >
	bool Manager::HasEntity(entity::Entity& _entity, CallbackType&& _callback) noexcept
	{
		m_entityManager->FindEntity(_entity);
	}

	template < typename... Components >
	void Manager::RegisterComponents() noexcept
	{
		m_componentManager->RegisterComponents< Components... >();
	}

	template < typename... Components >
	void Manager::RegisterSystems() noexcept
	{
		m_systemManager->RegisterSystems< Components... >();
	}

	void Manager::Run() noexcept
	{
		m_systemManager->RunAllSystems();
	}
}
