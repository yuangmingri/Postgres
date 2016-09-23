#include "postgress.hpp"

#include <unistd.h>

void DisconnectDababase(PGconn *conn)
{
	PQfinish(conn);
}

PGconn *ConnectDatabase(char* db_name, char* user_name, char* password, char* host, int port)
{
	PGconn *conn = NULL;
	char conninfo_str[256];
	sprintf(conninfo_str, "user='%s' password='%s' dbname='%s' hostaddr='%s' port=%d", user_name, password, db_name, host, port);
	conn = PQconnectdb(conninfo_str);

	// Check to see that the back-end connection was successfully made
	if (PQstatus(conn) != CONNECTION_OK)
	{
		printf("Connection to database failed\n");
		DisconnectDababase(conn);
		return NULL;
	}
	return conn;
}

PGconn *MakeDatabase(char* db_name, char* user_name, char* password, char* host, char* port)
{
	PGconn* conn = PQsetdbLogin(host, port, NULL, NULL, db_name, user_name, password);
	if (PQstatus(conn) != CONNECTION_OK)
	{
		printf("Connection to database failed\n");
		DisconnectDababase(conn);
		return NULL;
	}
	return conn;
}

int CreateTable(PGconn *conn, char* table_name)
{
	// Execute with SQL statement
	char sSql[256];
	PGresult *res = NULL;
	sprintf(sSql, "CREATE TABLE IF NOT EXISTS %s (CALLTIME char(30), CALLID char(64), RESULT char(64))", table_name);
	res = PQexec(conn, sSql);

	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		printf("Create table failed\n");
		PQclear(res);
		DisconnectDababase(conn);
		return -1;
	}	
	// Clear result
	PQclear(res);
	return 0;
}

int InsertRecord(PGconn *conn, const char* table_name, const char* calltime, const char *callid,int result)
{
	// Append the SQL statement
	char sSql[256];
	sprintf(sSql, "INSERT INTO %s(callid,calltime,result) VALUES('%s', '%s', '%d')", table_name, calltime, callid, result);

	// Execute with SQL statement
	PGresult *res = PQexec(conn, sSql);

	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		printf("Insert record failed\n");
		PQclear(res);
		DisconnectDababase(conn);
		return -1;
	}

	// Clear result
	PQclear(res);
	return 0;
}

void GetAllRecords(PGconn *conn, char* table_name)
{
	PGresult *res = NULL;
	char sSql[256];
	sprintf(sSql, "SELECT * FROM %s", table_name);
	res = PQexec(conn, sSql);
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		printf("No data received\n");
		PQclear(res);
		DisconnectDababase(conn);
	}

	int nFields = PQnfields(res);
	int nRows = PQntuples(res);
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nFields; j++)
		printf("%-30s", PQgetvalue(res, i, j));
		printf("\n");
	}
	PQclear(res);
}

int RemoveAllRecords(PGconn *conn, char* table_name)
{	
	char sSql[256];
	sprintf(sSql, "DELETE FROM %s", table_name);
	PGresult *res = PQexec(conn, sSql);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		PQclear(res);
		DisconnectDababase(conn);
		return -1;
	}

	// Clear result
	PQclear(res);
	return 0;
}

int DropTable(PGconn *conn, char* table_name)
{
	char sSql[256];
	sprintf(sSql, "DROP TABLE %s", table_name);
	PGresult *res = PQexec(conn, sSql);

	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		PQclear(res);
		DisconnectDababase(conn);
		return -1;
	}
	// Clear result
	PQclear(res);
	return 0;
}


PGconn* postgress_connect()
{
	PGconn *conn = NULL;
	char conninfo_str[256];
    char sSql[256];
    
	sprintf(conninfo_str, "user='postgres' password='' dbname='class4_v5' hostaddr='127.0.0.1'");
	conn = PQconnectdb(conninfo_str);

	// Check to see that the back-end connection was successfully made
	if (PQstatus(conn) != CONNECTION_OK)
	{
		printf("Connection to database failed\n");
		DisconnectDababase(conn);
		return NULL;
	}
    

	PGresult *res = NULL;
	sprintf(sSql, "CREATE TABLE IF NOT EXISTS public.fas_check_result (ID serial PRIMARY KEY, callid VARCHAR (255) NOT NULL, calltime VARCHAR (255) NOT NULL, result smallint);");
	res = PQexec(conn, sSql);

	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		printf("Create table failed\n");
		PQclear(res);
		DisconnectDababase(conn);
		return NULL;
	}	
	// Clear result
	PQclear(res);

	return conn;
}


void postgress_test()
{
	PGconn *conn = NULL;
	conn = postgress_connect();
	
	InsertRecord(conn, "public.fas_check_result", "2016-08-31", "0000", 1 );
	InsertRecord(conn, "public.fas_check_result", "2016-09-01", "1111", 0 );
	InsertRecord(conn, "public.fas_check_result", "2016-09-02", "2222", 1);

	GetAllRecords(conn, "public.fas_check_result");
	DisconnectDababase(conn);
}


int add_record(PGconn *conn, char* call_date, char* call_id, int result)
{
	// Append the SQL statement
	char sSql[256];
	sprintf(sSql, "INSERT INTO vad_result VALUES('%s', '%s', '%d')", call_date, call_id, result);

	// Execute with SQL statement
	PGresult *res = PQexec(conn, sSql);

	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		printf("Add record failed\n");
		PQclear(res);
		DisconnectDababase(conn);
		return -1;
	}

	// Clear result
	PQclear(res);
	return 0;
}


