  #include <SDL.h>
  #include <iostream>
  #include <map>

  using namespace std;

  /* Simple description of an SDL object */
  class my_SDL_object
  {
	protected:
     SDL_Surface *image;
     SDL_Texture *texture;
     SDL_Rect    rectangle;
 	public:
     bool permanent;
     bool display;
     bool selected = false;
     int xyw[4];
     int speed;
     int explosion;
     int explosion_delay;
     int state;
     int state_delay;
     int direction;
     int number_display;
    void assign_image(SDL_Renderer *renderer, const char* filename)
    {
      image = SDL_LoadBMP(filename);
      texture = SDL_CreateTextureFromSurface(renderer, image);
      SDL_FreeSurface(image);
    }
    void update(SDL_Renderer *renderer)
    {
      rectangle.x = xyw[0];
      rectangle.y = xyw[1];
      rectangle.w = xyw[2];
      rectangle.h = xyw[3];
      SDL_RenderCopy(renderer, texture, NULL, &rectangle);
    }
  };
  
  
  class game_parameters
  {
    public:
     int background_width_1  = 4150;
     int background_height_1 = 7500;
     int background_width_2  = 1440;
     int background_height_2 = 850;
     int play_again_width = 510;
     int play_again_height = 100;
     int home_width  = 238;
     int home_height = 92;
     long my_score = 0;
     int mode = 0;
     int refresh = 20;
     bool quit = false;
  };
  
  void update_renderer(SDL_Renderer *renderer, std::map<std::string, my_SDL_object> object_list)
  {
  	  SDL_RenderClear(renderer);
  	  object_list["background"].update(renderer);
  	  object_list["spaceship_iason"].update(renderer);
  	  object_list["score"].update(renderer);
  	  for (auto const& object : object_list)
 	  {
    	if ( (object_list[object.first.c_str()].permanent) || (object_list[object.first.c_str()].display) )
   	    {
      	  object_list[object.first.c_str()].update(renderer);
        }
      }
  	  SDL_RenderPresent(renderer);
  }

game_parameters main_game(SDL_Renderer *renderer, game_parameters GP)
{ 
  //game_parameters GP;
  int background_width_1  = 0;
  int background_height_1 = 0;
  int spaceship_width  = 80;
  int spaceship_height = 70;
  int n_missiles = 0, missile_ready = 0;
  int missile_delay = 30;
  int n_enemy = 0, enemy_ready = 0;
  int enemy_delay = 100;
  int n_enemy_diagonal = 0, enemy_ready_diagonal = 0;
  int enemy_delay_diagonal = 150;
  int i;
  int position;
  bool up, left, right, down, space;
  const Uint8 *state;
  int axis; 



  my_SDL_object test_object;
  SDL_Event event;
  std::map<std::string, my_SDL_object> object_list;
  std::string str_missile;
  std::string str_enemy;
  std::string str_enemy_diagonal;
  std::string str_tmp;
  std::string str_explosion;
  std::string str_state; 
  std::string my_str;
  std::string my_digit;
  std::string str_counter;
  std::string my_file;
   
    object_list["background"].permanent = true;
    object_list["background"].display = true;
    object_list["background"].xyw[0] = -1120;
    object_list["background"].xyw[1] = -5867;
    object_list["background"].xyw[2] = GP.background_width_1;
    object_list["background"].xyw[3] = GP.background_height_1;
    object_list["background"].speed = 2;
    object_list["background"].state = 0;	
    object_list["background"].state_delay = 0;	
  	object_list["background"].assign_image(renderer, "images/background_planets_raw.bmp");
  
    object_list["spaceship"].permanent = true;
    object_list["spaceship"].xyw[0] = 720 - spaceship_width / 2;
    object_list["spaceship"].xyw[1] = 450 - spaceship_height / 2;
    object_list["spaceship"].xyw[2] = spaceship_width;
    object_list["spaceship"].xyw[3] = spaceship_height;
    object_list["spaceship"].speed = 8;
    object_list["spaceship"].assign_image(renderer, "images/spaceship_iason.bmp");
  
    object_list["score"].permanent = true;
    object_list["score"].xyw[0] = 1354;
    object_list["score"].xyw[1] = 10;
    object_list["score"].xyw[2] = 76;
    object_list["score"].xyw[3] = 20;
    object_list["score"].assign_image(renderer, "images/score.bmp");
  
   
    while (!GP.quit)
    {
      SDL_Delay(GP.refresh);
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) {GP.quit = true;}
      

      my_str = std::to_string(GP.my_score);
      for(i=my_str.length()-1; i >= 0; i--)
      {
          my_digit = my_str.substr(i,1);
          position = 1418 - 15*( my_str.length()-1 - i );
          str_counter = std::to_string(position);
          str_counter =  "counter" + str_counter;
          my_file = "images/" + my_digit + ".bmp";
	      object_list[str_counter.c_str()].xyw[0] = position;
	      object_list[str_counter.c_str()].permanent = false;
	      object_list[str_counter.c_str()].display = true;
          object_list[str_counter.c_str()].xyw[1] = 35;
          object_list[str_counter.c_str()].xyw[2] = 12;
          object_list[str_counter.c_str()].xyw[3] = 20;
          object_list[str_counter.c_str()].assign_image(renderer, my_file.c_str());    
      }
    
      if (GP.my_score > 50)
      {
        GP.mode = 1;
        return GP;
      }
      
      //background moving -->
      object_list["background"].state_delay++;
      if (object_list["background"].state_delay == 1)
      {
           object_list["background"].xyw[1] = object_list["background"].xyw[1] + object_list["background"].speed; 
		   object_list["background"].state_delay = 0; 
		   if (object_list["background"].xyw[1] > 0)
		   {
	         object_list["background"].xyw[1] = -5867;
		   }
		 
      }
        
      //spaceship moving tech
      state = SDL_GetKeyboardState(NULL);
  
      up    = state[SDL_SCANCODE_UP];
      left  = state[SDL_SCANCODE_LEFT];
      right = state[SDL_SCANCODE_RIGHT];
      down  = state[SDL_SCANCODE_DOWN];
      space = state[SDL_SCANCODE_SPACE];
      
      if (up)		object_list["spaceship"].xyw[1] = object_list["spaceship"].xyw[1] - object_list["spaceship"].speed;
      if (left)	object_list["spaceship"].xyw[0] = object_list["spaceship"].xyw[0] - object_list["spaceship"].speed;
      if (right)	object_list["spaceship"].xyw[0] = object_list["spaceship"].xyw[0] + object_list["spaceship"].speed;
      if (down)	object_list["spaceship"].xyw[1] = object_list["spaceship"].xyw[1] + object_list["spaceship"].speed;
	
      if 	(object_list["spaceship"].xyw[1] < 0)  object_list["spaceship"].xyw[1] = 0;
	  if 	(object_list["spaceship"].xyw[0] < 0)  object_list["spaceship"].xyw[0] = 0;
	  if	(object_list["spaceship"].xyw[0] > 1440 - spaceship_width)  object_list["spaceship"].xyw[0] = 1440 - spaceship_width;
	  if 	(object_list["spaceship"].xyw[1] > 850 - spaceship_height)  object_list["spaceship"].xyw[1] = 850 - spaceship_height;
	
  	  //format for missile
	  missile_ready++;
	  if (space)
      {
        if (missile_ready >= missile_delay)
		{
		  missile_ready = 0;
	      str_missile = std::to_string(n_missiles%100);
  	      str_missile = "missile" + str_missile;
  		  object_list[str_missile.c_str()].permanent = false;
          object_list[str_missile.c_str()].display = true;
          object_list[str_missile.c_str()].xyw[2] = 15;
          object_list[str_missile.c_str()].xyw[3] = 54;
          object_list[str_missile.c_str()].xyw[0] = object_list["spaceship"].xyw[0] + object_list["spaceship"].xyw[2]/2 - object_list[str_missile.c_str()].xyw[2]/2 - 1;
          object_list[str_missile.c_str()].xyw[1] = object_list["spaceship"].xyw[1];
          object_list[str_missile.c_str()].speed  = 15;
          object_list[str_missile.c_str()].assign_image(renderer, "images/missile.bmp");
          n_missiles++;
        }
	  }
	  //format for normal enemy
      enemy_ready++;
      if (enemy_ready >= enemy_delay)
      {
        enemy_ready = 0;
        str_enemy = std::to_string(n_enemy%100);
        str_enemy = "enemy" + str_enemy;
        object_list[str_enemy.c_str()].permanent = false;
        object_list[str_enemy.c_str()].display = true;
        object_list[str_enemy.c_str()].xyw[2] = 51;
        object_list[str_enemy.c_str()].xyw[3] = 60;
        object_list[str_enemy.c_str()].xyw[0] = rand() % (1440 - object_list[str_enemy.c_str()].xyw[2]);
        object_list[str_enemy.c_str()].xyw[1] = 20;
        object_list[str_enemy.c_str()].speed  = 0;
        object_list[str_enemy.c_str()].explosion = 0;
        object_list[str_enemy.c_str()].explosion_delay = 0;
        if (n_enemy < 100)
        {
          object_list[str_enemy.c_str()].assign_image(renderer, "images/enemy.bmp");
        }
        n_enemy++; 
      }
      //format for enemy diagonal
      enemy_ready_diagonal++;
      if (enemy_ready_diagonal >= enemy_delay_diagonal)
      {
        axis = 2*(rand() % 2) - 1; 
        enemy_ready_diagonal = 0;
        str_enemy_diagonal = std::to_string(n_enemy_diagonal%100);
        str_enemy_diagonal = "enemy_diagonal" + str_enemy_diagonal;
        object_list[str_enemy_diagonal.c_str()].permanent = false;
        object_list[str_enemy_diagonal.c_str()].display = true;
        object_list[str_enemy_diagonal.c_str()].xyw[2] = 63;
        object_list[str_enemy_diagonal.c_str()].xyw[3] = 56;
        object_list[str_enemy_diagonal.c_str()].xyw[0] = rand() % (1440 - object_list[str_enemy_diagonal.c_str()].xyw[2]);
        object_list[str_enemy_diagonal.c_str()].xyw[1] = 100;
        object_list[str_enemy_diagonal.c_str()].speed  = 3;
        object_list[str_enemy_diagonal.c_str()].explosion = 0;
        object_list[str_enemy_diagonal.c_str()].explosion_delay = 0;
        object_list[str_enemy_diagonal.c_str()].direction = axis;
        if (n_enemy_diagonal < 100)
        {
          object_list[str_enemy_diagonal.c_str()].assign_image(renderer, "images/enemy_diagonal.bmp");
        }
        n_enemy_diagonal++; 
      }
	         
	         
	
	  // Check every missile and see if it collides with any enemy
	  for (auto const& missiles : object_list)
      {
        str_tmp = "missile";
        if (missiles.first.substr(0,7) == str_tmp)
        {
          if (! object_list[missiles.first].display) {continue;}
          object_list[missiles.first].xyw[1] = object_list[missiles.first].xyw[1] - object_list[missiles.first].speed;
          if (object_list[missiles.first].xyw[1] < 0 - object_list[str_missile.c_str()].xyw[3])
          {
            object_list[missiles.first].display = false;
          }
          for (auto const& enemies : object_list)
          {
            str_tmp = "enemy";
            if (enemies.first.substr(0,5) == str_tmp)
            {
              if (! object_list[enemies.first].display) {continue;}
              if (   (object_list[missiles.first].xyw[0] + object_list[missiles.first].xyw[2] > object_list[enemies.first].xyw[0])
                  && (object_list[missiles.first].xyw[0] < object_list[enemies.first].xyw[0] + object_list[enemies.first].xyw[2])
                  && (object_list[missiles.first].xyw[1] < object_list[enemies.first].xyw[1] + object_list[enemies.first].xyw[3])
                  && (object_list[missiles.first].xyw[1] + object_list[missiles.first].xyw[3] > object_list[enemies.first].xyw[1]) )
              {
                object_list[missiles.first].display = false;
                object_list[enemies.first].explosion = 1;  
                GP.my_score = GP.my_score + 50;
              }
            }
          } // loop on enemy
	      for (auto const& enemies_diagonal : object_list)
          {
            str_tmp = "enemy_diagonal";
            if (enemies_diagonal.first.substr(0,14) == str_tmp)
            {
              if (! object_list[enemies_diagonal.first].display) {continue;}
              if (   (object_list[missiles.first].xyw[0] + object_list[missiles.first].xyw[2] > object_list[enemies_diagonal.first].xyw[0])
                  && (object_list[missiles.first].xyw[0] < object_list[enemies_diagonal.first].xyw[0]+object_list[enemies_diagonal.first].xyw[2])
                  && (object_list[missiles.first].xyw[1] < object_list[enemies_diagonal.first].xyw[1]+object_list[enemies_diagonal.first].xyw[3])
                  && (object_list[missiles.first].xyw[1] + object_list[missiles.first].xyw[3] > object_list[enemies_diagonal.first].xyw[1]) )
              {
                object_list[missiles.first].display = false;
                object_list[enemies_diagonal.first].explosion = 1;  
                GP.my_score = GP.my_score + 50;
              }
            }
          } // loop on enemy diagonal
	    }			
	  } // loop on missiles
	
	  update_renderer(renderer, object_list);
	
	

	  //moving the diagonal enemies
      for (auto const& enemies_diagonal : object_list)
      {
        str_tmp = "enemy_diagonal";
        if (enemies_diagonal.first.substr(0,14) == str_tmp)
	    {
	      if (! object_list[enemies_diagonal.first].display) {continue;}
          object_list[enemies_diagonal.first].xyw[1] = object_list[enemies_diagonal.first].xyw[1] + (object_list[enemies_diagonal.first].speed);
          object_list[enemies_diagonal.first].xyw[0] = object_list[enemies_diagonal.first].xyw[0] + (object_list[enemies_diagonal.first].speed *    object_list[enemies_diagonal.first].direction);
          if (object_list[enemies_diagonal.first].xyw[0] >= (1440 - object_list[str_enemy_diagonal.c_str()].xyw[2]))
          {
            object_list[enemies_diagonal.first].direction = object_list[enemies_diagonal.first].direction * -1;
          }
          if ((object_list[enemies_diagonal.first].xyw[0] <= 0))
          {
            object_list[enemies_diagonal.first].direction = object_list[enemies_diagonal.first].direction * -1;
          }
          if (object_list[enemies_diagonal.first].xyw[1] > 850)
          { 
            object_list[enemies_diagonal.first].display = false;
          } 
        }
      }
	
	  // Check every enemy to see if it's exploding
      for (auto const& enemies : object_list)
      {
        str_tmp = "enemy";
        if (enemies.first.substr(0,5) == str_tmp)
        {
          if (! object_list[enemies.first].display) {continue;}
          if (object_list[enemies.first].explosion > 6)
          {
            object_list[enemies.first].display = false;
            object_list[enemies.first].explosion = 0;
            object_list[enemies.first].explosion_delay = 0;
          }
          if(object_list[enemies.first].explosion > 0)
          {
            object_list[enemies.first].explosion_delay++;
            if (object_list[enemies.first].explosion_delay == 3)
            {
              str_explosion = std::to_string(object_list[enemies.first].explosion);
              str_explosion = "images/explosion" + str_explosion + ".bmp";
              object_list[enemies.first].assign_image(renderer, str_explosion.c_str());
              object_list[enemies.first].explosion++;
              object_list[enemies.first].explosion_delay = 0;
            }
          }
        }
      }
	  update_renderer(renderer, object_list);
    }
    
    return GP;
}

  
  
  
game_parameters game_over(SDL_Renderer *renderer, game_parameters GP)
{
    int i;
    int position;
    bool up, left, right, down, space;
    const Uint8 *state;
  	
  	my_SDL_object test_object;
    SDL_Event event;
    std::map<std::string, my_SDL_object> object_list;
  	std::string my_str;
  	std::string my_digit; 
  	std::string str_counter;
  	std::string my_file;
  	
    while (!GP.quit)
    {   
      SDL_Delay(GP.refresh);
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) {GP.quit = true; return GP;}
      
      state = SDL_GetKeyboardState(NULL);
      
      left  = state[SDL_SCANCODE_LEFT];
      right = state[SDL_SCANCODE_RIGHT];
      space = state[SDL_SCANCODE_SPACE];
      
      if (left)		object_list["play_again"].selected = true, object_list["home"].selected = false;
      if (right)	object_list["play_again"].selected = false, object_list["home"].selected = true;
          
          
      object_list["background"].permanent = true;
      object_list["background"].display = true;    
      object_list["background"].xyw[0] = 0;
      object_list["background"].xyw[1] = 0;
      object_list["background"].xyw[2] = GP.background_width_2;
      object_list["background"].xyw[3] = GP.background_height_2;
      object_list["background"].assign_image(renderer, "images/space_gameover.bmp");
      
      object_list["play_again"].xyw[0] = GP.background_width_2/2 - GP.play_again_width + 100;
      object_list["play_again"].xyw[1] = 570;
      object_list["play_again"].xyw[2] = GP.play_again_width;
      object_list["play_again"].xyw[3] = GP.play_again_height;	
      object_list["play_again"].display = true;	
      
      if (object_list["play_again"].selected == true)
      {
        object_list["play_again"].assign_image(renderer, "images/play_again_select.bmp");
        if (space)
        {
          GP.mode = 0;
          GP.my_score = 0;
          return GP;
        }
      }
      else 
      {
      	object_list["play_again"].assign_image(renderer, "images/play_again.bmp");
      }
            
      object_list["home"].xyw[0] = GP.background_width_2/2 + 134;
      object_list["home"].xyw[1] = 570;
      object_list["home"].xyw[2] = GP.home_width;
      object_list["home"].xyw[3] = GP.home_height;	
      object_list["home"].display = true;	
      
      if (object_list["home"].selected == true)
      {
        object_list["home"].assign_image(renderer, "images/home_select.bmp");
        if (space)
        {
          GP.mode = 2;
          GP.my_score = 0;
          return GP;
        }
      }
      else 
      {
      	object_list["home"].assign_image(renderer, "images/home.bmp");
      }
      
      object_list["score"].permanent = true;
      object_list["score"].xyw[0] = 545;
      object_list["score"].xyw[1] = 350;
      object_list["score"].xyw[2] = 350;
      object_list["score"].xyw[3] = 92;
      object_list["score"].assign_image(renderer, "images/score.bmp");
      

      my_str = std::to_string(GP.my_score);
      for(i=my_str.length()-1; i >= 0; i--)
      {
          my_digit = my_str.substr(i,1);
          position = ((720 + ((my_str.length() - 1) * 65 - 10)/2) )- 65*( my_str.length()-1 - i );
          str_counter = std::to_string(position);
          str_counter =  "counter" + str_counter;
          my_file = "images/" + my_digit + ".bmp";
	      object_list[str_counter.c_str()].xyw[0] = position;
	      object_list[str_counter.c_str()].permanent = false;
	      object_list[str_counter.c_str()].display = true;
          object_list[str_counter.c_str()].xyw[1] = 460;
          object_list[str_counter.c_str()].xyw[2] = 55;
          object_list[str_counter.c_str()].xyw[3] = 92;
          object_list[str_counter.c_str()].assign_image(renderer, my_file.c_str());    
      }

      
      update_renderer(renderer, object_list);  
    
    
    }
    return GP;
}
 
  
int main(int argc, char ** argv)
{
  game_parameters GP;
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window * window = SDL_CreateWindow("SDL2 Keyboard/Mouse events",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1440, 900, 0);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);  
  
  GP.mode = 0;
  while(!GP.quit)
  {
    if (GP.mode == 0) {GP = main_game(renderer, GP);}
    if (GP.mode == 1) {GP = game_over(renderer, GP);}
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
  
}
