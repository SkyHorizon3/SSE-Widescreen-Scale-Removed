#include "Hooks.h"

#define DLLEXPORT __declspec(dllexport)
extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
	{
		SKSE::PluginVersionData v;
		v.PluginName(Plugin::NAME);
		v.AuthorName("SkyHorizon"sv);
		v.PluginVersion(Plugin::VERSION);
		v.UsesAddressLibrary();
		v.UsesUpdatedStructs();
		return v;
	}
();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse, true);

	spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v"s);

#ifndef NDEBUG
	spdlog::set_level(spdlog::level::trace);
	spdlog::flush_on(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
	spdlog::flush_on(spdlog::level::info);
#endif


	const auto version = skse->RuntimeVersion();

	SKSE::log::info("Game version: {}", version);

	if (version < SKSE::RUNTIME_SSE_1_6_1130) {
		const std::string_view message = std::format("RemovedWidescreenScale is not required for version {} and below", version);
		SKSE::stl::report_and_fail(message);
	}

	SKSE::AllocTrampoline(35);
	Hooks::InstallHooks();

	return true;
}