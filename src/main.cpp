
using Mode = RE::GFxMovieView::ScaleModeType;

struct LoadMovie
{
	static bool thunk(RE::BSScaleformManager* a_scaleformManager,
		RE::IMenu* a_menu,
		RE::GPtr<RE::GFxMovieView>& a_viewOut,
		const char* a_fileName,
		Mode a_mode,
		float a_backgroundAlpha)
	{

		if (a_fileName != nullptr && a_fileName[0] != '\0')
		{
			SKSE::log::info("menu: {} - mode: {}", a_fileName, std::to_underlying(a_mode));
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

/*
		case "LoadWaitSpinner"_h: - checked
		case "FavoritesMenu"_h: - checked
		case "HUDMenu"_h: - checked
		case "LevelUpMenu"_h: - checked
		case "LoadingMenu"_h:  - checked
		case "SafeZone"_h:
		case "SleepWaitMenu"_h: - checked
		case "Titles"_h:
		case "TutorialMenu"_h:  - checked
		case "TweenMenu"_h: - checked
		case "Map"_h:  - checked
		case "Quest_Journal"_h: - checked
			return Mode::kShowAll;
		case "BarterMenu"_h:  - checked
		case "ContainerMenu"_h: - checked
		case "CraftingMenu"_h: - checked
		case "DialogueMenu"_h: - checked
		case "InventoryMenu"_h:  - checked
		case "LockpickingMenu"_h:  - checked
		case "MagicMenu"_h: - checked
		case "MessageBox"_h:  - checked
		case "RaceSex_menu"_h: - checked
		case "TrainingMenu"_h: - checked
		case "StatsMenu"_h: - checked
		case "ConstructibleObjectMenu"_h: // replaces CraftingMenu https://www.nexusmods.com/skyrimspecialedition/mods/81409

*/

void InstallFix()
{
	const bool AE2 = REL::Module::get().version() >= SKSE::RUNTIME_SSE_1_7_99;

	// 1.6.1130, 1.6.1170, 1.6.1179, 1.7.99, 1.7.104
	std::array showAllIDs = {
				  std::make_pair(REL::ID(33989), 0x6D + 0x4), // "LoadWaitSpinner"
				  std::make_pair(REL::ID(34010), 0xBB + 0x4), // "LoadWaitSpinner"
				  std::make_pair(REL::ID(51532), 0x7F + 0x4), // "FavoritesMenu"
				  std::make_pair(REL::ID(51610), (AE2 ? 0xE1 : 0xE2) + 0x4), // "HUDMenu"
				  std::make_pair(REL::ID(51908), 0x38 + 0x4), // "LevelUpMenu"
				  std::make_pair(REL::ID(51920), 0x8A + 0x4), // "LevelUpMenu"
				  std::make_pair(REL::ID(51922), 0x6A + 0x4), // "LoadingMenu"
				  std::make_pair(REL::ID(51943), 0xC0 + 0x4), // "LoadingMenu"
				  std::make_pair(REL::ID(52478), 0x33 + 0x4), // "SafeZone"
				  std::make_pair(REL::ID(52481), 0x7D + 0x4), // "SafeZone"
				  std::make_pair(REL::ID(52484), 0x49 + 0x4), // "SleepWaitMenu"
				  std::make_pair(REL::ID(52499), 0x94 + 0x4), // "SleepWaitMenu"
				  std::make_pair(REL::ID(52649), 0x33 + 0x4), // "Titles"
				  std::make_pair(REL::ID(52656), 0x7D + 0x4), // "Titles"
				  std::make_pair(REL::ID(52687), 0x49 + 0x4), // "TutorialMenu"
				  std::make_pair(REL::ID(52700), 0x9A + 0x4), // "TutorialMenu"
				  std::make_pair(REL::ID(52703), 0x9C + 0x4), // "TweenMenu"
				  std::make_pair(REL::ID(52723), 0xEF + 0x4), // "TweenMenu"
				  std::make_pair(REL::ID(53093), 0x1B4 + 0x4), // "Map"
				  std::make_pair(REL::ID(53313), 0xC3 + 0x4), // "Quest_Journal"
				  std::make_pair(REL::ID(53342), 0x10A + 0x4), // "Quest_Journal"

	};

	std::array noBorderIDs = {
				   std::make_pair(REL::ID(50943), 0x98 + 0x4),  // "BarterMenu"
				   std::make_pair(REL::ID(51015), 0xEB + 0x4),  // "BarterMenu"
				   std::make_pair(REL::ID(51124), 0xBE + 0x4),  // "ContainerMenu"
				   std::make_pair(REL::ID(51198), 0x42 + 0x4),  // "CraftingMenu"
				   std::make_pair(REL::ID(51361), 0x90 + 0x4),  // "CraftingMenu"
				   std::make_pair(REL::ID(51504), 0x6B + 0x4),  // "DialogueMenu"
				   std::make_pair(REL::ID(51520), 0xBC + 0x4),  // "DialogueMenu"
				   std::make_pair(REL::ID(51846), 0x7F + 0x4),  // "InventoryMenu"
				   std::make_pair(REL::ID(51886), 0xD6 + 0x4),  // "InventoryMenu"
				   std::make_pair(REL::ID(51948), (AE2 ? 0x1A2 : 0x185) + 0x1), // "LockpickingMenu"
				   std::make_pair(REL::ID(52026), 0x5E + 0x4),  // "MagicMenu"
				   std::make_pair(REL::ID(52087), 0xAE + 0x4),  // "MagicMenu"
				   std::make_pair(REL::ID(52267), 0x37 + 0x4),  // "MessageBox"
				   std::make_pair(REL::ID(52297), 0x81 + 0x4),  // "MessageBox"
				   std::make_pair(REL::ID(52344), 0x5A + 0x4),  // "RaceSex_menu"
				   std::make_pair(REL::ID(52659), 0x95 + 0x4),  // "TrainingMenu"
				   std::make_pair(REL::ID(52678), 0xE5 + 0x4),  // "TrainingMenu"
				   std::make_pair(REL::ID(52508), (AE2 ? 0x4B6 : 0x446) + 0x4),  // "StatsMenu"

	};

	for (const auto& [id, offset] : showAllIDs)
	{
		REL::Relocation target{ id, offset };
		target.write(static_cast<std::uint8_t>(RE::GFxMovieView::ScaleModeType::kShowAll));
	}

	for (const auto& [id, offset] : noBorderIDs)
	{
		REL::Relocation target{ id, offset };
		target.write(static_cast<std::uint8_t>(RE::GFxMovieView::ScaleModeType::kNoBorder));
	}
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
	spdlog::set_level(spdlog::level::info);
	spdlog::flush_on(spdlog::level::info);

	const auto version = skse->RuntimeVersion();
	SKSE::log::info("Game version: {}", version);

	if (version < SKSE::RUNTIME_SSE_1_6_1130)
	{
		const auto message = std::format("WidescreenScaleRemoved is not required for version {} and below!", version);
		SKSE::stl::report_and_fail(message);
	}

	InstallFix();
	SKSE::AllocTrampoline(35);
	LoadMovie::Install();
	return true;
}
