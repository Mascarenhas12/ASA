#include <forward_list>
#include "Network.h"
#include <iostream>
#include <stdio.h>

Network::Network(int n): numSub(0), numNodes(n){
  /*for(int i=0;i<numNodes;i++){
    Router r(i);
    this::addRouter(r);
  }*/
  puts("Criei Network");
}

void Network::addRouter(Router r){
  //add to nodes list
}

Router::Router(int n): id(n){
  puts("Criei Router");
}

Network::~Network(){
  puts("Apaguei Network");
}

int Router::getID(){
  return id;
}

Router::~Router(){
  printf("%d\n", id);
}
