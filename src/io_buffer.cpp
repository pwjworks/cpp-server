//
// Created by pwjworks on 2021/10/25.
//
#include "io_buffer.h"
#include "print_error.h"
#include <cassert>
#include <cerrno>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <unistd.h>

#define EXTRA_MEM_LIMIT (5U * 1024 * 1024)

using namespace server;
using namespace server::net;

int buffer_pool::mem_size_ceil(int mem_size){
  int res_size;
  if(mem_size<=u4K)
    return u4K;
  else if(mem_size<=u16K)
    return u16K;
  else if(mem_size<=u256K)
    return u256K;
  else if(mem_size<=u1M)
    return u1M;
  else if(mem_size<=u4M)
    return u4M;
  else if(mem_size<=u8M)
    return u8M;
  else
    return -1;
}

io_buffer* buffer_pool::alloc(int N){
  int mem_size= mem_size_ceil(N);
  if(mem_size==-1)
    return nullptr;

  ::pthread_mutex_lock(&_mutex);
  if(!_pool[mem_size]){
    if(_total_mem+mem_size/1024>=EXTRA_MEM_LIMIT){
      exit_log("use too many memory");
      ::exit(1);
    }
    // alloc memory for io buffer.
    io_buffer* new_buf=new io_buffer(mem_size);
    exit_if(new_buf== nullptr,"new buffer error!");
    _total_mem+=mem_size/1024;
    pthread_mutex_unlock(&_mutex);
    return new_buf;
  }
  io_buffer* target=_pool[mem_size];
  _pool[mem_size]=target->next;
  ::pthread_mutex_unlock(&_mutex);

  target->next= nullptr;
  return target;
}

void buffer_pool::revert(io_buffer* buffer){
  int curr_capacity=buffer->capacity;
  buffer->length=0;
  buffer->head=0;
  ::pthread_mutex_lock(&_mutex);

  assert(_pool.find(curr_capacity)!=_pool.end());
  buffer->next=_pool[curr_capacity];
  _pool[curr_capacity]=buffer;

  ::pthread_mutex_unlock(&_mutex);
}