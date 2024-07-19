#define TEST_NO_MAIN
#include "acutest.h"

#define INI_IMPL
#include "ini.h"

const char* data_str = 
"; global section\n"
"network = wireless\n"
"; owner section\n"
"[owner]\n"
"name = John Doe\n"
"organization = Acme Widgets Inc.\n"
"; database section\n"
"[database]\n"
"server = 192.0.2.62\n"
"port = 143\n"
"file = payroll.dat\n"
"; variables section\n"
"[variables]\n"
"int=1234\n"
"float=12.34\n"
"bool=true\n"
"string=hello\n";

void test_ini_find_section(void) {
  ini_t* ini = ini_load(data_str);

  TEST_CHECK(ini_find_section(ini, "owner") == 1);
  TEST_CHECK(ini_find_section(ini, "database") == 2);
  TEST_CHECK(ini_find_section(ini, "nope") == -1);
}

void test_ini_section_exists(void) {
  ini_t* ini = ini_load(data_str);

  TEST_CHECK(ini_section_exists(ini, "owner") == true);
  TEST_CHECK(ini_section_exists(ini, "database") == true);
  TEST_CHECK(ini_section_exists(ini, "nope") == false);
}

void test_ini_property_exists(void) {
  ini_t* ini = ini_load(data_str);

  int owner_id = ini_find_section(ini, "owner");
  int database_id = ini_find_section(ini, "database");

  TEST_CHECK(ini_property_exists(ini, INI_GLOBAL_SECTION, "network") == true);

  TEST_CHECK(ini_property_exists(ini, owner_id, "name") == true);
  TEST_CHECK(ini_property_exists(ini, owner_id, "organization") == true);
  TEST_CHECK(ini_property_exists(ini, owner_id, "nope") == false);

  TEST_CHECK(ini_property_exists(ini, database_id, "server") == true);
  TEST_CHECK(ini_property_exists(ini, database_id, "port") == true);
  TEST_CHECK(ini_property_exists(ini, database_id, "file") == true);
  TEST_CHECK(ini_property_exists(ini, database_id, "nope") == false);
}

void test_ini_value(void) {
  ini_t* ini = ini_load(data_str);

  int owner_id = ini_find_section(ini, "owner");
  int database_id = ini_find_section(ini, "database");

  TEST_CHECK(strcmp(ini_value(ini, INI_GLOBAL_SECTION, "network"), "wireless") == 0);

  TEST_CHECK(strcmp(ini_value(ini, owner_id, "name"), "John Doe") == 0);
  TEST_CHECK(strcmp(ini_value(ini, owner_id, "organization"), "Acme Widgets Inc.") == 0);
  TEST_CHECK(ini_value(ini, owner_id, "nope") == NULL);

  TEST_CHECK(strcmp(ini_value(ini, database_id, "server"), "192.0.2.62") == 0);
  TEST_CHECK(strcmp(ini_value(ini, database_id, "port"), "143") == 0);
  TEST_CHECK(strcmp(ini_value(ini, database_id, "file"), "payroll.dat") == 0);
  TEST_CHECK(ini_value(ini, database_id, "nope") == NULL);

  TEST_CHECK(strcmp(ini_value(ini, database_id, "server"), "192.0.2.62") == 0);
  TEST_CHECK(strcmp(ini_value(ini, database_id, "port"), "143") == 0);
  TEST_CHECK(strcmp(ini_value(ini, database_id, "file"), "payroll.dat") == 0);
  TEST_CHECK(ini_value(ini, database_id, "nope") == NULL);
}

void test_ini_value_as_int(void) {
  ini_t* ini = ini_load(data_str);

  int variables_id = ini_find_section(ini, "variables");

  TEST_CHECK(ini_value_as_int(ini, variables_id, "int") == 1234);
  TEST_CHECK(ini_value_as_int(ini, variables_id, "float") == 12);
}

void test_ini_value_as_float(void) {
  ini_t* ini = ini_load(data_str);

  int variables_id = ini_find_section(ini, "variables");

  TEST_CHECK(ini_value_as_float(ini, variables_id, "int") == 1234.0f);
  TEST_CHECK(ini_value_as_float(ini, variables_id, "float") == 12.34f);
}

void test_ini_value_as_bool(void) {
  ini_t* ini = ini_load(data_str);

  int variables_id = ini_find_section(ini, "variables");

  TEST_CHECK(ini_value_as_bool(ini, variables_id, "int") == false);
  TEST_CHECK(ini_value_as_bool(ini, variables_id, "float") == false);
  TEST_CHECK(ini_value_as_bool(ini, variables_id, "bool") == true);
  TEST_CHECK(ini_value_as_bool(ini, variables_id, "string") == false);
}