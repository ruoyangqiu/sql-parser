/*
 * sql_tests.cpp
 */

#include "lib/SQLParser.h"
#include <stdio.h>
#include <string>
#include <cassert>

#define STREQUALS(str1, str2) std::string(str1).compare(std::string(str2)) == 0

#define ASSERT(cond) if (!(cond)) { fprintf(stderr, "failed! Assertion (" #cond ")\n"); return; }
#define ASSERT_STR(STR1, STR2) ASSERT(STREQUALS(STR1, STR2));


void SelectTest1() {
	printf("Test: SelectTest1... ");
	fflush(stdout);

	const char* sql = "SELECT age, name, address from table WHERE age > 12.5;";
	Statement* stmt = SQLParser::parseSQL(sql);
	ASSERT(stmt != NULL);
	ASSERT(stmt->_type == eSelect);

	SelectStatement* select = (SelectStatement*) stmt;

	ASSERT(select->_select_list->size() == 3);
	ASSERT_STR(select->_select_list->at(0)->name, "age");
	ASSERT_STR(select->_select_list->at(1)->name, "name");
	ASSERT_STR(select->_select_list->at(2)->name, "address");

	ASSERT(select->_from_table != NULL);
	ASSERT(select->_from_table->_type == eTableName);
	ASSERT_STR(select->_from_table->_table_names->at(0), "table");

	printf("passed!\n");
}

void SelectTest2() {
	printf("Test: SelectTest2... ");
	fflush(stdout);

	const char* sql = "SELECT age, name, address FROM (SELECT age FROM table, table2);";
	Statement* stmt = SQLParser::parseSQL(sql);
	ASSERT(stmt != NULL);
	ASSERT(stmt->_type == eSelect);

	SelectStatement* select = (SelectStatement*) stmt;

	ASSERT(select->_select_list->size() == 3);
	ASSERT_STR(select->_select_list->at(0)->name, "age");
	ASSERT_STR(select->_select_list->at(1)->name, "name");
	ASSERT_STR(select->_select_list->at(2)->name, "address");

	ASSERT(select->_from_table != NULL);
	ASSERT(select->_from_table->_type == eTableSelect);
	ASSERT(select->_from_table->_stmt != NULL);
	ASSERT(select->_from_table->_stmt->_select_list->size() == 1);
	ASSERT_STR(select->_from_table->_stmt->_from_table->_table_names->at(0), "table");
	ASSERT_STR(select->_from_table->_stmt->_from_table->_table_names->at(1), "table2");

	printf("passed!\n");
}

void SelectTest3() {
	printf("Test: SelectTest3... ");
	fflush(stdout);

	const char* sql = "SELECT name, AVG(age) FROM table GROUP BY name";
	Statement* stmt = SQLParser::parseSQL(sql);
	ASSERT(stmt != NULL);
	ASSERT(stmt->_type == eSelect);

	SelectStatement* select = (SelectStatement*) stmt;

	ASSERT(select->_select_list->size() == 2);

	ASSERT(select->_select_list->at(0)->func_name == NULL);
	ASSERT_STR("name", select->_select_list->at(0)->name);

	ASSERT(select->_select_list->at(1)->func_name != NULL);
	ASSERT_STR("age", select->_select_list->at(1)->name);
	ASSERT_STR("AVG", select->_select_list->at(1)->func_name);

	ASSERT(select->_group_by != NULL);
	ASSERT(select->_group_by->size() == 1);
	ASSERT_STR("name", select->_group_by->at(0)->name);

	printf("passed!\n");
}



int main(int argc, char *argv[]) {

	printf("\n######################################\n");
	printf("## Running all tests...\n\n");

	SelectTest1();
	SelectTest2();
	SelectTest3();

	printf("\n## Finished running all tests...\n");
	printf("######################################\n");

	return 0;
}