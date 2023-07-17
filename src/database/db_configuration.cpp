#include <db_create.h>

void db_configuration()
{
    DatabaseHandler database;
    database.open("./database/language.db");

    database.createTable("configuration", "id TEXT, language TEXT");
}