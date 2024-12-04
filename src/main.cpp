#include <SFML/Window.hpp>
#include "../include/Shop.h"
#include "../include/Game.h"
#include<iostream>
#include "../include/TexturePool.h"
using namespace std; 
int main(){
  try{
    Game game;
    game.run();
  }catch(const exception&e){
    cerr<<"!error: "<<e.what()<<endl;
    return EXIT_FAILURE;
  }
  TexturePool::clear();
  return EXIT_SUCCESS;
}                                                     