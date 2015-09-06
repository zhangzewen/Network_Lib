#include <iostream>
#include <assert.h>
#include <glog/logging.h>
#include "HttpServer.h"

int main(int argc, char** argv)
{
  assert(argc);
  assert(argv);
  google::InitGoogleLogging(argv[0]);
  //  google::SetLogDestination(google::INFO, "./logs/info");
  google::LogToStderr();
  //  FLAGS_log_dir = "./logs/";
  //  FLAGS_logbufsecs = 60;
  //  FLAGS_max_log_size = 10;
  LOG(INFO) << "Server start....";
  HttpServer server("127.0.0.1", 8989);
  server.start();
  google::ShutdownGoogleLogging();
  return 0;
}
