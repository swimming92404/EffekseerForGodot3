﻿#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <World.hpp>
#include "EffekseerRenderer.Renderer.h"
#include "EffekseerGodot3.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerGodot3
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
		@brief	\~English	Set Godot World instance
				\~Japanese	GodotのWorldインスタンスを指定
	*/
	virtual void SetWorld(godot::World* world) = 0;

	/**
		@brief	状態リセット
	*/
	virtual void ResetState() = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerGodot3
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
