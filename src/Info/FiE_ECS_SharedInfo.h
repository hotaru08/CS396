/******************************************************************************
filename:	FiE_ECS_SharedInfo.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Shared information that is used across different files working with the ECS
	system.

******************************************************************************/
#pragma once

namespace FireflyEngine::sharedinfo
{
	// ------------------------------------------------------------------------
	// Identifiers

	using entity_id_t		= std::uint64_t;				  //<! Type of ID of entity 
	using entity_index_t	= std::uint32_t;				  //<! Type of Index of entity
	using entity_valid_t	= entity_index_t;				  //<! Type of Validation of entity
															  
	using component_uid_t	= std::uint32_t;				  //<! Based on max number of components
	using component_size_t	= std::uint32_t;				  //<! For storing component size
															  
	using system_uid_t		= std::uint32_t;				  //<! Based on max number of systems
															  
	using empty_lambda_t	= decltype([](){});				  //<! Type definition of an empty lambda

	// ------------------------------------------------------------------------
	// Expressions

	// Archetype
	constexpr auto virtual_pool_page_size_v			= 4096;	  //<! 4k bytes (4Mb) as per specificed
															 
	// Component
	constexpr auto invalid_info_v = std::numeric_limits< sharedinfo::component_uid_t >::max(); //<! UID used to define a invalid id

	constexpr auto max_num_components_types_v		= 128;	  //<! Max number of components registered
	constexpr auto max_num_components_per_entity_v	= 32;	  //<! Max number of component each entity can have
															  
	// Entity												  
	constexpr auto max_num_entity_per_pool_v		= 1000;	  //<! Max number of entities in each memory Pool
	constexpr auto max_num_entity_total_v			= 10000;  //<! Max number of entities that can be generated at a time

	constexpr auto empty_lambda_v					= [](){}; //<! Type definition of an empty lambda
}
