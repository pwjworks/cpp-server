//
// Created by pwjworks on 2021/10/25.
//
#include "io_buffer.h"
#include <cassert>
#include <cerrno>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <unistd.h>

#define EXTRA_MEM_LIMIT(5U * 1024 * 1024)

using namespace server;
using namespace server::net;
