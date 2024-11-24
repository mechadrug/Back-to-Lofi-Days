#include <SFML/Window.hpp>
#include "../include/Shop.h"
#include "../include/Game.h"
#include<iostream>
using namespace std; 
int main(){
  try{
    Game game;
    game.run();
  }catch(const exception&e){
    cerr<<"!error: "<<e.what()<<endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
  //cout<<"hello"<<endl;
}