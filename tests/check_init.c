#include <check.h>
#include <stdbool.h>
#include "../src/game.h"
#include <stdlib.h>

START_TEST (test_create_game_object)
{
  game_object *player = create_game_object ();
  ck_assert_uint_eq (player->id, 0); 
}
END_TEST

Suite
*init_suite (void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create ("init");

  /* Core test case */
  tc_core = tcase_create ("core");

  tcase_add_test (tc_core, test_create_game_object);
  suite_add_tcase (s, tc_core);

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = init_suite ();
  sr = srunner_create (s);

  init_world (); /* XXX: Make this a unit. */

  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
