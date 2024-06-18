#include "unity.h"
#include "kette.h"
#include "common.h"


struct test_dlist list = { 0, DLIST_INIT(list.head) };

void setUp(void) {}

void tearDown(void)
{
  list.head.next = &list.head;
  list.head.prev = &list.head;
}

void test_dlist_list_entry_macro(void)
{
  struct test_dlist *entry = list_entry(&list.head, struct test_dlist, head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list, entry,
      "list_entry does not return the correct pointer");
}

void test_dlist_init_macro(void)
{
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head.next is not initialized to head");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.prev,
      "head.prev is not initialized to head");
}

void test_dlist_init_function(void)
{
  struct test_dlist list;
  dlist_init(&list.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head.next is not initialized to head");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.prev,
      "head.prev is not initialized to head");
}

void test_dlist_empty_true(void)
{
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, list_empty(&list.head),
      "list is not empty");
}

void test_dlist_empty_false(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  dlist_add(&second.head, &list.head);
  TEST_ASSERT_EQUAL_MESSAGE(0, list_empty(&list.head),
      "list is empty");
}

#define TEST_LEN_TWO_DLIST(list, second) \
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&second.head, list.head.next,\
      "head's next does not point to second");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, second.head.next,\
      "second's next does not point to head");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, second.head.prev,\
      "second's prev does not point to head");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&second.head, list.head.prev,\
      "head's prev does not point to second");

#define TEST_LEN_THREE_DLIST(list, second, third) \
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, second.head.prev,\
      "second's prev does not point to head");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, third.head.next,\
      "third's next does not point to head");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&second.head, list.head.next,\
      "head's next does not point to second");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&second.head, third.head.prev,\
      "third's prev does not point to second");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&third.head, second.head.next,\
      "second's next does not point to third");\
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&third.head, list.head.prev,\
      "head's prev does not point to third");

void test_dlist_add_single(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  dlist_add(&second.head, &list.head);
  TEST_LEN_TWO_DLIST(list, second);
}

void test_dlist_add_tail_single(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  dlist_add_tail(&second.head, &list.head);
  TEST_LEN_TWO_DLIST(list, second);
}

void test_dlist_add_multiple(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  struct test_dlist third = { 2, DLIST_INIT(third.head) };
  dlist_add(&third.head, &list.head);
  dlist_add(&second.head, &list.head);
  TEST_LEN_THREE_DLIST(list, second, third);
}

void test_dlist_add_tail_multiple(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  struct test_dlist third = { 2, DLIST_INIT(third.head) };
  dlist_add_tail(&second.head, &list.head);
  dlist_add_tail(&third.head, &list.head);
  TEST_LEN_THREE_DLIST(list, second, third);
}

void test_dlist_del_empty(void)
{
  dlist_del(&list.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head's next does not point to head");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.prev,
      "head's prev does not point to head");
}

void test_dlist_del_single(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  dlist_add(&second.head, &list.head);
  dlist_del(&second.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head's next does not point to head");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.prev,
      "head's prev does not point to head");
}

void test_dlist_splice_empty(void)
{
  dlist_splice(&list.head, &list.head);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.next,
      "head's next does not point to head");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&list.head, list.head.prev,
      "head's prev does not point to head");
}

void test_dlist_splice_from_same(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  dlist_add(&second.head, &list.head);
  dlist_splice(&list.head, &second.head);
  TEST_LEN_TWO_DLIST(list, second);
}

void test_dlist_splice_disjoint(void)
{
  struct test_dlist list2 = { 1, DLIST_INIT(list2.head) };
  struct test_dlist second = { 2, DLIST_INIT(second.head) };
  dlist_add(&second.head, &list2.head);
  dlist_splice(&list2.head, &list.head);
  TEST_LEN_THREE_DLIST(list, list2, second);
}

void test_dlist_traverse_forward(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  struct test_dlist third = { 2, DLIST_INIT(third.head) };
  dlist_add(&third.head, &list.head);
  dlist_add(&second.head, &list.head);

  struct test_dlist *entry;
  int i = 0;
  list_for_each(&list.head, entry, struct test_dlist, head) {
    TEST_ASSERT_EQUAL_MESSAGE(entry->data, i++,
        "entry data does not match iterator");
  }
}

void test_dlist_traverse_backward(void)
{
  struct test_dlist second = { 1, DLIST_INIT(second.head) };
  struct test_dlist third = { 2, DLIST_INIT(third.head) };
  dlist_add(&third.head, &list.head);
  dlist_add(&second.head, &list.head);

  struct test_dlist *entry;
  int i = 2;
  dlist_for_each_reverse(&third.head, entry, struct test_dlist, head) {
    TEST_ASSERT_EQUAL_MESSAGE(entry->data, i--,
        "entry data does not match iterator");
  }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_dlist_init_macro);
    RUN_TEST(test_dlist_init_function);

    RUN_TEST(test_dlist_list_entry_macro);

    RUN_TEST(test_dlist_empty_true);
    RUN_TEST(test_dlist_empty_false);

    RUN_TEST(test_dlist_add_single);
    RUN_TEST(test_dlist_add_tail_single);

    // Test multiple to ensure order is preserved
    RUN_TEST(test_dlist_add_multiple);
    RUN_TEST(test_dlist_add_tail_multiple);

    RUN_TEST(test_dlist_del_empty);
    RUN_TEST(test_dlist_del_single);

    RUN_TEST(test_dlist_splice_empty);
    RUN_TEST(test_dlist_splice_from_same);
    RUN_TEST(test_dlist_splice_disjoint);

    RUN_TEST(test_dlist_traverse_forward);
    RUN_TEST(test_dlist_traverse_backward);
    return UNITY_END();
}
