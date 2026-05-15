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
            (x + gs::PLAYER_DRAW_HALF_W) / gs::BOX_SCALE,
            (y + gs::PLAYER_DRAW_HALF_H) / gs::BOX_SCALE };
        b2BodyId playerBody = b2CreateBody(world, &bodyDef);


        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = gs::PLAYER_BODY_DENSITY;
        shapeDef.enableSensorEvents = true;
        b2Polygon poly = b2MakeBox(
            gs::PLAYER_DRAW_HALF_W / gs::BOX_SCALE,
            gs::PLAYER_DRAW_HALF_H / gs::BOX_SCALE);
        b2ShapeId playerShape = b2CreatePolygonShape(playerBody, &shapeDef, &poly);

        Entity player = Entity::create();
        player.addAll<
        LivesComponent,
            KeysComponent,
            IntentComponent,
            ColliderComponent,
            Drawable,
            VelocityComponent,
            Transform,
            WeaponComponent>(
            LivesComponent{ gs::PLAYER_INITIAL_HP },
            KeysComponent{ SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT },
            IntentComponent{ false, false, false },
            ColliderComponent{ playerBody, playerShape },
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
        shapeDef.enableSensorEvents = true;
        b2Polygon poly = b2MakeBox((gs::ALIEN_SPRITE_W * 2.f) / gs::BOX_SCALE, (gs::ALIEN_SPRITE_H * 2.f) / gs::BOX_SCALE);
        b2ShapeId alienShape = b2CreatePolygonShape(body, &shapeDef, &poly);

        alien.addAll<ColliderComponent, Drawable, VelocityComponent, Transform, AlienAIComponent>(
            ColliderComponent{ body, alienShape },
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
        bodyDef.isBullet = true;
        bodyDef.userData = (void*)(uintptr_t)bullet.entity().id;
        bodyDef.position = { x / gs::BOX_SCALE, y / gs::BOX_SCALE };
        b2BodyId body = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.isSensor = true;
        shapeDef.enableSensorEvents = true;
        b2Polygon poly = b2MakeBox((gs::BULLET_SPRITE_W) / gs::BOX_SCALE, (gs::BULLET_SPRITE_H) / gs::BOX_SCALE);
        b2ShapeId bulletShape = b2CreatePolygonShape(body, &shapeDef, &poly);

        bullet.addAll<ColliderComponent, Drawable, Transform, BulletComponent>(
            ColliderComponent{ body, bulletShape },
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

    Entity createAlienBullet(b2WorldId world, float x, float y, float dy) {
        Entity bullet = Entity::create();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.gravityScale = 0.0f;
        bodyDef.isBullet = true;
        bodyDef.userData = (void*)(uintptr_t)bullet.entity().id;
        bodyDef.position = { x / gs::BOX_SCALE, y / gs::BOX_SCALE };
        b2BodyId body = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.isSensor = true;
        shapeDef.enableSensorEvents = true;
        b2Polygon poly = b2MakeBox((gs::BULLET_SPRITE_W) / gs::BOX_SCALE, (gs::BULLET_SPRITE_H) / gs::BOX_SCALE);
        b2ShapeId alienBulletShape = b2CreatePolygonShape(body, &shapeDef, &poly);

        bullet.addAll<ColliderComponent, Drawable, Transform, AlienBulletComponent>(
            ColliderComponent{ body, alienBulletShape },
            Drawable{
               { gs::BULLET_SPRITE_X, gs::BULLET_SPRITE_Y, gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H },
               { gs::BULLET_SPRITE_W * 2.f, gs::BULLET_SPRITE_H * 2.f }
            },
            Transform{ SDL_FPoint{x, y}, 0.f },
            AlienBulletComponent{}
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

                float f = i.left ? -30.f : i.right ? 30.f : 0.f;

                b2Vec2 pos = b2Body_GetPosition(c.body);

                float minX = 30.f / gs::BOX_SCALE;
                float maxX = (WIN_W - 30.f) / gs::BOX_SCALE;

                if (pos.x <= minX && f < 0.f) f = 0.f;
                if (pos.x >= maxX && f > 0.f) f = 0.f;

                b2Body_SetLinearVelocity(c.body, { f, 0.f });
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

    void SpaceInvaders::alien_shooting_system() {
        static int shootTimer = SDL_rand(181);

        if (shootTimer > 0) {
            shootTimer--;
            return;
        }

        static const Mask mask = MaskBuilder()
            .set<Transform>()
            .set<AlienAIComponent>()
            .build();

        int count = 0;
        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                count++;
            }
        }

        if (count == 0) return;

        int chosenIndex = SDL_rand(count);
        int currentIndex = 0;

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                if (currentIndex == chosenIndex) {
                    const auto& t = e.get<Transform>();
                    createAlienBullet(box, t.p.x, t.p.y + 20.f, 25.f);
                    break;
                }
                currentIndex++;
            }
        }

        shootTimer = SDL_rand(181);
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
        const auto se = b2World_GetSensorEvents(box);
        for (int i = 0; i < se.beginCount; ++i) {
            const b2BodyId sensorBody = b2Shape_GetBody(se.beginEvents[i].sensorShapeId);
            const b2BodyId visitorBody = b2Shape_GetBody(se.beginEvents[i].visitorShapeId);

            if (!b2Body_IsValid(sensorBody) || !b2Body_IsValid(visitorBody)) {
                continue;
            }

            Entity sensorEntity{ ent_type{ static_cast<id_type>(reinterpret_cast<uintptr_t>(b2Body_GetUserData(sensorBody))) } };
            Entity visitorEntity{ ent_type{ static_cast<id_type>(reinterpret_cast<uintptr_t>(b2Body_GetUserData(visitorBody))) } };
            
            if (sensorEntity.has<BulletComponent>() && visitorEntity.has<AlienAIComponent>()) {
                visitorEntity.add<DestructionComponent>(DestructionComponent{ 0, 1, 1});
                b2Shape_EnableSensorEvents(visitorEntity.get<ColliderComponent>().shape, false);
                b2DestroyBody(sensorBody);
                sensorEntity.destroy();
            }
            else if (visitorEntity.has<LivesComponent>() && sensorEntity.has<AlienBulletComponent>()) {
                auto& lives = visitorEntity.get<LivesComponent>();
                visitorEntity.add<DestructionComponent>(DestructionComponent{ 0, 3, 1});
                b2Shape_EnableSensorEvents(visitorEntity.get<ColliderComponent>().shape, false);
                lives.lives--;
                if (lives.lives <= 0) {
                    b2DestroyBody(sensorBody);
                    sensorEntity.destroy();
                    b2DestroyBody(visitorBody);
                    visitorEntity.destroy();
                    return;
                }
            }
        }
    }

    void SpaceInvaders::cleanup_system() {
        // Clear Out-Of-Bounds items
        static const Mask transformMask = MaskBuilder().set<Transform>().build();
        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(transformMask) ) {
                const auto& t = e.get<Transform>();
                if (t.p.y > WIN_H + 100.f || t.p.y < -100.f) {
                    b2DestroyBody(e.get<ColliderComponent>().body);
                    e.destroy();
                }
            }
        }
    }

    void SpaceInvaders::alien_destruction_system() {
        static const Mask mask = MaskBuilder()
            .set<Transform>()
            .set<ColliderComponent>()
            .set<DestructionComponent>()
            .set<AlienAIComponent>()
            .set<Drawable>()
            .build();

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto& c = e.get<ColliderComponent>();
                auto& d = e.get<DestructionComponent>();

                if (d.currentDestructionStage > d.totalDestructionStages) {
                    b2DestroyBody(c.body);
                    e.destroy();
                }
                else {
                    d.framesToNextStage--;
                    if (d.framesToNextStage != 0)
                        continue;
                    d.currentDestructionStage++;
                    d.framesToNextStage = gs::ALIEN_DESTRUCTION_FRAMES_TO_NEXT_STAGE;
                    switch (d.currentDestructionStage) {
                        case 1:
                            e.get<Drawable>().part = { gs::ALIEN_DESTRUCTION_SPRITE_X, gs::ALIEN_DESTRUCTION_SPRITE_Y, gs::ALIEN_DESTRUCTION_SPRITE_W, gs::ALIEN_DESTRUCTION_SPRITE_H };
                            break;
                    }
                }
            }
        }
    }

    void SpaceInvaders::player_destruction_system() {
        static const Mask mask = MaskBuilder()
            .set<Transform>()
            .set<ColliderComponent>()
            .set<DestructionComponent>()
            .set<KeysComponent>()
            .set<Drawable>()
            .build();

        for (Entity e = Entity::first(); !e.eof(); e.next()) {
            if (e.test(mask)) {
                const auto& c = e.get<ColliderComponent>();
                auto& d = e.get<DestructionComponent>();

                if (d.currentDestructionStage > d.totalDestructionStages) {
                    b2DestroyBody(c.body);
                    e.destroy();
                }
                else {
                    d.framesToNextStage--;
                    if (d.framesToNextStage != 0)
                        continue;
                    d.currentDestructionStage++;
                    d.framesToNextStage = gs::PLAYER_DESTRUCTION_FRAMES_TO_NEXT_STAGE;
                    switch (d.currentDestructionStage) {
                        case 1:
                            e.get<Drawable>().part = { gs::PLAYER_DESTRUCTION_1_SPRITE_X, gs::PLAYER_DESTRUCTION_1_SPRITE_Y, gs::PLAYER_DESTRUCTION_1_SPRITE_W, gs::PLAYER_DESTRUCTION_1_SPRITE_H };
                            break;
                        case 2:
                            e.get<Drawable>().part = { gs::PLAYER_DESTRUCTION_2_SPRITE_X, gs::PLAYER_DESTRUCTION_2_SPRITE_Y, gs::PLAYER_DESTRUCTION_2_SPRITE_W, gs::PLAYER_DESTRUCTION_2_SPRITE_H };
                            break;
                        case 3:
                            e.get<Drawable>().part = { gs::PLAYER_DESTRUCTION_3_SPRITE_X, gs::PLAYER_DESTRUCTION_3_SPRITE_Y, gs::PLAYER_DESTRUCTION_3_SPRITE_W, gs::PLAYER_DESTRUCTION_3_SPRITE_H };
                            break;
                    }
                }
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
            alien_shooting_system();
            box_system();
            collision_system();
            alien_destruction_system();
            player_destruction_system();
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

        for (int row = 0; row < gs::ALIEN_ROWS; ++row) {
            for (int col = 0; col < gs::ALIEN_COLS; ++col) {
                createAlien(box,
                    gs::ALIEN_GRID_START_X + static_cast<float>(col) * gs::ALIEN_GRID_SPACING_X,
                    gs::ALIEN_GRID_START_Y + static_cast<float>(row) * gs::ALIEN_GRID_SPACING_Y);
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