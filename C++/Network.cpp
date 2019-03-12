#include <forward_list>
#include "Network.h"
#include <iostream>
#include <stdio.h>

Network::Network(){
  puts("Criei Network");
}
Sub_Net::Sub_Net(){
  puts("Criei Sub_Net");
}
Router::Router(int n): id(n){
  puts("Criei Router");
}

Network::~Network(){
  puts("Apaguei Network");
}
Sub_Net::~Sub_Net(){
  puts("Apaguei Sub_Net");
}

int Router::getID(){
  return id;
}

Router::~Router(){
  printf("%d\n", id);
}
