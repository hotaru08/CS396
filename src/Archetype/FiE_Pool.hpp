/******************************************************************************
filename:	FiE_Pool.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype's memory pool storing an array of pointers that points to the
	respective components the archetype refers to.

	Actions includes:
		- Identifies components that might be in this pool
		- Retrieving the component information for specified entity

******************************************************************************/
namespace FireflyEngine::pool
{
	// ------------------------------------------------------------------------
	// Helper Functions
	// ------------------------------------------------------------------------

	// Returns the index of the page that entity will be allocated to
	std::int32_t GetPageOfEntity(
		const sharedinfo::component_size_t _compInfoSize, 
		const sharedinfo::entity_index_t   _entityIndex) noexcept
	{
		return (_entityIndex * _compInfoSize - 1) / sharedinfo::virtual_page_size_v;
	}


	// ------------------------------------------------------------------------
	// Member Functions
	// ------------------------------------------------------------------------

	inline ArchetypePool::ArchetypePool() noexcept
		: m_numEntities { 0 },
		  m_pComponents { }
	{
	}

	inline ArchetypePool::ArchetypePool(std::span < component_infos_t > _componentInfos) noexcept
		: ArchetypePool { }
	{
		Initialize(_componentInfos);
	}

	inline ArchetypePool::~ArchetypePool()
	{
		if (!m_pComponents.front())
			return;

		// Virtually allocated memory, deallocate here
		Clear();
		for (auto* ptr : m_pComponents)
			VirtualFree(ptr, 0, MEM_RELEASE);
	}
	
	inline void ArchetypePool::Initialize(std::span < component_infos_t > _componentInfos) noexcept
	{
		// Retrieve the infos of the components of this archetype
		m_pCompInfos = _componentInfos;

		// For each component in archetype, allocate virtual memory
		const auto size = m_pCompInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			auto* info = m_pCompInfos[i];
			assert(info);
			assert(info->m_size <= sharedinfo::virtual_page_size_v);

			// Get number of pages for virtual memory
			const auto& numPages = GetPageOfEntity(info->m_size, sharedinfo::max_num_entity_per_pool_v) + 1;
			m_pComponents[i] = reinterpret_cast<std::byte*>(
				VirtualAlloc(nullptr, numPages * sharedinfo::virtual_page_size_v, MEM_RESERVE, PAGE_NOACCESS)
			);

			assert(m_pComponents[i]);
		}
	}

	inline std::int32_t ArchetypePool::Append() noexcept
	{
		assert(m_numEntities < sharedinfo::max_num_entity_per_pool_v);

		// For each component, get the page to append new entity
		const auto size = m_pCompInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			auto* info = m_pCompInfos[i];
			assert(info);

			const auto comp = m_pComponents[i];
			const std::int32_t nextPageIndex = GetPageOfEntity(info->m_size, m_numEntities + 1);
			const std::int32_t currPageIndex = GetPageOfEntity(info->m_size, m_numEntities);

			// Not enough space in current page, create new page
			if (currPageIndex != nextPageIndex)
			{
				auto* const newLoc = comp + nextPageIndex * sharedinfo::virtual_page_size_v;
				auto* const allocatedLoc = reinterpret_cast<std::byte*>(
					VirtualAlloc(newLoc, sharedinfo::virtual_page_size_v, MEM_COMMIT, PAGE_READWRITE)
				);
				assert(newLoc == allocatedLoc);
			}

			// Call ctor to initialize new entity
			if (info->m_pConstructor)
				info->m_pConstructor(comp + info->m_size * m_numEntities);
		}

		return m_numEntities++;
	}

	inline void ArchetypePool::Delete(
		const FireflyEngine::sharedinfo::entity_index_t _entityIndex) noexcept
	{
		assert(_entityIndex < m_numEntities);
		assert(_entityIndex >= 0);

		const auto size = m_pCompInfos.size();

		// Removing last entity 
		if (_entityIndex == --m_numEntities)
		{
			for (unsigned i = 0; i < size; ++i)
			{
				auto* info = m_pCompInfos[i];
				assert(info);

				// Get entity memory location to call dtor
				const auto comp = m_pComponents[i];
				if (info->m_pDestructor)
					info->m_pDestructor(comp + _entityIndex * m_numEntities);

				// Free page if removing entity does not use that page anymore
				const auto prevEntryPage = GetPageOfEntity(info->m_size, m_numEntities + 1);
				const auto currEntryPage = GetPageOfEntity(info->m_size, m_numEntities);

				if (currEntryPage != prevEntryPage)
				{
					bool deallocatedLoc = VirtualFree(
						comp + prevEntryPage * sharedinfo::virtual_page_size_v,
						sharedinfo::virtual_page_size_v,
						MEM_DECOMMIT
					);
					assert(deallocatedLoc);
				}
			}
			return;
		}

		// Removing entity in the middle
		for (unsigned i = 0; i < size; ++i)
		{
			auto* info = m_pCompInfos[i];
			assert(info);

			// Get current entity memory location to call dtor
			const auto comp = m_pComponents[i];
			if (info->m_pDestructor)
				info->m_pDestructor(comp + _entityIndex * m_numEntities);

			// Move the last entity to current entity location
			if (info->m_pMoveFunc)
				info->m_pMoveFunc(comp + info->m_size * m_numEntities, comp + info->m_size * _entityIndex);
			
			else
			{
				if (info->m_pDestructor)
					info->m_pDestructor(comp + info->m_size * _entityIndex);

				std::memcpy(comp + m_numEntities * info->m_size, comp + _entityIndex * info->m_size, info->m_size);

				if (info->m_pDestructor)
					info->m_pDestructor(comp + m_numEntities * info->m_size);
			}

			// Free page if removing entity does not use that page anymore
			const auto prevEntryPage = GetPageOfEntity(info->m_size, m_numEntities + 1);
			const auto currEntryPage = GetPageOfEntity(info->m_size, m_numEntities);

			if (currEntryPage != prevEntryPage)
			{
				bool deallocatedLoc = VirtualFree(
					comp + prevEntryPage * sharedinfo::virtual_page_size_v,
					sharedinfo::virtual_page_size_v,
					MEM_DECOMMIT
				);
				assert(deallocatedLoc);
			}
		}
	}

	inline void ArchetypePool::Clear() noexcept
	{
		// Deallocate physical memory allocated to entities
		while (m_numEntities)
			Delete(m_numEntities - 1);
	}

	inline constexpr std::uint32_t
	ArchetypePool::GetSize() const noexcept
	{
		return m_numEntities;
	}

	inline constexpr std::int32_t
	ArchetypePool::FindComponentType(
		const FireflyEngine::sharedinfo::component_uid_t& _uid) const noexcept
	{
		const auto size = m_pCompInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			if (m_pCompInfos[i]->m_uid == _uid)
				return i;
		}

		// Cannot find component, means this archetype does not contain component
		return -1;
	}

	template< typename Component >
		requires std::is_same_v< Component, std::decay_t< Component > >
	inline Component& 
	ArchetypePool::GetComponent(
		const FireflyEngine::sharedinfo::entity_index_t& _entityIndex) const noexcept
	{	
		const auto& info = component::info_v< Component >;
		const auto typeIndex = FindComponentType(info.m_uid);
		assert(typeIndex >= 0);

		return *reinterpret_cast< Component* >
		(
			m_pComponents[typeIndex] + _entityIndex * info.m_size
		);
	}
}
