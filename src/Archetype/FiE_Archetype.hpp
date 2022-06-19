/******************************************************************************
filename:	FiE_Archetype.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype defines the components that an entities has. It also provides
	functionality to interact with entities and their components.

******************************************************************************/
namespace FireflyEngine::archetype
{
	Archetype::Archetype(
		const std::span < component_info_t > _componentInfos,
		const tools::Bits& _bits,
		entity::Manager& _entityManager) noexcept
		: m_compSignature		{ _bits },
		  m_toDeleteEntities	{ },
		  m_entityManager		{ _entityManager },
		  m_currPool			{ _componentInfos },
		  m_processesRunning	{ 0 }
	{
	}

	inline sharedinfo::entity_index_t Archetype::AddNewPoolEntity() noexcept
	{
		return m_currPool.Append();
	}

	template < typename Component >
	inline Component& Archetype::GetComponent(const sharedinfo::entity_index_t _entityIndex) const noexcept
	{
		return m_currPool.GetComponent< Component >(_entityIndex);
	}

	void Archetype::DestroyEntity(const entity::Entity& _entity) noexcept
	{
		assert(_entity.IsDead());
		m_toDeleteEntities.emplace_back(_entity);

		// If all systems has finished updating, update changes to structure / entities
		if (!m_processesRunning) UpdateStructuralChanges();
	}

	template < typename Component >
	constexpr bool Archetype::CheckArchetypeSignature() const noexcept
	{
		return m_compSignature.GetBit(component::info_v< Component >.m_uid);
	}

	void Archetype::UpdateStructuralChanges() noexcept
	{
		// Delete entities that are to be deleted
		for (size_t i = 0, size = m_toDeleteEntities.size(); i < size; ++i)
		{
			const entity::Entity& currEntity = m_toDeleteEntities[i];
			const entity::EntityInfo& currEntityInfo = m_entityManager.GetEntityInfo(currEntity);
			assert(currEntityInfo.m_pArchetype == this && "Editing current archetype, UDB.");

			// Delete from pool and handle for any system changes
			m_currPool.Delete(currEntityInfo.m_poolIndex);
			if (currEntityInfo.m_poolIndex == m_currPool.GetSize())
			{
				// Directly removes entity from component pool and global pool
				m_entityManager.UpdateEntityInfoAfterDelete(currEntity);
				continue;
			}

			// Swap with 
			m_entityManager.UpdateEntityInfoAfterDelete(
				currEntity,
				m_currPool.GetComponent<entity::Entity>(currEntityInfo.m_poolIndex)
			);
		}
		m_toDeleteEntities.clear();
	}

	template < typename CallbackType >
		requires
			tools::traits::has_functor< CallbackType > &&
			std::is_same_v < typename tools::traits::fn_traits< CallbackType >::return_type_t, void >
	inline void Archetype::AccessGuard(CallbackType&& _callbackFunc) noexcept
	{
		++m_processesRunning;
		_callbackFunc();

		// If all systems has finished updating, update changes to structure / entities
		if (!--processesRunning) UpdateStructuralChanges();
	}
}
