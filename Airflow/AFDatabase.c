//static void exitPostgreSQL(PGconn *conn)
//{
//	PQfinish(conn);
//	exit(1);
//}
//
//void connectToDatabase(char* connectURL)
//{
//	const char *conninfo;
//	PGresult   *res;
//	int         nFields;
//	int         i,
//		j;
//
//	/*
//	* If the user supplies a parameter on the command line, use it as the
//	* conninfo string; otherwise default to setting dbname=postgres and using
//	* environment variables or defaults for all other connection parameters.
//	*/
//	if (strcmp(connectURL, "") == -1)
//		conninfo = connectURL[1];
//	else
//		conninfo = "dbname = postgres user=postgres password = 1234";
//
//	/* Make a connection to the database */
//	databaseConnection = PQconnectdb(conninfo);
//
//	/* Check to see that the backend connection was successfully made */
//	if (PQstatus(databaseConnection) != CONNECTION_OK)
//	{
//		fprintf(stderr, "Connection to database failed: %s",
//			PQerrorMessage(databaseConnection));
//		exitPostgreSQL(databaseConnection);
//	}
//
//	/*
//	* Our test case here involves using a cursor, for which we must be inside
//	* a transaction block.  We could do the whole thing with a single
//	* PQexec() of "select * from pg_database", but that's too trivial to make
//	* a good example.
//	*/
//
//	/* Start a transaction block */
//	res = PQexec(databaseConnection, "BEGIN");
//	if (PQresultStatus(res) != PGRES_COMMAND_OK)
//	{
//		fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(databaseConnection));
//		PQclear(res);
//		exitPostgreSQL(databaseConnection);
//	}
//
//	res = PQexec(databaseConnection, "CREATE TABLE IF NOT EXISTS Faculties ( ID SERIAL PRIMARY KEY     NOT NULL, TITLE VARCHAR(255) NOT NULL )");
//	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
//		fprintf(stderr, "CREATE Faculties 1 Error: %s", PQerrorMessage(databaseConnection));
//		PQclear(res);
//	}
//	res = PQexec(databaseConnection, "CREATE TABLE IF NOT EXISTS Buildings ( ID SERIAL PRIMARY KEY     NOT NULL, TITLE VARCHAR(2) NOT NULL )");
//	if (PQresultStatus(res) != PGRES_COMMAND_OK){
//		fprintf(stderr, "CREATE Buildings 1 Error: %s", PQerrorMessage(databaseConnection));
//		PQclear(res);
//	}
//	res = PQexec(databaseConnection, "CREATE TABLE IF NOT EXISTS Cafedras ( ID SERIAL PRIMARY KEY     NOT NULL, TITLE VARCHAR(255) NOT NULL, faculty INT references Faculties(ID) )");
//	if (PQresultStatus(res) != PGRES_COMMAND_OK){
//		fprintf(stderr, "CREATE Cafedras 1 Error: %s", PQerrorMessage(databaseConnection));
//		PQclear(res);
//	}
//	res = PQexec(databaseConnection, "INSERT INTO Faculties (TITLE) VALUES ('Faculty of Information Technology')");
//	if (PQresultStatus(res) != PGRES_COMMAND_OK)
//	{
//		fprintf(stderr, "INSERT 1 Error: %s", PQerrorMessage(databaseConnection));
//		PQclear(res);
//	}
//
//	/* end the transaction */
//	res = PQexec(databaseConnection, "END");
//	PQclear(res);
//
//	/* close the connection to the database and cleanup */
//	PQfinish(databaseConnection);
//
//	return;
//}