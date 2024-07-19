#include "acutest.h"

/* filesystem.h */
extern void test__fs_native_delete(void);
extern void test__fs_native_mkdir(void);
extern void test__fs_native_open(void);
extern void test__fs_native_read(void);
extern void test__fs_native_write(void);
extern void test_fs_setup(void);
extern void test_fs_append(void);
extern void test_fs_delete(void);
extern void test_fs_exists(void);
extern void test_fs_get_cwd(void);
extern void test_fs_get_info(void);
extern void test_fs_mkdir(void);
extern void test_fs_read(void);
extern void test_fs_write(void);
extern void test_fs_insert_basepath(void);
extern void test_fs_remove_basepath(void);

/* ini.h */
extern void test_ini_find_section(void);
extern void test_ini_section_exists(void);
extern void test_ini_property_exists(void);
extern void test_ini_value(void);
extern void test_ini_value_as_int(void);
extern void test_ini_value_as_float(void);
extern void test_ini_value_as_bool(void);

TEST_LIST = {
  /* filesystem.h */
  { "_fs_native_delete", test__fs_native_delete },
  { "_fs_native_mkdir", test__fs_native_mkdir },
  { "_fs_native_open", test__fs_native_open },
  { "_fs_native_read", test__fs_native_read },
  { "_fs_native_write", test__fs_native_write },
  { "fs_setup", test_fs_setup },
  { "fs_append", test_fs_append },
  { "fs_delete", test_fs_delete },
  { "fs_exists", test_fs_exists },
  { "fs_get_cwd", test_fs_get_cwd },
  { "fs_get_info", test_fs_get_info },
  { "fs_mkdir", test_fs_mkdir },
  { "fs_read", test_fs_read },
  { "fs_write", test_fs_write },
  { "fs_insert_basepath", test_fs_insert_basepath },
  { "fs_remove_basepath", test_fs_remove_basepath },

  /* ini.h */
  { "ini_find_section", test_ini_find_section },
  { "ini_section_exists", test_ini_section_exists },
  { "ini_property_exists", test_ini_property_exists },
  { "ini_value", test_ini_value },
  { "ini_value_as_int", test_ini_value_as_int },
  { "ini_value_as_float", test_ini_value_as_float },
  { "ini_value_as_bool", test_ini_value_as_bool },

  /* always last. */
  { NULL, NULL }
};