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
	// ------------------------------------------------------------------------
	// Helper Functions
	// ------------------------------------------------------------------------
	const entity::EntityInfo& Manager::GetEntityInfo(const entity::Entity& _entity) const noexcept
	{
		const entity::EntityInfo& entityInfo = m_entityInfos[_entity.m_infoIndex];
		assert(entityInfo.m_validation == _entity.m_validation && "Incorrect generation of entity.");
		return entityInfo;
	}

	//archetype::Archetype& Manager::RetrieveArchetype(const std::span< component_info_t > _componentInfos) noexcept
	//{
	//
	//}

	//template < typename... Components >
	//archetype::Archetype& Manager::RetrieveArchetype() noexcept
	//{
	//	static_assert
	//	(
	//		
	//	);
	//}


	// ------------------------------------------------------------------------
	// Member Exposed Functions
	// ------------------------------------------------------------------------
	Manager::Manager() noexcept
	{
		// Initialize list of 
	}

	template
	<
		typename... Components,
		tools::traits::is_return_void_fn CallbackType
	>
	inline entity::Entity Manager::CreateEntity(CallbackType&& _callbackFunc) noexcept
	{


		return {};
	}



	//template<Tools::is_void_Fn CallBackType>
	//inline Entity EntityManager::CreateEntity(CallBackType&& _function) noexcept
	//{
	//	using func_traits = Tools::Fn_Traits<CallBackType>;

	//	return[&]<typename ... Components>(std::tuple<Components...>*) constexpr noexcept
	//	{
	//		auto& archetype = GetOrCreateArchetype<Components...>();

	//		assert(archetype.m_bits.GetBit(Component::ComponentInfo_v<Components>.m_uid) && ...);
	//		//add entity to its respective archetype
	//		const auto EntityIndexInPool = archetype.AppendEntity();
	//		//add entity to the global pool

	//		const auto entity = AllocNewEntity(EntityIndexInPool, archetype);
	//		archetype.GetComponent<Entity>(EntityIndexInPool) = entity;

	//		if constexpr (false == std::is_same_v<ECS_Utility::EmptyLambda, CallBackType>)
	//			_function(archetype.GetComponent<std::remove_reference_t<Components>>(EntityIndexInPool)...);
	//		return entity;
	//	}(Tools::cast_null_tuple_v<func_traits::Args_Tuple>);
	//}

}
