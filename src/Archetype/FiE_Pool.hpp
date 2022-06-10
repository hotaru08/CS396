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
	constexpr std::int32_t GetPageOfEntity(
		const sharedinfo::component_size_t _compInfoSize, 
		const sharedinfo::entity_index_t   _entityIndex) noexcept
	{
		return (_entityIndex * _compInfoSize - 1) / sharedinfo::virtual_page_size_v;
	}


	// ------------------------------------------------------------------------
	// Member Functions
	// ------------------------------------------------------------------------

	inline ArchetypePool::ArchetypePool() noexcept
		: m_size { 0 }
	{
	}

	inline ArchetypePool::ArchetypePool(component_infos_t _componentInfos) noexcept
		: ArchetypePool { }
	{
		Initialize(_componentInfos);
	}

	inline ArchetypePool::~ArchetypePool()
	{
		Clear();
	}
	
	inline void ArchetypePool::Initialize(component_infos_t _componentInfos) noexcept
	{
		// Retrieve the infos of the components of this archetype
		m_compInfos = _componentInfos;

		// For each component in archetype, allocate virtual memory
		const auto size = m_compInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			assert(m_compInfos[i]);
			assert(m_compInfos[i]->m_size <= sharedinfo::virtual_page_size_v);

			// Get number of pages for virtual memory
			const auto& numPages = GetPageOfEntity(m_compInfos[i]->m_size, sharedinfo::max_num_entity_per_pool_v) + 1;
			m_components[i] = reinterpret_cast<std::byte*>(
				VirtualAlloc(nullptr, numPages * sharedinfo::virtual_page_size_v, MEM_RESERVE, PAGE_NOACCESS)
			);

			assert(m_components[i]);
		}
	}

	inline std::int32_t ArchetypePool::Append() noexcept
	{
		assert(m_size < sharedinfo::max_num_entity_per_pool_v);

		// For each component, get the page to append new entity
		const auto size = m_compInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			auto* info = m_compInfos[i];
			assert(info);

			const auto comp = m_components[i];
			const auto nextPageIndex = GetPageOfEntity(info->m_size, m_size + 1);
			const auto currPageIndex = GetPageOfEntity(info->m_size, m_size);

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
				info->m_pConstructor(comp + info->m_size * m_size);
		}

		return m_size++;
	}

	inline void ArchetypePool::Delete(
		const FireflyEngine::sharedinfo::entity_index_t _entityIndex) noexcept
	{
		assert(_entityIndex < m_size);
		assert(_entityIndex >= 0);
		
		const auto size = m_compInfos.size();

		// Removing last entity 
		if (_entityIndex == --m_size)
		{
			for (unsigned i = 0; i < size; ++i)
			{
				const auto* info = m_compInfos[i];
				assert(info);

				// Get entity memory location to call dtor
				const auto comp = m_components[i];
				if (info->m_pDestructor)
					info->m_pDestructor(comp + info->m_size * m_size);
				
				// Free page if removing entity does not use that page anymore
				const auto lastEntryPage = GetPageOfEntity(m_compInfos[i]->m_size, m_size + 1);
				const auto currEntryPage = GetPageOfEntity(m_compInfos[i]->m_size, m_size);

				if (currEntryPage != lastEntryPage)
				{
					auto* const newLoc = comp + lastEntryPage * sharedinfo::virtual_page_size_v;
					bool allocatedLoc = VirtualFree(newLoc, sharedinfo::virtual_page_size_v, MEM_DECOMMIT);
					assert(allocatedLoc);
				}
			}
		}

		// Removing entity in the middle
		else
		{
			for (unsigned i = 0; i < size; ++i)
			{
				const auto* info = m_compInfos[i];
				assert(info);

				// Get entity memory location to call dtor or move
				const auto comp = m_components[i];
				if (info->m_pDestructor)
					info->m_pDestructor(comp + info->m_size * _entityIndex);

				if (info->m_pMoveFunc)
				{
					info->m_pMoveFunc(comp + info->m_size * _entityIndex, comp + info->m_size * m_size);
					if (info->m_pDestructor)
						info->m_pDestructor(comp + info->m_size * m_size);
				}
				else
				{
					if (info->m_pDestructor)
						info->m_pDestructor(comp + info->m_size * _entityIndex);

					std::memcpy(comp + _entityIndex * info->m_size, comp + m_size * info->m_size, info->m_size);

					if (info->m_pDestructor)
						info->m_pDestructor(comp + info->m_size * m_size);
				}

				// Free page if removing entity does not use that page anymore
				const auto lastEntryPage = GetPageOfEntity(m_compInfos[i]->m_size, m_size + 1);
				const auto currEntryPage = GetPageOfEntity(m_compInfos[i]->m_size, m_size);

				if (currEntryPage != lastEntryPage)
				{
					auto* const newLoc = comp + lastEntryPage * sharedinfo::virtual_page_size_v;
					bool allocatedLoc = VirtualFree(newLoc, sharedinfo::virtual_page_size_v, MEM_DECOMMIT);
					assert(allocatedLoc);
				}
			}
		}
	}

	inline void ArchetypePool::Clear() noexcept
	{
		while (m_size)
			Delete(m_size - 1);
	}

	inline constexpr std::uint32_t
	ArchetypePool::GetSize() const noexcept
	{
		return m_size;
	}

	inline constexpr std::int32_t
	ArchetypePool::FindComponentTypeInPool(
		const FireflyEngine::sharedinfo::component_uid_t& _uid) const noexcept
	{


		return std::int32_t();
	}

	template< typename Component >
	inline Component& 
	ArchetypePool::GetComponent(
		const FireflyEngine::sharedinfo::entity_index_t& _entityIndex) const noexcept
	{
		// TODO: insert return statement here
	}
}
