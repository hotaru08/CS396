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
		const tools::Bits& _bits) noexcept
		: m_pCompInfos			{ },
		  m_compSignature		{ _bits },
		  m_toDeleteEntities	{ }, 
		  m_pool				{ _componentInfos },
		  m_processesRunning	{ 0 }
	{
		const auto size = _componentInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			m_pCompInfos[i] = _componentInfos[i];
		}
	}

	void Archetype::DestroyEntity(entity::Entity& _entity) noexcept
	{
		assert(_entity.IsAlive());
		m_toDeleteEntities.emplace_back(_entity);

		// Set entity in global

		// If all systems has finished updating, update changes to structure / entities
		if (m_processesRunning)
			UpdateStructuralChanges();
	}

	void Archetype::UpdateStructuralChanges() noexcept
	{

	}
}
