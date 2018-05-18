/**
 * Copyright eBay Corporation 2018
 */

#pragma once

extern "C" {
#include <event.h>
#include <sys/time.h>
}
#include <condition_variable>
#include <map>
#include <memory>
#include <vector>

#include "iomgr.hpp"

namespace iomgr
{

constexpr size_t MAX_PRI = 10;

struct fd_info;
   
struct thread_info {
   pthread_t tid;
   uint64_t count;
   uint64_t time_spent_ns;
   int id;
   int ev_fd;
   int inited;
   int *epollfd_pri;
};


struct ioMgrImpl {
  std::vector<thread_info> threads;
  std::vector <struct fd_info *>global_fd; /* fds shared between the threads */
  static thread_local int epollfd_pri[MAX_PRI];
  static thread_local int epollfd;

  ioMgrImpl(size_t num_ep, size_t num_threads);
  void start();
  void local_init();
  void add_ep(class EndPoint *ep);
  void add_fd(int fd, ev_callback cb, int ev, int pri, void *cookie);
  void add_local_fd(int fd, ev_callback cb, int ev, int pri, void *cookie);
  void add_fd_to_thread(int id, int fd, ev_callback cb, int ev, 
                        int pri, void *cookie);
  void callback(void *data, uint32_t ev);
  void process_done_impl(void *data,int ev);
  struct thread_info *get_thread_info(pthread_t &tid);
  void print_perf_cntrs();
  bool can_process(void *data, uint32_t event);
  void fd_reschedule(int fd, uint32_t event);
  void process_evfd(int fd, void *data, uint32_t event);
  struct thread_info *get_tid_info(pthread_t &tid);
  void process_done(int fd, int ev);
  void wait_for_ready();

 private:
  size_t num_ep;
  size_t num_threads;
  std::vector<class EndPoint *> ep_list;
  std::map<int, fd_info *> fd_info_map;
  std::mutex map_mtx;
  std::mutex cv_mtx;
  std::condition_variable cv;
  bool ready;
};

} /* iomgr */ 
