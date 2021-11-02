//
// Created by pwjworks on 2021/10/25.
//

#include "io_buffer.h"
#include <cassert>
#include <iostream>
using namespace server::net;

int main() {
  auto *buffer = new io_buffer(1024);
  assert(buffer);
  std::cout << "hello world!";
}