#include "types.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

// COW的引用计数结构
struct {
  uint8 ref_cnt; //计数
  struct spinlock lock;	//自旋锁
} cows[(PHYSTOP - KERNBASE) >> 12];//压缩的页引用数

void increfcnt(uint64 pa) {
  if (pa < KERNBASE) {
    return;
  }
  pa = (pa - KERNBASE) >> 12;
  acquire(&cows[pa].lock);
  ++cows[pa].ref_cnt;
  release(&cows[pa].lock); //物理地址的引用计数元素
}// 引用计数+1

uint8 decrefcnt(uint64 pa) {
  uint8 ret;
  if (pa < KERNBASE) {
    return 0;
  }
  pa = (pa - KERNBASE) >> 12;
  acquire(&cows[pa].lock);
  ret = --cows[pa].ref_cnt;
  release(&cows[pa].lock);
  return ret;
}// 引用计数减一
