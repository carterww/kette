#include "unity.h"
#include "kette.h"
#include "common.h"

struct test_slist list = { 0, SLIST_INIT(list.head) };

void setUp(void) {}

void tearDown(void)
{
  list.head.next = &list.head;
}

void test_slist_list_entry_macro(void)
{
  struct test_slist *entry = list_entry(&list.head, struct test_slist, head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list, entry,
      "list_entry does not return the correct pointer");
}

void test_slist_init_macro(void)
{
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head.next is not initialized to head");
}

void test_slist_init_function(void)
{
  struct test_slist list;
  slist_init(&list.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head.next is not initialized to head");
}

void test_slist_empty_true(void)
{
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, list_empty(&list.head),
      "list is not empty");
}

void test_slist_empty_false(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  slist_add(&second.head, &list.head);
  TEST_ASSERT_EQUAL_MESSAGE(0, list_empty(&list.head),
      "list is empty");
}

void test_slist_find_prev(void)
{
  struct slink *prev;
  slist_find_prev(&list.head, &prev);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, prev,
      "prev is not head");
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  slist_add(&second.head, &list.head);
  slist_find_prev(&list.head, &prev);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&second.head, prev,
      "prev is not second element");
}

#define TEST_LEN_TWO_SLIST(list, second) \
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&second.head, list.head.next,\
      "head's next does not point to second");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, second.head.next,\
      "second's next does not point to head");

#define TEST_LEN_THREE_SLIST(list, second, third) \
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, third.head.next,\
      "third's next does not point to head");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&second.head, list.head.next,\
      "head's next does not point to second");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&third.head, second.head.next,\
      "second's next does not point to third");

void test_slist_add_single(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  slist_add(&second.head, &list.head);
  TEST_LEN_TWO_SLIST(list, second);
}

void test_slist_add_tail_single(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  slist_add_tail(&second.head, &list.head);
  TEST_LEN_TWO_SLIST(list, second);
}

void test_slist_add_multiple(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  struct test_slist third = { 2, SLIST_INIT(third.head) };
  slist_add(&third.head, &list.head);
  slist_add(&second.head, &list.head);
  TEST_LEN_THREE_SLIST(list, second, third);
}

void test_slist_add_tail_multiple(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  struct test_slist third = { 2, SLIST_INIT(third.head) };
  slist_add_tail(&second.head, &list.head);
  slist_add_tail(&third.head, &list.head);
  TEST_LEN_THREE_SLIST(list, second, third);
}

void test_slist_del_empty(void)
{
  slist_del(&list.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head.next is not initialized to head");
}

void test_slist_del_single(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  slist_add(&second.head, &list.head);
  slist_del(&second.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head.next is not initialized to head");
}

void test_slist_splice_empty(void)
{
  slist_splice(&list.head, &list.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head.next is not initialized to head");
}

void test_slist_splice_from_same(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  slist_add(&second.head, &list.head);
  slist_splice(&list.head, &second.head);
  TEST_LEN_TWO_SLIST(list, second);
}

void test_slist_splice_disjoint(void)
{
  struct test_slist list2 = { 1, SLIST_INIT(list2.head) };
  struct test_slist second = { 2, SLIST_INIT(second.head) };
  slist_add(&second.head, &list2.head);
  slist_splice(&list2.head, &list.head);
  TEST_LEN_THREE_SLIST(list, list2, second);
}

void test_slist_traverse_forward(void)
{
  struct test_slist second = { 1, SLIST_INIT(second.head) };
  struct test_slist third = { 2, SLIST_INIT(third.head) };
  slist_add(&third.head, &list.head);
  slist_add(&second.head, &list.head);
  struct test_slist *entry;
  int i = 0;
  list_for_each(&list.head, entry, struct test_slist, head) {
    TEST_ASSERT_EQUAL_MESSAGE(entry->data, i++,
        "entry data does not match iterator");
  }
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_slist_list_entry_macro);
  
  RUN_TEST(test_slist_init_macro);
  RUN_TEST(test_slist_init_function);

  RUN_TEST(test_slist_empty_true);
  RUN_TEST(test_slist_empty_false);

  RUN_TEST(test_slist_find_prev);

  RUN_TEST(test_slist_add_single);
  RUN_TEST(test_slist_add_tail_single);

  // Test multiple to ensure order is preserved
  RUN_TEST(test_slist_add_multiple);
  RUN_TEST(test_slist_add_tail_multiple);

  RUN_TEST(test_slist_del_empty);
  RUN_TEST(test_slist_del_single);

  RUN_TEST(test_slist_splice_empty);
  RUN_TEST(test_slist_splice_from_same);
  RUN_TEST(test_slist_splice_disjoint);

  RUN_TEST(test_slist_traverse_forward);
  return UNITY_END();
}
