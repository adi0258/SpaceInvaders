#pragma once
#include <algorithm>
#include <string>

#include "bagel.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <box2d/box2d.h>

namespace invaders {
    using namespace bagel;

    namespace gs
    {
        inline constexpr const char* SPRITE_SHEET_PATH = "res/alternate_space_invaders_sprite_sheet.png";
        constexpr float TEX_SCALE = 0.25f;
        constexpr int SPRITESHEET_TEXTURE_ID = -1;

        constexpr float BOX_SCALE = 10.f;

        constexpr float PLAYER_SPRITE_X = 1.f;
        constexpr float PLAYER_SPRITE_Y = 20.f;
        constexpr float PLAYER_SPRITE_W = 13.f;
        constexpr float PLAYER_SPRITE_H = 8.f;
        constexpr float PLAYER_SPRITE_HALF_W = PLAYER_SPRITE_W * 0.5f;
        constexpr float PLAYER_SPRITE_HALF_H = PLAYER_SPRITE_H * 0.5f;

        constexpr float PLAYER_REF_WIN_W = 800.f;
        constexpr float PLAYER_WIDTH_TO_SCREEN = 0.10f;
        constexpr float PLAYER_DRAW_W = PLAYER_REF_WIN_W * PLAYER_WIDTH_TO_SCREEN;
        constexpr float PLAYER_DRAW_H = PLAYER_DRAW_W * (PLAYER_SPRITE_H / PLAYER_SPRITE_W);
        constexpr float PLAYER_DRAW_HALF_W = PLAYER_DRAW_W * 0.5f;
        constexpr float PLAYER_DRAW_HALF_H = PLAYER_DRAW_H * 0.5f;
        constexpr int PLAYER_INITIAL_HP = 3;
        constexpr float PLAYER_BODY_DENSITY = 1.f;
        constexpr int PLAYER_INVULNERABLE_FRAMES = 120;

        // Player ship row on alternate_space_invaders_sprite_sheet.png: four 16px-spaced
        // columns at x = 1, 17, 33, 49 (intact at 1,20,13,8 then three follow-on frames).
        constexpr float PLAYER_DESTRUCTION_1_SPRITE_X = 17.f;
        constexpr float PLAYER_DESTRUCTION_1_SPRITE_Y = 22.f;
        constexpr float PLAYER_DESTRUCTION_1_SPRITE_W = 13.f;
        constexpr float PLAYER_DESTRUCTION_1_SPRITE_H = 6.f;

        constexpr float PLAYER_DESTRUCTION_2_SPRITE_X = 33.f;
        constexpr float PLAYER_DESTRUCTION_2_SPRITE_Y = 20.f;
        constexpr float PLAYER_DESTRUCTION_2_SPRITE_W = 13.f;
        constexpr float PLAYER_DESTRUCTION_2_SPRITE_H = 8.f;

        constexpr float PLAYER_DESTRUCTION_3_SPRITE_X = 49.f;
        constexpr float PLAYER_DESTRUCTION_3_SPRITE_Y = 20.f;
        constexpr float PLAYER_DESTRUCTION_3_SPRITE_W = 13.f;
        constexpr float PLAYER_DESTRUCTION_3_SPRITE_H = 8.f;
        constexpr int PLAYER_DESTRUCTION_FRAMES_TO_NEXT_STAGE = 40;

        constexpr float ALIEN_SPRITE_X = 4.f;
        constexpr float ALIEN_SPRITE_Y = 52.f;
        constexpr float ALIEN_SPRITE_W = 8.f;
        constexpr float ALIEN_SPRITE_H = 8.f;

        constexpr float ALIEN_DESTRUCTION_SPRITE_X = 129.f;
        constexpr float ALIEN_DESTRUCTION_SPRITE_Y = 52.f;
        constexpr float ALIEN_DESTRUCTION_SPRITE_W = 14.f;
        constexpr float ALIEN_DESTRUCTION_SPRITE_H = 8.f;
        constexpr int ALIEN_DESTRUCTION_FRAMES_TO_NEXT_STAGE = 15;

        constexpr float ALIEN_VELOCITY_X = 1.0f;
        constexpr float ALIEN_VELOCITY_Y = 0.f;
        constexpr float ALIEN_AI_TIME_TO_MOVE = 1.0f;
        constexpr int ALIEN_AI_INITIAL_DIRECTION = 1;

        constexpr int ALIEN_ROWS = 1;
        constexpr int ALIEN_COLS = 8;
        constexpr float ALIEN_GRID_START_X = 100.f;
        constexpr float ALIEN_GRID_START_Y = 60.f;
        constexpr float ALIEN_GRID_SPACING_X = 60.f;
        constexpr float ALIEN_GRID_SPACING_Y = 50.f;

        constexpr float BUNKER_SPRITE_X = 32.f;
        constexpr float BUNKER_SPRITE_Y = 112.f;
        constexpr float BUNKER_SPRITE_W = 16.f;
        constexpr float BUNKER_SPRITE_H = 8.f;
        constexpr int BUNKER_INITIAL_HP = 10;

        constexpr float BULLET_SPRITE_X = 15.f;
        constexpr float BULLET_SPRITE_Y = 80.f;
        constexpr float BULLET_SPRITE_W = 3.f;
        constexpr float BULLET_SPRITE_H = 8.f;
        constexpr int BULLET_DAMAGE = 1;

        constexpr float EXPLOSION_SPRITE_X = 33.f;
        constexpr float EXPLOSION_SPRITE_Y = 20.f;
        constexpr float EXPLOSION_SPRITE_W = 13.f;
        constexpr float EXPLOSION_SPRITE_H = 8.f;
        constexpr float EXPLOSION_LIFETIME_SEC = 0.5f;

        // res/hud_sprite_sheet.png — 2196x1952; srcrect for SDL_RenderTexture (tight bounds on atlas).
        inline constexpr const char* HUD_SPRITE_SHEET_PATH = "res/hud_sprite_sheet.png";
        inline constexpr int HUD_SHEET_W = 2196;
        inline constexpr int HUD_SHEET_H = 1952;

        inline constexpr SDL_Rect HUD_SRC_LIVES{ 26, 367, 365, 133 };
        inline constexpr SDL_Rect HUD_SRC_GAME_OVER{ 28, 1053, 1132, 129 };
        inline constexpr SDL_Rect HUD_SRC_START_GAME{ 29, 1310, 2011, 108 };
        // Precomposed life rows: N = number of filled hearts (0 = empty outline only).
        // 562×200 band at (7,450); omit top 10% of original height (two 5% crops) from sampling.
        inline constexpr SDL_Rect HUD_SRC_HEARTS_3{ 7, 450 + 200 * 10 / 100, 562, 200 - 200 * 10 / 100 };
        inline constexpr SDL_Rect HUD_SRC_HEARTS_2{ 10, 712, 362, 168 };
        inline constexpr SDL_Rect HUD_SRC_HEARTS_1{ 626, 712, 174, 168 };
        inline constexpr SDL_Rect HUD_SRC_HEARTS_0{ 384, 712, 175, 168 };

        constexpr int GAME_OVER_TTL_FRAMES = 180;
        constexpr float HUD_CORNER_PADDING = 16.f;
        constexpr float HUD_HEARTS_MAX_DRAW_W = 120.f;
        constexpr float HUD_TITLE_MAX_DRAW_W_FRAC = 0.92f;
    }

    using Transform = struct { SDL_FPoint p; float a; };
    using VelocityComponent = struct { float dx{ 0 }, dy{ 0 }; };
    using Drawable = struct { SDL_FRect part; SDL_FPoint size; };
    using ColliderComponent = struct { b2BodyId body{}; b2ShapeId shape{}; };
    using KeysComponent = struct { SDL_Scancode left, right; };
    using IntentComponent = struct { bool left, right, isShooting; };
    using WeaponComponent = struct { int cooldown{ 0 }; };
    using BulletComponent = struct {};
    using AlienBulletComponent = struct {};
    using LivesComponent = struct { int lives{ 3 }; };
    using DestructionComponent = struct { int currentDestructionStage{ 0 }, totalDestructionStages{ 3 }, framesToNextStage{ gs::PLAYER_DESTRUCTION_FRAMES_TO_NEXT_STAGE }; };
    using InvulnerableComponent = struct { int ttl; };
    using GameStateComponent = struct { int state{ 0 }; int ttl{ 0 }; };

    using AlienAIComponent = struct {
        float timeToMove{ 1.0f };
        int direction{ 1 };
    };

    Entity createPlayer(b2WorldId world, float x, float y);
    Entity createAlien(b2WorldId world, float x, float y);
    Entity createBullet(b2WorldId world, float x, float y, float dy);
    Entity createAlienBullet(b2WorldId world, float x, float y, float dy);

    class SpaceInvaders
    {
    public:
        SpaceInvaders();
        ~SpaceInvaders();

        void run();
        bool valid() const { return b2World_IsValid(box); }
    private:
        static constexpr int	WIN_W = 800;
        static constexpr int	WIN_H = 600;

        static constexpr int	FPS = 60;
        static constexpr Uint64	GAME_FRAME = 1000/FPS;
        static constexpr float	RAD_TO_DEG = 57.2958f;

        void setup_entities_for_new_game();

        void movement_system();
        void draw_system();
        void draw_hud_system();
        void box_system();
        void input_system();
        void alien_ai_system();
        void collision_system();
        void shooting_system();
        void alien_shooting_system();
        void cleanup_system();
        void alien_destruction_system();
        void player_destruction_system();
        void invulnerability_system();

        SDL_Texture*		tex = nullptr;
        SDL_Texture*		hud_tex = nullptr;
        SDL_Renderer*		ren = nullptr;
        SDL_Window*			win = nullptr;
        b2WorldId			box = b2_nullWorldId;

        Entity HudEntity = {{0}};
    };
}

template <> struct bagel::Storage<invaders::Transform> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::Transform>; };
template <> struct bagel::Storage<invaders::VelocityComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::VelocityComponent>; };
template <> struct bagel::Storage<invaders::Drawable> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::Drawable>; };
template <> struct bagel::Storage<invaders::ColliderComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::ColliderComponent>; };
template <> struct bagel::Storage<invaders::KeysComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::KeysComponent>; };
template <> struct bagel::Storage<invaders::IntentComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::IntentComponent>; };
template <> struct bagel::Storage<invaders::AlienAIComponent> final : bagel::NoInstance { using type = bagel::SparseStorage<invaders::AlienAIComponent>; };
template <> struct bagel::Storage<invaders::WeaponComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::WeaponComponent>; };
template <> struct bagel::Storage<invaders::BulletComponent> final : bagel::NoInstance { using type = bagel::TaggedStorage<invaders::BulletComponent>; };
template <> struct bagel::Storage<invaders::AlienBulletComponent> final : bagel::NoInstance { using type = bagel::TaggedStorage<invaders::AlienBulletComponent>; };
template <> struct bagel::Storage<invaders::LivesComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::LivesComponent>; };
template <> struct bagel::Storage<invaders::DestructionComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::DestructionComponent>; };
template <> struct bagel::Storage<invaders::InvulnerableComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::InvulnerableComponent>; };
template <> struct bagel::Storage<invaders::GameStateComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::GameStateComponent>; };