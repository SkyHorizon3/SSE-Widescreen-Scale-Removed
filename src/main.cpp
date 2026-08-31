
#include <SimpleIni.h>

using Mode = RE::GFxMovieView::ScaleModeType;

struct LoadMovie
{
	static std::optional<Mode> getMode(const char* a_fileName)
	{
		switch (string::const_hash(a_fileName))
		{
		case "LoadWaitSpinner"_h:
		case "FavoritesMenu"_h:
		case "HUDMenu"_h:
		case "LevelUpMenu"_h:
		case "LoadingMenu"_h:
		case "SafeZone"_h:
		case "SleepWaitMenu"_h:
		case "Titles"_h:
		case "TutorialMenu"_h:
		case "TweenMenu"_h:
		case "Map"_h:
		case "Quest_Journal"_h:
			return Mode::kShowAll;
		case "BarterMenu"_h:
		case "ContainerMenu"_h:
		case "CraftingMenu"_h:
		case "DialogueMenu"_h:
		case "InventoryMenu"_h:
		case "LockpickingMenu"_h:
		case "MagicMenu"_h:
		case "MessageBox"_h:
		case "RaceSex_menu"_h:
		case "TrainingMenu"_h:
		case "StatsMenu"_h:
		case "ConstructibleObjectMenu"_h: // replaces CraftingMenu https://www.nexusmods.com/skyrimspecialedition/mods/81409
			return Mode::kNoBorder;
		default:
			return std::nullopt;
		}
	}

	static bool thunk(RE::BSScaleformManager* a_scaleformManager,
		RE::IMenu* a_menu,
		RE::GPtr<RE::GFxMovieView>& a_viewOut,
		const char* a_fileName,
		Mode a_mode,
		float a_backgroundAlpha)
	{

		if (a_fileName != nullptr && a_fileName[0] != '\0')
		{
			SKSE::log::debug("menu: {} - mode: {}", a_fileName, std::to_underlying(a_mode));

			const auto mode = getMode(a_fileName);
			if (mode.has_value())
			{
				a_mode = *mode;
			}
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
		const auto message = std::format("{} is not required for versions <={}!", Plugin::NAME, version);
		SKSE::stl::report_and_fail(message);
	}

	SKSE::AllocTrampoline(35);
	LoadMovie::Install();

	return true;
}
