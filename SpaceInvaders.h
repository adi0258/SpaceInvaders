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
        // --- Assets / rendering (SDL_Image load path; RenderComponent.textureId; optional draw scaling) ---
        inline constexpr const char* SPRITE_SHEET_PATH = "res/alternate_space_invaders_sprite_sheet.png";
        constexpr float TEX_SCALE = 0.25f;
        constexpr int SPRITESHEET_TEXTURE_ID = -1;

        // --- Physics world (pixel-to-Box2D scale; walls in SpaceInvaders ctor; player body in createPlayer) ---
        constexpr float BOX_SCALE = 10.f;

        // --- Player (createPlayer: PositionComponent, RenderComponent, ColliderComponent, HealthComponent, PlayerInputComponent; Box2D kinematic shape) ---
        constexpr float PLAYER_SPRITE_X = 1.f;
        constexpr float PLAYER_SPRITE_Y = 20.f;
        constexpr float PLAYER_SPRITE_W = 13.f;
        constexpr float PLAYER_SPRITE_H = 8.f;
        constexpr float PLAYER_SPRITE_HALF_W = PLAYER_SPRITE_W * 0.5f;
        constexpr float PLAYER_SPRITE_HALF_H = PLAYER_SPRITE_H * 0.5f;
        // Match SpaceInvaders::WIN_W; on-screen + collider size scale with window width.
        constexpr float PLAYER_REF_WIN_W = 800.f;
        constexpr float PLAYER_WIDTH_TO_SCREEN = 0.10f;
        constexpr float PLAYER_DRAW_W = PLAYER_REF_WIN_W * PLAYER_WIDTH_TO_SCREEN;
        constexpr float PLAYER_DRAW_H = PLAYER_DRAW_W * (PLAYER_SPRITE_H / PLAYER_SPRITE_W);
        constexpr float PLAYER_DRAW_HALF_W = PLAYER_DRAW_W * 0.5f;
        constexpr float PLAYER_DRAW_HALF_H = PLAYER_DRAW_H * 0.5f;
        constexpr int PLAYER_INITIAL_HP = 3;
        constexpr float PLAYER_BODY_DENSITY = 1.f;

        // --- Alien (createAlien: VelocityComponent, RenderComponent, ColliderComponent, AlienAIComponent) ---
        constexpr float ALIEN_SPRITE_X = 4.f;
        constexpr float ALIEN_SPRITE_Y = 52.f;
        constexpr float ALIEN_SPRITE_W = 8.f;
        constexpr float ALIEN_SPRITE_H = 8.f;
        constexpr float ALIEN_VELOCITY_X = 1.0f;
        constexpr float ALIEN_VELOCITY_Y = 0.f;
        constexpr float ALIEN_AI_TIME_TO_MOVE = 1.0f;
        constexpr int ALIEN_AI_INITIAL_DIRECTION = 1;

        // --- Bunker (createBunker: RenderComponent, ColliderComponent, HealthComponent) ---
        constexpr float BUNKER_SPRITE_X = 32.f;
        constexpr float BUNKER_SPRITE_Y = 112.f;
        constexpr float BUNKER_SPRITE_W = 16.f;
        constexpr float BUNKER_SPRITE_H = 8.f;
        constexpr int BUNKER_INITIAL_HP = 10;

        // --- Bullet (createBullet: VelocityComponent dy from caller; RenderComponent, ColliderComponent, DamageComponent) ---
        constexpr float BULLET_SPRITE_X = 15.f;
        constexpr float BULLET_SPRITE_Y = 80.f;
        constexpr float BULLET_SPRITE_W = 3.f;
        constexpr float BULLET_SPRITE_H = 8.f;
        constexpr int BULLET_DAMAGE = 1;

        // --- Explosion (createExplosion: RenderComponent, LifetimeComponent) ---
        constexpr float EXPLOSION_SPRITE_X = 33.f;
        constexpr float EXPLOSION_SPRITE_Y = 20.f;
        constexpr float EXPLOSION_SPRITE_W = 13.f;
        constexpr float EXPLOSION_SPRITE_H = 8.f;
        constexpr float EXPLOSION_LIFETIME_SEC = 0.5f;
    }

    using Transform = struct { SDL_FPoint p; float a; };

    //TODO: LOOK IF RANDOM IS NEEDED FOR ALIEN SHOOTING

    using VelocityComponent = struct {
        float dx{ 0 }, dy{ 0 };
    };

    using Drawable = struct { SDL_FRect part; SDL_FPoint size; };
    //TODO: ADD LIVES COMPONENT FOR PLAYER

    using ColliderComponent = struct {
        b2BodyId body{};
    };



    using KeysComponent = struct { SDL_Scancode left, right; };//TODO: LOOK INTO SAVING PREVIOUS STATE

    using IntentComponent = struct { bool left, right, isShooting; };


    using AlienAIComponent = struct {
        float timeToMove{ 1.0f };
        int direction{ 1 };
    };


    Entity createPlayer(b2WorldId world, float x, float y);
    ent_type createAlien(float x, float y);
    ent_type createBullet(float x, float y, float dy);

    class SpaceInvaders
    {
    public:
        SpaceInvaders();
        ~SpaceInvaders(); // not virtual

        void run();
        bool valid() const { return b2World_IsValid(box); }
    private:
        static constexpr int	WIN_W = 800;
        static constexpr int	WIN_H = 600;

        static constexpr int	FPS = 60;
        static constexpr Uint64	GAME_FRAME = 1000/FPS;
        static constexpr float	RAD_TO_DEG = 57.2958f;

        void movement_system();
        void draw_system();
        void box_system();
        void input_system();
        void alien_ai_system();
        void collision_system();
        void lifetime_system();

        SDL_Texture*		tex = nullptr;
        SDL_Renderer*		ren = nullptr;
        SDL_Window*			win = nullptr;
        b2WorldId			box = b2_nullWorldId;
    };
}

template <> struct bagel::Storage<invaders::Transform> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::Transform>; };
template <> struct bagel::Storage<invaders::VelocityComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::VelocityComponent>; };
template <> struct bagel::Storage<invaders::Drawable> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::Drawable>; };
template <> struct bagel::Storage<invaders::ColliderComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::ColliderComponent>; };
template <> struct bagel::Storage<invaders::KeysComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::KeysComponent>; };
template <> struct bagel::Storage<invaders::IntentComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::IntentComponent>; };
template <> struct bagel::Storage<invaders::AlienAIComponent> final : bagel::NoInstance { using type = bagel::SparseStorage<invaders::AlienAIComponent>; };
