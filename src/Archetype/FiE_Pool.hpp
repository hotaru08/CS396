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
	inline ArchetypePool::~ArchetypePool()
	{

	}
	
	inline void ArchetypePool::Initialize() noexcept
	{

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

	inline std::uint32_t ArchetypePool::GetSize() const noexcept
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
