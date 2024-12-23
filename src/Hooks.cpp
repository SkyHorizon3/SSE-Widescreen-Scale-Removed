#include "Hooks.h"
#include <frozen/unordered_map.h>
#include <frozen/bits/elsa_std.h>

namespace Hooks
{

	struct LoadMovie
	{

		static constexpr frozen::unordered_map<std::string_view, RE::GFxMovieView::ScaleModeType, 23> modeMap = {
			{"LoadWaitSpinner"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"BarterMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"ContainerMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"CraftingMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"DialogueMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"FavoritesMenu"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"HUDMenu"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"InventoryMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"LevelUpMenu"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"LoadingMenu"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"LockpickingMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"MagicMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"MessageBox"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"RaceSex_menu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"SafeZone"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"SleepWaitMenu"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"Titles"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"TrainingMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
			{"TutorialMenu"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"TweenMenu"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"Map"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"Quest_Journal"sv, RE::GFxMovieView::ScaleModeType::kShowAll},
			{"StatsMenu"sv, RE::GFxMovieView::ScaleModeType::kNoBorder},
		};

		static bool thunk(RE::BSScaleformManager* a_scaleformManager,
			RE::IMenu* a_menu,
			RE::GPtr<RE::GFxMovieView>& a_viewOut,
			const char* a_fileName,
			RE::GFxMovieView::ScaleModeType a_mode,
			float a_backgroundAlpha)
		{

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