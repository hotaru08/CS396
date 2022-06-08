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

	// Gets the page that has enough free memory for new entity
	constexpr std::int32_t GetCurrPageOfEntity(
		const component::Info& _compInfo, 
		const sharedinfo::entity_index_t _entityIndex) noexcept
	{
		return (_entityIndex * _compInfo.m_size) / sharedinfo::virtual_page_size_v;
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

	}
	
	inline void ArchetypePool::Initialize(component_infos_t _componentInfos) noexcept
	{
		m_compInfos = _componentInfos;

		// For each component in archetype, allocate virtual memory
		const auto& size = m_compInfos.size();
		for (unsigned i = 0; i < size; ++i)
		{
			assert(m_compInfos[i]->m_size <= sharedinfo::virtual_page_size_v);

			const auto& page = GetCurrPageOfEntity(*m_compInfos[i], sharedinfo::max_num_entity_per_pool_v) + 1;
			m_components[i] = reinterpret_cast<std::byte*>(
				VirtualAlloc(m_components[i], page * sharedinfo::virtual_page_size_v, MEM_RESERVE, PAGE_NOACCESS)
			);

			assert(m_components[i]);
		}
	}

	inline void ArchetypePool::Append() noexcept
	{

	}

	inline void ArchetypePool::Delete(const FireflyEngine::sharedinfo::entity_index_t _entityIndex) noexcept
	{

	}

	inline void ArchetypePool::Clear() noexcept
	{

	}

	inline constexpr std::uint32_t
	ArchetypePool::GetSize() const noexcept
	{
		return m_size;
	}

	inline constexpr std::int32_t
	ArchetypePool::FindComponentTypeInPool(const FireflyEngine::sharedinfo::component_uid_t& _uid) const noexcept
	{


		return std::int32_t();
	}

	template< typename Component >
	inline Component& 
	ArchetypePool::GetComponent(const FireflyEngine::sharedinfo::entity_index_t& _entityIndex) const noexcept
	{
		// TODO: insert return statement here
	}
}
