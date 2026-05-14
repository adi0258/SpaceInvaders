#include "SpaceInvaders.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
using namespace std;

#include "bagel.h"
using namespace bagel;

namespace invaders {

    Entity createPlayer(b2WorldId world, float x, float y) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;

        bodyDef.position = {
            (x + gs::PLAYER_DRAW_HALF_W) / gs::BOX_SCALE,
            (y + gs::PLAYER_DRAW_HALF_H) / gs::BOX_SCALE };
        b2BodyId playerBody = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = gs::PLAYER_BODY_DENSITY;
        b2Polygon poly = b2MakeBox(
            gs::PLAYER_DRAW_HALF_W / gs::BOX_SCALE,
            gs::PLAYER_DRAW_HALF_H / gs::BOX_SCALE);
        b2CreatePolygonShape(playerBody, &shapeDef, &poly);

        Entity player = Entity::create();
        player.addAll<
            KeysComponent,
            IntentComponent,
            ColliderComponent,
            Drawable,
            VelocityComponent,
            Transform,
            WeaponComponent>(
            KeysComponent{ SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT },
            IntentComponent{ false, false, false },
            ColliderComponent{ playerBody },
            Drawable{
               { gs::PLAYER_SPRITE_X, gs::PLAYER_SPRITE_Y, gs::PLAYER_SPRITE_W, gs::PLAYER_SPRITE_H },
               { gs::PLAYER_DRAW_W, gs::PLAYER_DRAW_H } },
            VelocityComponent{},
            Transform{
               SDL_FPoint{ x + gs::PLAYER_DRAW_HALF_W, y + gs::PLAYER_DRAW_HALF_H },
               0.f },
            WeaponComponent{ 0 });
        return player;
    }

    Entity createAlien(b2WorldId world, float x, float y) {
        Entity alien = Entity::create();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_kinematicBody;
        bodyDef.userData = (void*)(uintptr_t)alien.entity().id;
        bodyDef.position = { x / gs::BOX_SCALE, y / gs::BOX_SCALE };
        b2BodyId body = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.isSensor = true;
        b2Polygon poly = b2MakeBox((gs::ALIEN_SPRITE_W * 2.f) / gs::BOX_SCALE, (gs::ALIEN_SPRITE_H * 2.f) / gs::BOX_SCALE);
        b2CreatePolygonShape(body, &shapeDef, &poly);

        alien.addAll<ColliderComponent, Drawable, VelocityComponent, Transform, AlienAIComponent>(
            ColliderComponent{ body },
            Drawable{
               { gs::ALIEN_SPRITE_X, gs::ALIEN_SPRITE_Y, gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H },
               { gs::ALIEN_SPRITE_W * 4.f, gs::ALIEN_SPRITE_H * 4.f }
            },
            VelocityComponent{ gs::ALIEN_VELOCITY_X, gs::ALIEN_VELOCITY_Y },
            Transform{ SDL_FPoint{x, y}, 0.f },
            AlienAIComponent{ gs::ALIEN_AI_TIME_TO_MOVE, gs::ALIEN_AI_INITIAL_DIRECTION }
        );

        b2Body_SetLinearVelocity(body, { gs::ALIEN_VELOCITY_X * 10.f, gs::ALIEN_VELOCITY_Y });
        return alien;
    }

    Entity createBullet(b2WorldId world, float x, float y, float dy) {
        Entity bullet = Entity::create();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.gravityScale = 0.0f;
        bodyDef.isBullet = true; //to avoid skipping the alien
        bodyDef.userData = (void*)(uintptr_t)bullet.entity().id;
        bodyDef.position = { x / gs::BOX_SCALE, y / gs::BOX_SCALE };
        b2BodyId body = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f; //giving density to bullet in order for the engine to not treat it as static (so collision isnt missed)
        b2Polygon poly = b2MakeBox((gs::BULLET_SPRITE_W) / gs::BOX_SCALE, (gs::BULLET_SPRITE_H) / gs::BOX_SCALE);
        b2CreatePolygonShape(body, &shapeDef, &poly);

        bullet.addAll<ColliderComponent, Drawable, Transform, BulletComponent>(
            ColliderComponent{ body },
            Drawable{
               { gs::BULLET_SPRITE_X, gs::BULLET_SPRITE_Y, gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H },
               { gs::BULLET_SPRITE_W * 2.f, gs::BULLET_SPRITE_H * 2.f }
            },
            Transform{ SDL_FPoint{x, y}, 0.f },
            BulletComponent{}
        );

        b2Body_SetLinearVelocity(body, { 0.f, dy });
        return bullet;
    }

    void SpaceInvaders::draw_system()
    {
        static const Mask mask = MaskBuilder()
            .set<Transform>()
            .set<Drawable>()
            .build();

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto& t = e.get<Transform>();
                const auto& d = e.get<Drawable>();

                SDL_FRect dest = {
                   t.p.x - d.size.x / 2,
                   t.p.y - d.size.y / 2,
                   d.size.x, d.size.y };

                SDL_RenderTextureRotated(
                    ren, tex, &d.part, &dest, t.a,
                    nullptr, SDL_FLIP_NONE);
            }
        }
    }

    void SpaceInvaders::input_system()
    {
        static const Mask mask = MaskBuilder()
            .set<KeysComponent>()
            .set<IntentComponent>()
            .build();

        SDL_PumpEvents();
        const bool* keys = SDL_GetKeyboardState(nullptr);

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto& k = e.get<KeysComponent>();
                auto& i = e.get<IntentComponent>();

                i.left = keys[k.left];
                i.right = keys[k.right];
                i.isShooting = keys[SDL_SCANCODE_SPACE];
            }
        }
    }

    void SpaceInvaders::movement_system()
    {
        static const Mask mask = MaskBuilder()
            .set<IntentComponent>()
            .set<ColliderComponent>()
            .build();

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto& i = e.get<IntentComponent>();
                const auto& c = e.get<ColliderComponent>();

                const float f = i.left ? -30 : i.right ? 30 : 0;
                b2Body_SetLinearVelocity(c.body, { f,0 });
            }
        }
    }

    void SpaceInvaders::box_system()
    {
        static constexpr float BOX_STEP = 1.f / FPS;
        static const Mask mask = MaskBuilder()
            .set<Transform>()
            .set<ColliderComponent>()
            .build();

        b2World_Step(box, BOX_STEP, 4);

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto t = b2Body_GetTransform(e.get<ColliderComponent>().body);
                auto& tr = e.get<Transform>();
                tr.p.x = t.p.x * gs::BOX_SCALE;
                tr.p.y = t.p.y * gs::BOX_SCALE;
                tr.a = RAD_TO_DEG * b2Rot_GetAngle(t.q);
            }
        }
    }

    void SpaceInvaders::shooting_system() {
        static const Mask mask = MaskBuilder()
            .set<IntentComponent>()
            .set<Transform>()
            .set<WeaponComponent>()
            .build();

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto& intent = e.get<IntentComponent>();
                const auto& t = e.get<Transform>();
                auto& weapon = e.get<WeaponComponent>();

                if (weapon.cooldown > 0) {
                    weapon.cooldown--;
                }
                else if (intent.isShooting) {
                    createBullet(box, t.p.x, t.p.y - 20.f, -40.f);
                    weapon.cooldown = 20;
                }
            }
        }
    }

    void SpaceInvaders::alien_ai_system() {
        static const Mask mask = MaskBuilder()
            .set<Transform>()
            .set<ColliderComponent>()
            .set<AlienAIComponent>()
            .build();

        bool hitEdge = false;
        int newDirection = 0;

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto& t = e.get<Transform>();
                const auto& ai = e.get<AlienAIComponent>();

                if (t.p.x < 50.f && ai.direction == -1) {
                    hitEdge = true;
                    newDirection = 1;
                    break;
                }
                if (t.p.x > WIN_W - 50.f && ai.direction == 1) {
                    hitEdge = true;
                    newDirection = -1;
                    break;
                }
            }
        }

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                auto& ai = e.get<AlienAIComponent>();
                const auto& c = e.get<ColliderComponent>();

                if (hitEdge) {
                    ai.direction = newDirection;

                    b2Vec2 pos = b2Body_GetPosition(c.body);
                    b2Rot rot = b2Body_GetRotation(c.body);
                    pos.y += 20.f / gs::BOX_SCALE;
                    b2Body_SetTransform(c.body, pos, rot);
                }

                b2Body_SetLinearVelocity(c.body, { ai.direction * 15.f, 0.f });
            }
        }
    }

    void SpaceInvaders::collision_system() {
        static const Mask bulletMask = MaskBuilder().set<Transform>().set<BulletComponent>().build();
        static const Mask alienMask = MaskBuilder().set<Transform>().set<AlienAIComponent>().build();

        for (Entity eBullet = Entity::first(); !eBullet.eof(); eBullet.next()) {
            if (eBullet.test(bulletMask) && !eBullet.has<DeadComponent>()) {
                const auto& tB = eBullet.get<Transform>();

                for (Entity eAlien = Entity::first(); !eAlien.eof(); eAlien.next()) {
                    if (eAlien.test(alienMask) && !eAlien.has<DeadComponent>()) {
                        const auto& tA = eAlien.get<Transform>();

                        float dx = tB.p.x - tA.p.x;
                        float dy = tB.p.y - tA.p.y;
                        if (dx < 0) dx = -dx;
                        if (dy < 0) dy = -dy;

                        if (dx < 19.0f && dy < 24.0f) {
                            eBullet.add(DeadComponent{});
                            eAlien.add(DeadComponent{});

                            break;
                        }
                    }
                }
            }
        }
    }

    void SpaceInvaders::cleanup_system() {
        static const Mask mask = MaskBuilder()
            .set<ColliderComponent>()
            .set<DeadComponent>()
            .build();

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                b2DestroyBody(e.get<ColliderComponent>().body);
                e.destroy();
            }
        }
    }

    void SpaceInvaders::run()
    {
        auto start = SDL_GetTicks();
        bool quit = false;

        while (!quit) {
            input_system();
            movement_system();
            shooting_system();
            alien_ai_system();
            box_system();
            collision_system();
            cleanup_system();

            SDL_RenderClear(ren);
            draw_system();
            SDL_RenderPresent(ren);

            const auto end = SDL_GetTicks();
            if (end - start < GAME_FRAME) {
                SDL_Delay(GAME_FRAME - (end - start));
            }
            start += GAME_FRAME;

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if ((e.type == SDL_EVENT_QUIT) ||
                    ((e.type == SDL_EVENT_KEY_DOWN) && (e.key.scancode == SDL_SCANCODE_ESCAPE))) {
                    quit = true;
                }
            }
        }
    }

    SpaceInvaders::SpaceInvaders()
    {
        constexpr float WIN_H_MID = WIN_H / 2.f;
        constexpr float WIN_W_MID = WIN_W / 2.f;

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            cout << SDL_GetError() << endl;
            return;
        }

        if (!SDL_CreateWindowAndRenderer(
            "Space Invaders!", WIN_W, WIN_H, 0, &win, &ren)) {
            cout << SDL_GetError() << endl;
            return;
        }

        SDL_Surface* surf = IMG_Load(gs::SPRITE_SHEET_PATH);
        if (surf == nullptr) {
            cout << SDL_GetError() << endl;
            return;
        }

        tex = SDL_CreateTextureFromSurface(ren, surf);
        if (tex == nullptr) {
            cout << SDL_GetError() << endl;
            return;
        }
        SDL_DestroySurface(surf);

        SDL_srand(time(nullptr));
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0,0 };
        box = b2CreateWorld(&worldDef);
        if (!b2World_IsValid(box)) {
            cout << "Failed creating Box2D world" << endl;
            return;
        }

        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;
        wallDef.position = { -1, WIN_H_MID / gs::BOX_SCALE };

        b2ShapeDef wallShapeDef = b2DefaultShapeDef();
        wallShapeDef.density = 1;
        b2Polygon wallPoly = b2MakeBox(1, WIN_H_MID / gs::BOX_SCALE);
        b2BodyId wallBody = b2CreateBody(box, &wallDef);
        b2CreatePolygonShape(wallBody, &wallShapeDef, &wallPoly);

        wallDef.position.x = WIN_W / gs::BOX_SCALE + 1;
        wallBody = b2CreateBody(box, &wallDef);
        b2CreatePolygonShape(wallBody, &wallShapeDef, &wallPoly);

        createPlayer(box,
            WIN_W_MID - gs::PLAYER_DRAW_HALF_W,
            WIN_H - gs::PLAYER_DRAW_H);

        const int ALIEN_ROWS = 4;
        const int ALIEN_COLS = 8;
        const float START_X = 100.f;
        const float START_Y = 60.f;
        const float SPACING_X = 60.f;
        const float SPACING_Y = 50.f;

        for (int row = 0; row < ALIEN_ROWS; ++row) {
            for (int col = 0; col < ALIEN_COLS; ++col) {
                createAlien(box, START_X + col * SPACING_X, START_Y + row * SPACING_Y);
            }
        }
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