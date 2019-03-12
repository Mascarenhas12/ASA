#ifndef NETWORK_H
#define NETWORK_H
#include <forward_list>
#include <iostream>

class Router{
  private:
    int id;
    std::forward_list<Router> adj;
  public:
    Router(int id);
    ~Router();
    int getID();
    void addAdj(int id);
};

class Sub_Net{
  private:
    int maxID;
    std::forward_list<Router> critR;
    std::forward_list<Router> nodes;
  public:
    Sub_Net();
    ~Sub_Net();
};

class Network{
private:
  std::forward_list<Router> nodes;
  std::forward_list<Sub_Net> subs;
  int numSub;
public:
  Network();
  ~Network();
};

#endif
