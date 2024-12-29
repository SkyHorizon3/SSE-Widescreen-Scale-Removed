#include "Hooks.h"
#include <frozen/unordered_map.h>
#include <frozen/bits/elsa_std.h>

namespace Hooks
{

	struct LoadMovie
	{
		using Mode = RE::GFxMovieView::ScaleModeType;

		static constexpr frozen::unordered_map<std::string_view, Mode, 24> modeMap = {
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

		static bool thunk(RE::BSScaleformManager* a_scaleformManager,
			RE::IMenu* a_menu,
			RE::GPtr<RE::GFxMovieView>& a_viewOut,
			const char* a_fileName,
			RE::GFxMovieView::ScaleModeType a_mode,
			float a_backgroundAlpha)
		{

			SKSE::log::debug("fileName: {} - mode: {}", a_fileName, std::to_underlying(a_mode));

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


	void InstallHooks()
	{
		LoadMovie::Install();
	}
}

/*
*
*
*
//CreationClubMenu: kShowAll
//Marketplace: kShowAll
//ModManager: kShowAll
//KinectMenu: kNoBorder
LoadWaitSpinner: kShowAll -> kExactFit
BarterMenu: kNoBorder -> kExactFit
//BookMenu: kNoBorder
//Book: kShowAll
//Console: kNoScale
ContainerMenu: kNoBorder -> kExactFit
CraftingMenu: kNoBorder -> kExactFit
//CreditsMenu: kShowAll
//CursorMenu: kShowAll
DialogueMenu: kNoBorder -> kExactFit
//FaderMenu: kExactFit
FavoritesMenu: kShowAll -> kExactFit
//GiftMenu: kNoBorder
HUDMenu: kShowAll -> kExactFit
InventoryMenu: kNoBorder -> kExactFit
LevelUpMenu: kShowAll -> kExactFit
LoadingMenu: kShowAll -> kExactFit
LockpickingMenu: kNoBorder -> kExactFit
MagicMenu: kNoBorder -> kExactFit
//StartMenu: kShowAll
MessageBox: kNoBorder -> kExactFit
RaceSex_menu: kNoBorder -> kExactFit
SafeZone: kShowAll -> kExactFit
SleepWaitMenu: kShowAll -> kExactFit
Titles: kShowAll -> kExactFit
TrainingMenu: kNoBorder -> kExactFit
TutorialMenu: kShowAll -> kExactFit
TweenMenu: kShowAll -> kExactFit
Map: kShowAll -> kExactFit
Quest_Journal: kShowAll -> kExactFit

*/