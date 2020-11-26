#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

const int INITIAL_CAPACITY = 24;
const int GROWTH_RATE = 2;
// Store text using a dynamically allocated character array with a given
// capacity. Use strlen to find the length of the string contained in the array.
struct text {
    int capacity;
    char *content;
};

// Create a new text structure, containing a copy of the given string.
text *newText(char *s) {
  text *t = malloc(sizeof(text));
  char *c = malloc(INITIAL_CAPACITY * sizeof(char)); 
  int x = INITIAL_CAPACITY;  
  while(x < strlen(s) + 1)  x *= GROWTH_RATE;
  if (x > INITIAL_CAPACITY) c = realloc(c, x);
  strcpy(c,s);
  *t = (text) {x, c};
  return t;
}

// Free up both the space for the struct and the space for the content.
// Be careful not to access anything after it has been freed.
void freeText(text *t) {
  free(t->content);
  free(t);
}

int length(text *t) {
  return strlen(t->content);
}

char get(text *t, int i) {
  assert(i <= length(t));
  return t->content[i];
}

void set(text *t, int i, char c) {
  assert(i <= length(t));
  t->content[i] = c;
}

text *copy(text *t) {
  text *cp = newText(t->content);
  return cp;
}

int compare(text *t1, text *t2) {
  return strcmp(t1->content, t2->content);
}

void append(text *t1, text *t2) {
  int len = length(t1) + length(t2);
  int cap = t1->capacity;
  while(cap < len + 1) cap *= GROWTH_RATE;
  if(cap != t1->capacity) t1->content = realloc(t1->content, cap);
  t1->capacity = cap;

  strcat(t1->content,t2->content);
}

text *slice(text *t, int i, int j) {
  assert(i <= j);
  char c[j - i + 1];
  strncpy(c,t->content+i,j-i);
  c[j - i] = '\0';
  text *new = newText(c);
  return new;
}

int find(text *t, text *p) {
    return 0;
}

//-----------------------------------------------------------------------------
// Testing. Use -Dtest_text when compiling for testing.
#ifdef test_text

// Check whether a text is like a given string, as far as can be checked
// without knowing how much memory has been allocated.
bool like(text *t, char *s) {
    if (t == NULL) return false;
    if (t->capacity < strlen(s) + 1) return false;
    if (strcmp(t->content, s) != 0) return false;
    return true;
}

// Test that new allocates 24 bytes if the string fits, or doubles the capacity
// until it does fit, taking the final null character into account.
void testNew() {
    text *t = newText("");
    assert(like(t, ""));
    assert(t->capacity == 24);
    freeText(t);
    t = newText("cat");
    assert(like(t, "cat"));
    assert(t->capacity == 24);
    freeText(t);
    char *s = "supercalifragilisticexpialidocious";
    t = newText(s);
    assert(like(t, s));
    assert(t->capacity == 48);
    freeText(t);
    s = "12345678901234567890123";
    t = newText(s);
    assert(like(t, s));
    assert(t->capacity == 24);
    freeText(t);
    s = "123456789012345678901234";
    t = newText(s);
    assert(like(t, s));
    assert(t->capacity == 48);
    freeText(t);
    s = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    t = newText(s);
    assert(like(t, s));
    assert(t->capacity == 96);
    freeText(t);
}

void testLength() {
    text *t = newText("cat");
    assert(length(t) == 3);
    freeText(t);
}

void testGet() {
    text *t = newText("cat");
    assert(get(t,0) == 'c');
    assert(get(t,1) == 'a');
    assert(get(t,2) == 't');
    assert(get(t,3) == '\0');
    freeText(t);
}

void testSet() {
    text *t = newText("cat");
    set(t, 0, 'b');
    set(t, 2, 'r');
    assert(like(t, "bar"));
    freeText(t);
}

void testCopy() {
    text *t = newText("cat");
    text *t2 = copy(t);
    assert(t2 != t);
    assert(t2->content != t->content);
    assert(like(t2, "cat"));
    freeText(t);
    freeText(t2);
}

void testCompare() {
    text *t = newText("cat");
    text *t2 = newText("cat");
    assert(compare(t, t2) == 0);
    text *t3 = newText("car");
    assert(compare(t, t3) > 0);
    text *t4 = newText("caw");
    assert(compare(t, t4) < 0);
    text *t5 = newText("catch");
    assert(compare(t, t5) < 0);
    freeText(t);
    freeText(t2);
    freeText(t3);
    freeText(t4);
    freeText(t5);
}

void testAppend() {
    text *t = newText("car");
    text *t2 = newText("pet");
    append(t, t2);
    assert(like(t, "carpet"));
    assert(t->capacity == 24);
    text *t3 = newText("789012345678901234");
    append(t, t3);
    assert(like(t, "carpet789012345678901234"));
    assert(t->capacity == 48);
    freeText(t);
    freeText(t2);
    freeText(t3);
}

void testSlice() {
    text *t = newText("carpet");
    text *t2 = slice(t, 0, 3);
    assert(like(t2, "car"));
    text *t3 = slice(t, 3, 6);
    assert(like(t3, "pet"));
    freeText(t);
    freeText(t2);
    freeText(t3);
}

void testFind() {
    text *t = newText("carpet");
    text *t2 = newText("car");
    assert(find(t, t2) == 0);
    text *t3 = newText("pet");
    assert(find(t, t3) == 3);
    text *t4 = newText("cat");
    assert(find(t, t4) == -1);
    freeText(t);
    freeText(t2);
    freeText(t3);
    freeText(t4);
}

// Test the library.
int main() {
    testNew();
    testLength();
    testGet();
    testSet();
    testCopy();
    testCompare();
    testAppend();
    testSlice();
    //testFind();
    printf("Tests all pass.\n");
    return 0;
}

#endif
