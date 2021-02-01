﻿#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <World.hpp>
#include "EffekseerRenderer.Renderer.h"
#include "EffekseerGodot.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerGodot
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class Renderer;
using RendererRef = Effekseer::RefPtr<Renderer>;

/**
	@brief	描画クラス
*/
class Renderer : public ::EffekseerRenderer::Renderer
{
protected:
	Renderer()
	{
	}
	virtual ~Renderer()
	{
	}

public:
	/**
		@brief	インスタンスを生成する。
		@param	squareMaxCount	最大描画スプライト数
		@return	インスタンス
	*/
	static RendererRef Create(int32_t squareMaxCount, int32_t drawMaxCount);

	/**
		@brief	状態リセット
	*/
	virtual void ResetState() = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerGodot
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
