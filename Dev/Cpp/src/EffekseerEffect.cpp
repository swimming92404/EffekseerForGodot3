#include <Engine.hpp>
#include <File.hpp>
#include <ResourceLoader.hpp>
#include "EffekseerSystem.h"
#include "EffekseerEffect.h"
#include "Utils/EffekseerGodot.Utils.h"
#include "../Effekseer/Effekseer/IO/Effekseer.EfkEfcFactory.h"

namespace godot {

void EffekseerEffect::_register_methods()
{
	register_method("_init", &EffekseerEffect::_init);
	register_method("import", &EffekseerEffect::import);
	register_method("load", &EffekseerEffect::load);
	register_method("release", &EffekseerEffect::release);
	register_property<EffekseerEffect, PoolByteArray>("data_bytes", 
		&EffekseerEffect::set_data_bytes, &EffekseerEffect::get_data_bytes, {});
	register_property<EffekseerEffect, Dictionary>("subresources", 
		&EffekseerEffect::set_subresources, &EffekseerEffect::get_subresources, {});
	register_property<EffekseerEffect, float>("scale", 
		&EffekseerEffect::set_scale, &EffekseerEffect::get_scale, 1.0f);
}

EffekseerEffect::EffekseerEffect()
{
}

EffekseerEffect::~EffekseerEffect()
{
	release();
}

void EffekseerEffect::_init()
{
}

void EffekseerEffect::import(String path, bool shrink_binary)
{
	godot::Ref<godot::File> file = godot::File::_new();
	if (file->open(path, godot::File::READ) != godot::Error::OK) {
		Godot::print_error(String("Failed open file: ") + path, __FUNCTION__, "", __LINE__);
		return;
	}

	int64_t size = file->get_len();
	PoolByteArray bytes = file->get_buffer(size);
	file->close();

	if (shrink_binary) {
		auto read = bytes.read();
		Effekseer::EfkEfcFile efkefc(read.ptr(), (int32_t)bytes.size());
		if (efkefc.IsValid()) {
			auto binChunk = efkefc.ReadRuntimeData();
			m_data_bytes.resize(binChunk.size);
			auto write = m_data_bytes.write();
			memcpy(write.ptr(), binChunk.data, binChunk.size);
		}
	} else {
		m_data_bytes = bytes;
	}

	auto setting = Effekseer::Setting::Create();
	auto native = Effekseer::Effect::Create(setting, m_data_bytes.read().ptr(), (int32_t)m_data_bytes.size());
	if (native == nullptr) {
		Godot::print_error(String("Failed load effect: ") + get_path(), __FUNCTION__, "", __LINE__);
		return;
	}

	auto nativeptr = native.Get();

	String materialDir = path.substr(0, path.find_last("/") + 1);
	auto loader = godot::ResourceLoader::get_singleton();
	
	auto enumerateResouces = [&](const char16_t* (Effekseer::Effect::*getter)(int) const, int count){
		for (int i = 0; i < count; i++) {
			godot::String path = EffekseerGodot::ToGdString((nativeptr->*getter)(i));
			m_subresources[path] = loader->load(materialDir + path);
		}
	};

	enumerateResouces(&Effekseer::Effect::GetColorImagePath, native->GetColorImageCount());
	enumerateResouces(&Effekseer::Effect::GetNormalImagePath, native->GetNormalImageCount());
	enumerateResouces(&Effekseer::Effect::GetDistortionImagePath, native->GetDistortionImageCount());
	enumerateResouces(&Effekseer::Effect::GetModelPath, native->GetModelCount());
	enumerateResouces(&Effekseer::Effect::GetCurvePath, native->GetCurveCount());
	enumerateResouces(&Effekseer::Effect::GetMaterialPath, native->GetMaterialCount());
	enumerateResouces(&Effekseer::Effect::GetWavePath, native->GetWaveCount());
}

void EffekseerEffect::load()
{
	if (m_native != nullptr) return;

	auto system = EffekseerSystem::get_instance();
	if (system == nullptr) return;
	auto manager = system->get_manager();
	if (manager == nullptr) return;

	String path = get_path();
	String basePath = path.substr(0, path.find_last("/"));
	char16_t materialPath[1024];
	EffekseerGodot::ToEfkString(materialPath, basePath, sizeof(materialPath) / sizeof(materialPath[0]));

	m_native = Effekseer::Effect::Create(manager, 
		m_data_bytes.read().ptr(), (int32_t)m_data_bytes.size(), m_scale, materialPath);
	if (m_native == nullptr) {
		Godot::print_error(String("Failed load effect: ") + get_path(), __FUNCTION__, "", __LINE__);
		return;
	}

	if (!Engine::get_singleton()->is_editor_hint()) {
		// Release data bytes memory
		m_data_bytes = PoolByteArray();
	}
}

void EffekseerEffect::release()
{
	m_native.Reset();
}

}
