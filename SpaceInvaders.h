#pragma once
#include <algorithm>
#include "bagel.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <box2d/box2d.h>


namespace invaders {
    using namespace bagel;

    namespace gs
    {
        // --- Assets / rendering (SDL_Image load path; RenderComponent.textureId; optional draw scaling) ---
        constexpr char* SPRITE_SHEET_PATH = "res/pong.png";//TODO ADD SPRITESHEET AND CHANGE PATH
        constexpr float TEX_SCALE = 0.25f;
        constexpr int SPRITESHEET_TEXTURE_ID = -1;

        // --- Physics world (pixel-to-Box2D scale; walls in SpaceInvaders ctor; player body in createPlayer) ---
        constexpr float BOX_SCALE = 10.f;

        // --- Player (createPlayer: PositionComponent, RenderComponent, ColliderComponent, HealthComponent, PlayerInputComponent; Box2D kinematic shape) ---
        constexpr float PLAYER_SPRITE_X = 0;
        constexpr float PLAYER_SPRITE_Y = 0;
        constexpr float PLAYER_SPRITE_W = 32;
        constexpr float PLAYER_SPRITE_H = 32;
        constexpr float PLAYER_SPRITE_HALF_W = PLAYER_SPRITE_W * 0.5f;
        constexpr float PLAYER_SPRITE_HALF_H = PLAYER_SPRITE_H * 0.5f;
        constexpr int PLAYER_INITIAL_HP = 3;
        constexpr float PLAYER_BODY_DENSITY = 1.f;

        // --- Alien (createAlien: VelocityComponent, RenderComponent, ColliderComponent, AlienAIComponent) ---
        constexpr float ALIEN_SPRITE_X = 32;
        constexpr float ALIEN_SPRITE_Y = 0;
        constexpr float ALIEN_SPRITE_W = 32;
        constexpr float ALIEN_SPRITE_H = 32;
        constexpr float ALIEN_VELOCITY_X = 1.0f;
        constexpr float ALIEN_VELOCITY_Y = 0.f;
        constexpr float ALIEN_AI_TIME_TO_MOVE = 1.0f;
        constexpr int ALIEN_AI_INITIAL_DIRECTION = 1;

        // --- Bunker (createBunker: RenderComponent, ColliderComponent, HealthComponent) ---
        constexpr float BUNKER_SPRITE_X = 64;
        constexpr float BUNKER_SPRITE_Y = 0;
        constexpr float BUNKER_SPRITE_W = 64;
        constexpr float BUNKER_SPRITE_H = 32;
        constexpr int BUNKER_INITIAL_HP = 10;

        // --- Bullet (createBullet: VelocityComponent dy from caller; RenderComponent, ColliderComponent, DamageComponent) ---
        constexpr float BULLET_SPRITE_X = 96;
        constexpr float BULLET_SPRITE_Y = 0;
        constexpr float BULLET_SPRITE_W = 8;
        constexpr float BULLET_SPRITE_H = 16;
        constexpr int BULLET_DAMAGE = 1;

        // --- Explosion (createExplosion: RenderComponent, LifetimeComponent) ---
        constexpr float EXPLOSION_SPRITE_X = 104;
        constexpr float EXPLOSION_SPRITE_Y = 0;
        constexpr float EXPLOSION_SPRITE_W = 32;
        constexpr float EXPLOSION_SPRITE_H = 32;
        constexpr float EXPLOSION_LIFETIME_SEC = 0.5f;
    }

    struct PositionComponent {
        float x{ 0 }, y{ 0 };
    };

    struct VelocityComponent {
        float dx{ 0 }, dy{ 0 };
    };

    struct RenderComponent {
        SDL_FRect part{ 0,0,0,0 };
        SDL_FRect dest{ 0,0,0,0 };
        int textureId{ 0 };
    };

    struct ColliderComponent {
        float width{ 0 }, height{ 0 };
        b2BodyId body{};
    };

    struct HealthComponent {
        int hp{ 1 };
    };

    struct PlayerInputComponent {};

    struct AlienAIComponent {
        float timeToMove{ 1.0f };
        int direction{ 1 };
    };

    struct DamageComponent {
        int damage{ 1 };
    };

    struct LifetimeComponent {
        float timeLeft{ 1.0f };
    };



    ent_type createPlayer(b2WorldId world, float x, float y);
    ent_type createAlien(float x, float y);
    ent_type createBunker(float x, float y);
    ent_type createBullet(float x, float y, float dy);
    ent_type createExplosion(float x, float y);

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
        void render_system();
        void player_control_system();
        void alien_ai_system();
        void collision_system();
        void lifetime_system();

        SDL_Texture*		tex = nullptr;
        SDL_Renderer*		ren = nullptr;
        SDL_Window*			win = nullptr;
        b2WorldId			box = b2_nullWorldId;
    };
}

template <> struct bagel::Storage<invaders::PositionComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::PositionComponent>; };
template <> struct bagel::Storage<invaders::VelocityComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::VelocityComponent>; };
template <> struct bagel::Storage<invaders::RenderComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::RenderComponent>; };
template <> struct bagel::Storage<invaders::ColliderComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::ColliderComponent>; };
template <> struct bagel::Storage<invaders::HealthComponent> final : bagel::NoInstance { using type = bagel::SparseStorage<invaders::HealthComponent>; };
template <> struct bagel::Storage<invaders::PlayerInputComponent> final : bagel::NoInstance { using type = bagel::TaggedStorage<invaders::PlayerInputComponent>; };
template <> struct bagel::Storage<invaders::AlienAIComponent> final : bagel::NoInstance { using type = bagel::SparseStorage<invaders::AlienAIComponent>; };
template <> struct bagel::Storage<invaders::DamageComponent> final : bagel::NoInstance { using type = bagel::SparseStorage<invaders::DamageComponent>; };
template <> struct bagel::Storage<invaders::LifetimeComponent> final : bagel::NoInstance { using type = bagel::SparseStorage<invaders::LifetimeComponent>; };