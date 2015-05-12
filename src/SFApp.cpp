#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(320.0, 90.0f);
  player->SetPosition(player_pos);

  const int number_of_aliens = 20;
  for(int i=0; i<number_of_aliens; i++) {
    // place an alien at width/number_of_aliens * i
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2((430.0/number_of_aliens) * i, 200.0f);
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }

  const int number_of_alien = 10;
  for(int i=0; i<number_of_alien; i++) {
    // place an alien at width/number_of_aliens * i
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2(570.0, (320.0f/number_of_alien) * i);
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }

  const int number_of_enemy = 1;
  for(int i=0; i<number_of_enemy; i++) {
    auto enemy = make_shared<SFAsset>(SFASSET_ENEMY, sf_window);
    auto pos   = Point2(320, 320);
    enemy->SetPosition(pos);
    enemys.push_back(enemy);
  }


  const int num_of_enemy = 1;
  for(int i=0; i<num_of_enemy; i++) {
    auto enemy = make_shared<SFAsset>(SFASSET_ENEMY, sf_window);
    auto pos   = Point2(260, 380);
    enemy->SetPosition(pos);
    enemys.push_back(enemy);
  }

  const int num_of_boss = 1;
  for(int i=0; i<num_of_boss; i++) {
    auto bos = make_shared<SFAsset>(SFASSET_BOSS, sf_window);
    auto pos   = Point2(canvas_w/4, 400);
    bos->SetPosition(pos);
    boss.push_back(bos);
  }

  auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
  auto pos  = Point2((canvas_w/4), 450);
  coin->SetPosition(pos);
  coins.push_back(coin);

}

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    break;
  case SFEVENT_PLAYER_UP:
    player->GoUp();
    break;
  case SFEVENT_PLAYER_DOWN:
    player->GoDown();
    break;
  case SFEVENT_FIRE:
    fire ++;
    FireProjectile();
    break;
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // if this is an update event, then handle it in SFApp,
    // otherwise punt it to the SFEventDispacher.
    SFEvent sfevent((const SDL_Event) event);
    OnEvent(sfevent);
  }
}
int life = 0;

void SFApp::OnUpdateWorld() {
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }



  // Update enemy positions
  for(auto b : boss) {

  }

  // Detect collisions
  for(auto a : aliens) {
    for(auto a : aliens) {
      if(player->CollidesWith(a)) {
          auto player_pos = Point2(320.0, 90.0f);
          player->SetPosition(player_pos);
      }
    }
  }

  for(auto e : enemys) {
    for(auto e : enemys) {
      if(player->CollidesWith(e)) {
          auto player_pos = Point2(999.0, 999.0f);
          player->SetPosition(player_pos);
      }
    }
  }

  for(auto b : boss) {
    for(auto b : boss) {
      if(player->CollidesWith(b)) {
          auto player_pos = Point2(999.0, 999.0f);
          player->SetPosition(player_pos);
      }
    }
  }

  for(auto p : projectiles) {
    for(auto e : enemys) {
      if(p->CollidesWith(e)) {
              auto pos   = Point2(999, 999);
              e->SetPosition(pos);
      }
    }
  }

  for(auto p : projectiles) {
    for(auto b : boss) {
      if(p->CollidesWith(b)) {
      if(life==100){
              auto pos   = Point2(999, 999);
              b->SetPosition(pos);
       }
      else{
        life++;
       }
      }
    }
  }

  for(auto c : coins) {
    for(auto c : coins) {
      if(player->CollidesWith(c)) {
      is_running = false;
      }
    }
  }

  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp.push_back(a);
    }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);
}

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

  // draw the player
  player->OnRender();

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }

  for(auto e: enemys) {
    if(e->IsAlive()) {e->OnRender();}
  }

  for(auto b: boss) {
    if(b->IsAlive()) {b->OnRender();}
  }

  for(auto c: coins) {
    c->OnRender();
  }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}
