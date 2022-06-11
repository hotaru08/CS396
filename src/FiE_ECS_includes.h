/******************************************************************************
filename:	FiE_ECS_includes.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Common includes utilized among the project.
******************************************************************************/
#pragma once

// ----------------------------------------------------------------------------
// system includes
// ----------------------------------------------------------------------------
#include <Windows.h>

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <memory>
#include <utility>
#include <array>
#include <span>
#include <cassert>
#include <concepts>


// ----------------------------------------------------------------------------
// ECS includes
// ----------------------------------------------------------------------------

/* Shared Info */
#include <Info\FiE_ECS_SharedInfo.h>

/* Components */
#include <Component\FiE_Component.h>
#include <Component\FiE_ComponentManager.h>

/* Memory */
#include <Archetype\FiE_Pool.h>

/* Archetype */
#include <Archetype\FiE_Archetype.h>

/* Systems */
#include <System\FiE_SystemManager.h>

/* Entities */
#include <Entity\FiE_Entity.h>