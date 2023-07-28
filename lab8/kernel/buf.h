struct buf {
  int valid;   // has data been read from disk?
  int disk;    // does disk "own" buf?
  uint dev;
  uint blockno;
  struct sleeplock lock;
  uint refcnt;
  struct buf *prev; // 新增定义 
  struct buf *next;
  uchar data[BSIZE];
  uint lastuse;
};
