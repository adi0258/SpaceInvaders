#include <iostream>
using namespace std;

#include "bagel.h"
#include "SpaceInvaders.h"
using namespace bagel;

bool test_create_entities()
{
	ent_type e1 = World::createEntity();
	if (e1.id != 0) return false;
	ent_type e2 = World::createEntity();
	if (e2.id != 1) return false;
	ent_type e3 = World::createEntity();
	if (e3.id != 2) return false;

	World::deleteEntity(e2);
	World::deleteEntity(e1);
	e2 = World::createEntity();
	if (e2.id != 0) return false;

	return true;
}

bool test_world() {
	if (!test_create_entities())
		return false;
	return true;
}


bool test_dynamic_bag() {
	DynamicBag<int, 5> bag;

	bag.push(10);
	bag.push(20);
	if (bag.size() != 2) return false;

	if (bag[0] != 10 || bag[1] != 20) return false;

	int popped = bag.pop();
	if (popped != 20 || bag.size() != 1) return false;

	return true;
}

struct HealthComponent {
	int hp;
};

template<> DynamicBag<HealthComponent, 100> SparseStorage<HealthComponent>::_comps{};

bool test_sparse_storage() {
	ent_type e = World::createEntity();

	SparseStorage<HealthComponent>::add(e, HealthComponent{100});

	HealthComponent& h = SparseStorage<HealthComponent>::get(e);
	if (h.hp != 100) return false;

	h.hp = 85;
	if (SparseStorage<HealthComponent>::get(e).hp != 85) return false;

	return true;
}

void run_tests()
{
	if (!test_world()) {
		cout << "test_world() failed" << endl;
	} else {
		cout << "test_world() passed" << endl;
	}

	if (!test_dynamic_bag()) {
		cout << "test_dynamic_bag() failed" << endl;
	} else {
		cout << "test_dynamic_bag() passed" << endl;
	}

	if (!test_sparse_storage()) {
		cout << "test_sparse_storage() failed" << endl;
	} else {
		cout << "test_sparse_storage() passed" << endl;
	}
}

namespace {

constexpr int kPreviewWinW = 800;
constexpr int kPreviewWinH = 600;
constexpr Uint64 kMsPerSprite = 3000;

struct SpritePreviewEntry {
	const char* label;
	SDL_Texture* atlas;
	SDL_FRect src;
	float destW;
	float destH;
};

SDL_FPoint hudPreviewDestSize(const SDL_Rect& src, float maxW)
{
	const float w = (float)src.w;
	const float h = (float)src.h;
	if (w <= maxW)
		return { w, h };
	const float s = maxW / w;
	return { w * s, h * s };
}

SDL_FRect rectToFRect(const SDL_Rect& r)
{
	return { (float)r.x, (float)r.y, (float)r.w, (float)r.h };
}

SpritePreviewEntry makeHudPreviewEntry(const char* label, SDL_Texture* atlas, const SDL_Rect& src, float maxHudW)
{
	const SDL_FPoint d = hudPreviewDestSize(src, maxHudW);
	return { label, atlas, rectToFRect(src), d.x, d.y };
}

} // namespace

void run_sprite_atlas_preview_test()
{
	using namespace invaders;
	const float alienDrawW = gs::ALIEN_SPRITE_W * 4.f;
	const float alienDrawH = gs::ALIEN_SPRITE_H * 4.f;
	const float bunkerDrawW = gs::BUNKER_SPRITE_W * 4.f;
	const float bunkerDrawH = gs::BUNKER_SPRITE_H * 4.f;
	const float explosionDrawW = gs::EXPLOSION_SPRITE_W * 4.f;
	const float explosionDrawH = gs::EXPLOSION_SPRITE_H * 4.f;

	SDL_Window* win = nullptr;
	SDL_Renderer* ren = nullptr;
	SDL_Texture* tex = nullptr;
	SDL_Texture* hudTex = nullptr;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		cout << "run_sprite_atlas_preview_test: SDL_Init failed: " << SDL_GetError() << endl;
		return;
	}

	if (!SDL_CreateWindowAndRenderer("Sprite atlas preview", kPreviewWinW, kPreviewWinH, 0, &win, &ren)) {
		cout << "run_sprite_atlas_preview_test: SDL_CreateWindowAndRenderer failed: " << SDL_GetError() << endl;
		SDL_Quit();
		return;
	}

	SDL_Surface* surf = IMG_Load(gs::SPRITE_SHEET_PATH);
	if (surf == nullptr) {
		cout << "run_sprite_atlas_preview_test: IMG_Load failed for " << gs::SPRITE_SHEET_PATH << ": " << SDL_GetError() << endl;
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return;
	}

	tex = SDL_CreateTextureFromSurface(ren, surf);
	SDL_DestroySurface(surf);
	if (tex == nullptr) {
		cout << "run_sprite_atlas_preview_test: SDL_CreateTextureFromSurface failed: " << SDL_GetError() << endl;
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return;
	}

	SDL_Surface* hudSurf = IMG_Load(gs::HUD_SPRITE_SHEET_PATH);
	if (hudSurf == nullptr) {
		cout << "run_sprite_atlas_preview_test: IMG_Load failed for " << gs::HUD_SPRITE_SHEET_PATH << ": " << SDL_GetError() << endl;
		SDL_DestroyTexture(tex);
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return;
	}
	hudTex = SDL_CreateTextureFromSurface(ren, hudSurf);
	SDL_DestroySurface(hudSurf);
	if (hudTex == nullptr) {
		cout << "run_sprite_atlas_preview_test: SDL_CreateTextureFromSurface (HUD) failed: " << SDL_GetError() << endl;
		SDL_DestroyTexture(tex);
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return;
	}

	const float maxHudPreviewW = kPreviewWinW * 0.92f;

	const SpritePreviewEntry entries[] = {
		{ "player", tex,
		  { gs::PLAYER_SPRITE_X, gs::PLAYER_SPRITE_Y, gs::PLAYER_SPRITE_W, gs::PLAYER_SPRITE_H },
		  gs::PLAYER_DRAW_W, gs::PLAYER_DRAW_H },
		{ "player_destruction_1", tex,
		  { gs::PLAYER_DESTRUCTION_1_SPRITE_X, gs::PLAYER_DESTRUCTION_1_SPRITE_Y, gs::PLAYER_DESTRUCTION_1_SPRITE_W, gs::PLAYER_DESTRUCTION_1_SPRITE_H },
		  gs::PLAYER_DRAW_W, gs::PLAYER_DRAW_H },
		{ "player_destruction_2", tex,
		  { gs::PLAYER_DESTRUCTION_2_SPRITE_X, gs::PLAYER_DESTRUCTION_2_SPRITE_Y, gs::PLAYER_DESTRUCTION_2_SPRITE_W, gs::PLAYER_DESTRUCTION_2_SPRITE_H },
		  gs::PLAYER_DRAW_W, gs::PLAYER_DRAW_H },
		{ "player_destruction_3", tex,
		  { gs::PLAYER_DESTRUCTION_3_SPRITE_X, gs::PLAYER_DESTRUCTION_3_SPRITE_Y, gs::PLAYER_DESTRUCTION_3_SPRITE_W, gs::PLAYER_DESTRUCTION_3_SPRITE_H },
		  gs::PLAYER_DRAW_W, gs::PLAYER_DRAW_H },
		{ "alien", tex,
		  { gs::ALIEN_SPRITE_X, gs::ALIEN_SPRITE_Y, gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H },
		  alienDrawW, alienDrawH },
		{ "alien_destruction", tex,
		  { gs::ALIEN_DESTRUCTION_SPRITE_X, gs::ALIEN_DESTRUCTION_SPRITE_Y, gs::ALIEN_DESTRUCTION_SPRITE_W, gs::ALIEN_DESTRUCTION_SPRITE_H },
		  alienDrawW, alienDrawH },
		makeHudPreviewEntry("hud_lives", hudTex, gs::HUD_SRC_LIVES, maxHudPreviewW),
		makeHudPreviewEntry("hud_game_over", hudTex, gs::HUD_SRC_GAME_OVER, maxHudPreviewW),
		makeHudPreviewEntry("hud_start_game", hudTex, gs::HUD_SRC_START_GAME, maxHudPreviewW),
		makeHudPreviewEntry("hud_hearts_3", hudTex, gs::HUD_SRC_HEARTS_3, maxHudPreviewW),
		makeHudPreviewEntry("hud_hearts_2", hudTex, gs::HUD_SRC_HEARTS_2, maxHudPreviewW),
		makeHudPreviewEntry("hud_hearts_1", hudTex, gs::HUD_SRC_HEARTS_1, maxHudPreviewW),
		makeHudPreviewEntry("hud_hearts_0", hudTex, gs::HUD_SRC_HEARTS_0, maxHudPreviewW),
	};

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	const float cx = kPreviewWinW * 0.5f;
	const float cy = kPreviewWinH * 0.5f;
	bool quitPreview = false;

	for (const SpritePreviewEntry& e : entries) {
		if (quitPreview)
			break;
		cout << e.label << endl;

		const Uint64 phaseEnd = SDL_GetTicks() + kMsPerSprite;
		while (SDL_GetTicks() < phaseEnd && !quitPreview) {
			SDL_Event ev;
			while (SDL_PollEvent(&ev)) {
				if (ev.type == SDL_EVENT_QUIT)
					quitPreview = true;
				if (ev.type == SDL_EVENT_KEY_DOWN && ev.key.scancode == SDL_SCANCODE_ESCAPE)
					quitPreview = true;
			}

			SDL_RenderClear(ren);
			const SDL_FRect dest = {
				cx - e.destW * 0.5f,
				cy - e.destH * 0.5f,
				e.destW,
				e.destH };
			SDL_RenderTextureRotated(ren, e.atlas, &e.src, &dest, 0.f, nullptr, SDL_FLIP_NONE);
			SDL_RenderPresent(ren);
			SDL_Delay(16);
		}
	}

	SDL_DestroyTexture(hudTex);
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}