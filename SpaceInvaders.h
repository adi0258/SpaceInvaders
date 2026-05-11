#pragma once
#include <algorithm>
#include "bagel.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace invaders {
    using namespace bagel;


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

    void movement_system();
    void render_system();
    void player_control_system();
    void alien_ai_system();
    void collision_system();
    void lifetime_system();

    ent_type createPlayer(float x, float y);
    ent_type createAlien(float x, float y);
    ent_type createBunker(float x, float y);
    ent_type createBullet(float x, float y, float dy);
    ent_type createExplosion(float x, float y);
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