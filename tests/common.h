#ifndef TESTS_COMMON_H
#define TESTS_COMMON_H

#include "kette.h"

struct test_slist {
  long data;
  struct slink head;
};

struct test_dlist {
  long data;
  struct dlink head;
};

#endif // TESTS_COMMON_H
