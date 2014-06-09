// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	SlateRHIRendererPrivatePCH.h: Pre-compiled header file for the SlateRHIRenderer module.
=============================================================================*/

#pragma once

#include "ISlateRHIRendererModule.h"


/* Private Dependencies
 *****************************************************************************/

#include "Slate.h"
#include "SlateCore.h"
#include "RenderingCommon.h"
#include "Runtime/RHI/Public/RHI.h"
#include "Runtime/Engine/Public/Engine.h"
#include "Runtime/Engine/Public/ScreenRendering.h"
#include "Runtime/Engine/Public/ShaderCompiler.h"
#include "Runtime/Engine/Public/Slate/SlateTextures.h"
#include "RHIStaticStates.h"
#include "ShaderParameterUtils.h"


/* Private Includes
 *****************************************************************************/

#include "SlateRHIFontTexture.h"
#include "SlateRHIResourceManager.h"
#include "SlateRHIRenderingPolicy.h"
#include "SlateRHITextureAtlas.h"
#include "SlateShaders.h"
#include "SlateRHIRenderer.h"
