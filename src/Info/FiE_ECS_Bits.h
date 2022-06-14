/******************************************************************************
filename:	FiE_ECS_Bits.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Bits signature that is used to keep track of the components each archetype
	contains. Can be interpreted like a signature bitset.

******************************************************************************/
namespace FireflyEngine::tools
{
	// ------------------------------------------------------------------------
	// Bits signature ( bit chunks )
	// ------------------------------------------------------------------------
	class Bits final
	{
		using bits_chunk_t = std::uint64_t;						//<! How many bits to define a bit chunk
		static constexpr auto num_bits_per_bit_chunk_v = 64;	//<! Number of bits per chunk to check with

	public:

		// Set bit at the specified index
		void setBit(const std::uint32_t _bitIndex) noexcept
		{
			m_bits[_bitIndex / num_bits_per_bit_chunk_v] |= 
				1ULL << (_bitIndex % num_bits_per_bit_chunk_v);
		}

		// Get bit at the specified index
		constexpr bool getBit(const std::uint32_t _bitIndex) const noexcept
		{
			return m_bits[_bitIndex / num_bits_per_bit_chunk_v] &
				1ULL << (_bitIndex % num_bits_per_bit_chunk_v);
		}

		// Overloaded comparision operator for checking by chunks of bits
		constexpr bool operator==(const Bits& _rhs) const noexcept
		{
			const auto size = m_bits.size();
			for (unsigned i = 0; i < size; ++i)
			{
				if (m_bits[i] != _rhs.m_bits[i])
					return false;
			} 

			return true;
		}

		// Set bits according to the components that is defined in archetype
		template < typename... Components >
		void SetBitsFromComponents() noexcept
		{
			((setBit(component::info_v< Components >.m_uid)), ...);
		}

	private:

		std::array
		<
			bits_chunk_t,
			((sharedinfo::max_num_components_types_v - 1) / num_bits_per_bit_chunk_v) + 1
		>
		m_bits; //<! Bits signature that can be used to track archetype's components
	};
}