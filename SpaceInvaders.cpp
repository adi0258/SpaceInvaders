#include "SpaceInvaders.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
using namespace std;

#include "bagel.h"
using namespace bagel;


namespace invaders {
    Entity createPlayer(b2WorldId world, float x, float y) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_kinematicBody;
        bodyDef.position = {
            (x + gs::PLAYER_SPRITE_HALF_W) / gs::BOX_SCALE,
            (y + gs::PLAYER_SPRITE_HALF_H) / gs::BOX_SCALE };
        b2BodyId playerBody = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = gs::PLAYER_BODY_DENSITY;
        b2Polygon poly = b2MakeBox(
            gs::PLAYER_SPRITE_HALF_W / gs::BOX_SCALE,
            gs::PLAYER_SPRITE_HALF_H / gs::BOX_SCALE);
        b2CreatePolygonShape(playerBody, &shapeDef, &poly);
        return Entity::create();
        // Entity::create().addAll<RenderComponent,ColliderComponent,KeysComponent>(
        //     RenderComponent{
        //         { gs::PLAYER_SPRITE_X, gs::PLAYER_SPRITE_Y, gs::PLAYER_SPRITE_W, gs::PLAYER_SPRITE_H },
        //         { x, y, gs::PLAYER_SPRITE_W, gs::PLAYER_SPRITE_H },
        //         gs::SPRITESHEET_TEXTURE_ID },
        //     ColliderComponent{ gs::PLAYER_SPRITE_W, gs::PLAYER_SPRITE_H, playerBody }
        //     KeysComponent{});
    }

    ent_type createAlien(float x, float y) {
        ent_type e = World::createEntity();
        //World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, VelocityComponent{ gs::ALIEN_VELOCITY_X, gs::ALIEN_VELOCITY_Y });
        World::addComponent(e, RenderComponent{
            { gs::ALIEN_SPRITE_X, gs::ALIEN_SPRITE_Y, gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H },
            { x, y, gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H },
            gs::SPRITESHEET_TEXTURE_ID });
        //World::addComponent(e, ColliderComponent{ gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H });
        World::addComponent(e, AlienAIComponent{ gs::ALIEN_AI_TIME_TO_MOVE, gs::ALIEN_AI_INITIAL_DIRECTION });
        return e;
    }

    ent_type createBunker(float x, float y) {
        ent_type e = World::createEntity();
        //World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, RenderComponent{
            { gs::BUNKER_SPRITE_X, gs::BUNKER_SPRITE_Y, gs::BUNKER_SPRITE_W, gs::BUNKER_SPRITE_H },
            { x, y, gs::BUNKER_SPRITE_W, gs::BUNKER_SPRITE_H },
            gs::SPRITESHEET_TEXTURE_ID });
        //World::addComponent(e, ColliderComponent{ gs::BUNKER_SPRITE_W, gs::BUNKER_SPRITE_H });
        //World::addComponent(e, HealthComponent{ gs::BUNKER_INITIAL_HP });
        return e;
    }

    ent_type createBullet(float x, float y, float dy) {
        ent_type e = World::createEntity();
        //World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, VelocityComponent{ 0, dy });
        World::addComponent(e, RenderComponent{
            { gs::BULLET_SPRITE_X, gs::BULLET_SPRITE_Y, gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H },
            { x, y, gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H },
            gs::SPRITESHEET_TEXTURE_ID });
        //World::addComponent(e, ColliderComponent{ gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H });
        //World::addComponent(e, DamageComponent{ gs::BULLET_DAMAGE });
        return e;
    }

    ent_type createExplosion(float x, float y) {
        ent_type e = World::createEntity();
        //World::addComponent(e, PositionComponent{ x, y });
        World::addComponent(e, RenderComponent{
            { gs::EXPLOSION_SPRITE_X, gs::EXPLOSION_SPRITE_Y, gs::EXPLOSION_SPRITE_W, gs::EXPLOSION_SPRITE_H },
            { x, y, gs::EXPLOSION_SPRITE_W, gs::EXPLOSION_SPRITE_H },
            gs::SPRITESHEET_TEXTURE_ID });
        //World::addComponent(e, LifetimeComponent{ gs::EXPLOSION_LIFETIME_SEC });
        return e;
    }


    SpaceInvaders::SpaceInvaders()
    {
        constexpr float WIN_H_MID = WIN_H/2.f;
        constexpr float WIN_W_MID = WIN_W/2.f;

        // initialize SDL for video (drawing to screen)
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            cout << SDL_GetError() << endl;
            return;
        }

        // create game window
        if (!SDL_CreateWindowAndRenderer(
            "Space Invaders!", WIN_W, WIN_H, 0, &win, &ren)) {
            cout << SDL_GetError() << endl;
            return;
            }

        // load spritesheet image
        SDL_Surface *surf = IMG_Load(gs::SPRITE_SHEET_PATH);
        if (surf == nullptr) {
            cout << SDL_GetError() << endl;
            return;
        }

        // create texture from spritesheet and destroy orig image
        tex = SDL_CreateTextureFromSurface(ren, surf);
        if (tex == nullptr) {
            cout << SDL_GetError() << endl;
            return;
        }
        SDL_DestroySurface(surf);

        // seed randomizer and set default clear color
        SDL_srand(time(nullptr));
        SDL_SetRenderDrawColor(ren, 0,0,0,255);

        // create Box2D world
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = {0,0};
        box = b2CreateWorld(&worldDef);
        if (!b2World_IsValid(box)) {
            cout << "Failed creating Box2D world" << endl;
            return;
        }

        // create left & right walls (no entities - they just block movement)
        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;
        wallDef.position = {-1, WIN_H_MID/gs::BOX_SCALE};
        // left wall
        b2ShapeDef wallShapeDef = b2DefaultShapeDef();
        wallShapeDef.density = 1;
        b2Polygon wallPoly = b2MakeBox(1, WIN_H_MID/gs::BOX_SCALE);
        b2BodyId wallBody = b2CreateBody(box, &wallDef);
        b2CreatePolygonShape(wallBody, &wallShapeDef, &wallPoly);
        // right wall
        wallDef.position.x = WIN_W/gs::BOX_SCALE + 1;
        wallBody = b2CreateBody(box, &wallDef);
        b2CreatePolygonShape(wallBody, &wallShapeDef, &wallPoly);

        createPlayer(box,
            WIN_W_MID - gs::PLAYER_SPRITE_HALF_W,
            WIN_H - gs::PLAYER_SPRITE_H);

    }

    SpaceInvaders::~SpaceInvaders()
    {
        if (b2World_IsValid(box))
            b2DestroyWorld(box);
        if (tex != nullptr)
            SDL_DestroyTexture(tex);
        if (ren != nullptr)
            SDL_DestroyRenderer(ren);
        if (win != nullptr)
            SDL_DestroyWindow(win);

        SDL_Quit();
    }

}