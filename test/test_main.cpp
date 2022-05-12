#include <Arduino.h>
#include <unity.h>

void test_test(void) {
  TEST_ASSERT_EQUAL(12, 6*2);
}

void setup() {
  delay(2000);
  UNITY_BEGIN();

  RUN_TEST(test_test);
}

void loop() {}