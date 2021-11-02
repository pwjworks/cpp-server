//
// Created by pwjworks on 2021/10/25.
//

#ifndef CPP_SERVER_IO_BUFFER_H
#define CPP_SERVER_IO_BUFFER_H


#include <cassert>
#include <cstdint>
#include <cstring>
#include <list>
#include <pthread.h>
#include <unordered_map>

namespace server {
  namespace net {
    class io_buffer {
    public:
      explicit io_buffer(int size) : capacity(size), length(0), head(0), next(nullptr) {
        data = new char[size];
        assert(data);
      };
      void clear() {
        length = head = 0;
      }
      /**
       * @brief 调整缓存数据到最前端
       */
      void adjust() {
        if (head != 0) {
          if (length != 0) {
            // 把数据挪到前端
            ::memmove(data, data + head, length);
          }
          head = 0;
        }
      }
      /**
       * @brief 复制其他缓存的数据
       * @param other 其他缓存
       */
      void copy(const io_buffer *other) {
        // 复制自己的数据
        ::memcpy(data, other->data + other->head, other->length);
        head = 0;
        length = other->length;
      }
      /**
       * @brief 弹出数据
       * @param len 数据的长度
       */
      void pop(int len) {
        length -= len;
        head += len;
      }

      int length;
      int capacity;
      int head;
      io_buffer *next;
      char *data;
    };
    class buffer_pool {
    public:
      enum MEM_CAP {
        u4K = 4096,
        u16K = 16384,
        u64K = 65536,
        u256K = 262144,
        u1M = 1048576,
        u4M = 4194304,
        u8M = 8388608
      };
      /**
       * @brief 初始化缓存池
       */
      static void init() {
        _ins = new buffer_pool();
        assert(_ins);
      }

    private:
      buffer_pool();
      buffer_pool(const buffer_pool &);
      const buffer_pool &operator=(const buffer_pool &);
      static buffer_pool *_ins;
      static pthread_mutex_t _mutex;
      static pthread_once_t _once;
    };

    class tcp_buffer {
    public:
      tcp_buffer() : _buf(nullptr) {}
      ~tcp_buffer() { clear(); }

      int length() const { return _buf ? _buf->length : 0; }

      void pop(int len);

      void clear();

    protected:
      io_buffer *_buf;
    };
    class input_buffer : public tcp_buffer {
    public:
      int read_data(int df);

      const char *data() const { return _buf ? _buf->data + _buf->head : nullptr }

      void adjust() {
        if (_buf) _buf->adjust();
      }
    };

    class output_buffer : public tcp_buffer {
    public:
      int send_data(const char *data, int datalen);

      int write_fd(int fd);
    };
  }// namespace net
}// namespace server


#endif//CPP_SERVER_IO_BUFFER_H
