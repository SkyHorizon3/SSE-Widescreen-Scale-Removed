#include <SimpleIni.h>

using Mode = RE::GFxMovieView::ScaleModeType;

struct LoadMovie
{
	static std::unordered_map<std::string_view, Mode> modeMap;

	static bool thunk(RE::BSScaleformManager* a_scaleformManager,
		RE::IMenu* a_menu,
		RE::GPtr<RE::GFxMovieView>& a_viewOut,
		const char* a_fileName,
		RE::GFxMovieView::ScaleModeType a_mode,
		float a_backgroundAlpha)
	{

		SKSE::log::debug("menu: {} - mode: {}", a_fileName, std::to_underlying(a_mode));

		const auto it = modeMap.find(a_fileName);
		if (it != modeMap.end())
		{
			return func(
				a_scaleformManager,
				a_menu,
				a_viewOut,
				a_fileName,
				it->second,
				a_backgroundAlpha);
		}

		return func(
			a_scaleformManager,
			a_menu,
			a_viewOut,
			a_fileName,
			a_mode,
			a_backgroundAlpha);
	}
	static inline REL::Relocation<decltype(thunk)> func;

	static void Install()
	{
		REL::Relocation<std::uintptr_t> target{ REL::ID(82325) };
		stl::hook_function_prologue<LoadMovie, 7>(target.address());

		SKSE::log::info("Hooked BSScaleformManager::LoadMovie");
	}
};

std::unordered_map<std::string_view, Mode> LoadMovie::modeMap = {
		{"LoadWaitSpinner"sv, Mode::kShowAll},
		{"BarterMenu"sv, Mode::kNoBorder},
		{"ContainerMenu"sv, Mode::kNoBorder},
		{"CraftingMenu"sv, Mode::kNoBorder},
		{"DialogueMenu"sv, Mode::kNoBorder},
		{"FavoritesMenu"sv, Mode::kShowAll},
		{"HUDMenu"sv, Mode::kShowAll},
		{"InventoryMenu"sv, Mode::kNoBorder},
		{"LevelUpMenu"sv, Mode::kShowAll},
		{"LoadingMenu"sv, Mode::kShowAll},
		{"LockpickingMenu"sv, Mode::kNoBorder},
		{"MagicMenu"sv, Mode::kNoBorder},
		{"MessageBox"sv, Mode::kNoBorder},
		{"RaceSex_menu"sv, Mode::kNoBorder},
		{"SafeZone"sv, Mode::kShowAll},
		{"SleepWaitMenu"sv, Mode::kShowAll},
		{"Titles"sv, Mode::kShowAll},
		{"TrainingMenu"sv, Mode::kNoBorder},
		{"TutorialMenu"sv, Mode::kShowAll},
		{"TweenMenu"sv, Mode::kShowAll},
		{"Map"sv, Mode::kShowAll},
		{"Quest_Journal"sv, Mode::kShowAll},
		{"StatsMenu"sv, Mode::kNoBorder},
		{"ConstructibleObjectMenu"sv, Mode::kNoBorder},
};

bool LoadINI()
{
	const auto path = std::format("Data/SKSE/Plugins/{}.ini", Plugin::NAME);
	if (!std::filesystem::exists(path))
		return false;

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(path.c_str());

	return ini.GetBoolValue("Debug", "EnableDebugLog");
}

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

	if (LoadINI())
	{
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_on(spdlog::level::trace);
	}
	else
	{
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_on(spdlog::level::info);
	}

	const auto version = skse->RuntimeVersion();
	SKSE::log::info("Game version: {}", version);

	if (version < SKSE::RUNTIME_SSE_1_6_1130)
	{
		const auto message = std::format("WidescreenScaleRemoved is not required for version {} and below", version);
		SKSE::stl::report_and_fail(message);
	}

	SKSE::AllocTrampoline(35);
	LoadMovie::Install();

	return true;
}
