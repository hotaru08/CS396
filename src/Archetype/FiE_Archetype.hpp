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
		const std::span < component_info_t >& _componentInfos,
		const tools::Bits& _bits,
		entity::Manager& _entityManager) noexcept
		: m_pCompInfos			{ },
		  m_compSignature		{ _bits },
		  m_toDeleteEntities	{ },
		  m_entityManager		{ _entityManager },
		  m_currPool			{ _componentInfos },
		  m_processesRunning	{ 0 }
	{
		const auto size = _componentInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			m_pCompInfos[i] = _componentInfos[i];
		}
	}

	sharedinfo::entity_index_t Archetype::AddNewEntityData() noexcept
	{
		return m_currPool.Append();
	}

	void Archetype::DestroyEntity(entity::Entity& _entity) noexcept
	{
		assert(_entity.IsAlive());
		m_toDeleteEntities.emplace_back(_entity);

		// Set global entity to be not alive in the container that stores all global entities


		// If all systems has finished updating, update changes to structure / entities
		if (!m_processesRunning)
			UpdateStructuralChanges();
	}

	void Archetype::UpdateStructuralChanges() noexcept
	{
		// Delete entities that are to be deleted
		const auto size = m_toDeleteEntities.size();
		for (unsigned i = 0; i < size; ++i)
		{

		}
	}



	template<tools::traits::is_empty_fn CallbackType>
	inline void Archetype::AccessGuard(CallbackType&& _callbackFunc) noexcept
	{

	}
}
