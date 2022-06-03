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
#include <iostream>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <memory>
#include <utility>
#include <array>
#include <span>


// ----------------------------------------------------------------------------
// ECS includes
// ----------------------------------------------------------------------------

/* Shared Info */
#include <Info\FiE_ECS_SharedInfo.h>

/* Components */
#include <Component\FiE_Component.h>
#include <Component\FiE_ComponentManager.h>

/* Memory */
#include <Memory\FiE_Pool.h>

/* Systems */



/* Entities */
#include <Entity\FiE_Entity.h>