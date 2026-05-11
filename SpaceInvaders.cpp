#include "SpaceInvaders.h"

namespace invaders {

    void movement_system() {}
    void render_system() {}
    void player_control_system() {}
    void alien_ai_system() {}
    void collision_system() {}
    void lifetime_system() {}

    // יצירת שחקן
    ent_type createPlayer(float x, float y) {
        ent_type e = World::createEntity();
        World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, RenderComponent{ {0,0,32,32}, {x, y, 32, 32}, -1 });
        World::addComponent(e, ColliderComponent{ 32, 32 });
        World::addComponent(e, HealthComponent{ 3 });
        World::addComponent(e, PlayerInputComponent{});
        return e;
    }

    ent_type createAlien(float x, float y) {
        ent_type e = World::createEntity();
        World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, VelocityComponent{ 1.0f, 0 });
        World::addComponent(e, RenderComponent{ {32,0,32,32}, {x, y, 32, 32}, -1 });
        World::addComponent(e, ColliderComponent{ 32, 32 });
        World::addComponent(e, AlienAIComponent{ 1.0f, 1 });
        return e;
    }

    ent_type createBunker(float x, float y) {
        ent_type e = World::createEntity();
        World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, RenderComponent{ {64,0,64,32}, {x, y, 64, 32}, -1 });
        World::addComponent(e, ColliderComponent{ 64, 32 });
        World::addComponent(e, HealthComponent{ 10 });
        return e;
    }

    ent_type createBullet(float x, float y, float dy) {
        ent_type e = World::createEntity();
        World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, VelocityComponent{ 0, dy });
        World::addComponent(e, RenderComponent{ {96,0,8,16}, {x, y, 8, 16}, -1 });
        World::addComponent(e, ColliderComponent{ 8, 16 });
        World::addComponent(e, DamageComponent{ 1 });
        return e;
    }

    ent_type createExplosion(float x, float y) {
        ent_type e = World::createEntity();
        World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, RenderComponent{ {104,0,32,32}, {x, y, 32, 32}, -1 });
        World::addComponent(e, LifetimeComponent{ 0.5f });
        return e;
    }
}