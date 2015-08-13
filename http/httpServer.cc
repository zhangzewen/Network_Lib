#include <iostream>
#include <assert.h>
#include "HttpServer.h"

int main(int argc, char** argv)
{
  assert(argc);
  assert(argv);
  HttpServer server("127.0.0.1", 8989);
  server.start();
  return 0;
}
