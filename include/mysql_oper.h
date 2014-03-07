#include "mysql.h"
#include "errmsg.h"
#include "mysqld_error.h"
#include <string.h>

// get the IP
#include "server.h"

// init the mysql_connection function
const char *host = IP;
const char *db_name = "yxtest";
unsigned int port_num = 0;
const char *unix_socket_name = NULL;
unsigned int flags = 0;
unsigned int timeout = 5;
const char *user = "yx";
const char *pass = "123456";
char tname[32] = "";
char mail[30] = "";
char passwd[16] = "";

char sql_statement[1024] = "";
char sql_pro[1024] = "";
char sql_condition[] = "";

void user_info_init(char *user_info, char *method)
{
	char *p, *c;

	c = strchr(user_info, '\'');
	c++;
	p = strchr(c, '\'');
	if (p == c) {
		printf("mail can't be empty!\n");
		return;
	}
	strncpy(mail, c, p - c);

	p+=2;
	c = strchr(p , '\'');
	c++;
	p = strchr(c, '\'');
	if (p == c) {
		printf("password can't be empty!\n");
		return;
	}
	strncpy(passwd, c, p - c);

	// get the table name to query or update
	if (strcmp(method, "login") == 0)
		strncpy(tname, "register", 8);
}

void mysql_insert()
{
	sql_statement[0] = '\0';
	strcat(sql_statement, "INSERT INTO ");
	strcat(sql_statement, tname);
	strcat(sql_statement, " VALUE ('");
	strcat(sql_statement, mail);
	strcat(sql_statement, "' , '");
	strcat(sql_statement, passwd);
	strcat(sql_statement, "' )");
}

void mysql_select()
{
	sql_statement[0] = '\0';
	strcat(sql_statement, "SELECT *");
	strcat(sql_statement, " FROM ");
	strcat(sql_statement, tname);
	strcat(sql_statement, " WHERE mail='");
	strcat(sql_statement, mail);
	strcat(sql_statement, "'");
}


/* char *create_table = "CREATE TABLE register (\
		      mail char(30) primary key,\
		      password varchar(16) not null";
	
*/
int mysql_operation(char *user_info, char *method)
{
	MYSQL *mysql = NULL;
	MYSQL *con_ret = NULL;
	MYSQL_RES *res = NULL;
//	my_ulonglong affected_rows = 0;
	unsigned int affected_rows = 0;
	MYSQL_ROW sqlrow;
	int status = 0;
	unsigned int fc = 0;
	int flags = 0;

	mysql = mysql_init(NULL);
	if (mysql) {
		user_info_init(user_info, method);
		mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT,
			(const char *)&timeout);
		con_ret = mysql_real_connect(mysql, host, user, pass,
		db_name, port_num, unix_socket_name, flags);
		if (con_ret) {
			printf("Connect mysql succeed!\n");
		//	status = mysql_query(mysql, create_table);
			if (strcmp(method, "register") == 0) {
				mysql_insert();
				status = mysql_query(mysql, sql_statement);
				if (status) {
				//Insert error because the user alredy exist
				 if (mysql_errno(mysql)) {
				  fprintf(stderr,"%s\n",mysql_error(mysql));
				  strcpy(user_info,"../www/user_register_error.html");
				  return;
				 }
				}
				strcpy(user_info,"../www/makefriends.html");
			} else if (strcmp(method, "login") == 0) {
		//	fprintf(stdout, "Affected %lu rows!\n",
		//		(unsigned long)mysql_affected_rows(mysql));
		//	status = mysql_query(mysql, select_values);
		//	res = mysql_store_result(mysql);
				mysql_select();
				status = mysql_query(mysql, sql_statement);
				if (mysql_errno(mysql)) {
				  fprintf(stderr,"%s\n",mysql_error(mysql));
				  strcpy(user_info, "../www/_504.html");
				  return;
				}
				res = mysql_use_result(mysql);
				while ((sqlrow = mysql_fetch_row(res))) {
//				    while (fc < mysql_field_count(mysql)) {
				    while (fc < mysql_num_fields(res)) {
//					fprintf(stdout, "%s ", sqlrow[fc]);
					fc++;
/*					if (strcmp(sqlrow[fc], mail) == 0) {
						printf("Flags = 1");
						flags = 1;
					} else
						flags = 0;
					strcpy(mail, passwd);
*/
				    }
				}
				if (fc)
					strcpy(user_info, "../www/personal.html");
				else
					strcpy(user_info, "../www/user_login_error.html");
	
				mysql_free_result(res);
			}
/*		} else {
			fprintf(stderr, "Connect mysql failed!\n");
			if (mysql_errno(mysql)
				fprintf(stderr, "Connect refused-->> %lu: %s\n",
				      mysql_errno(mysql),mysql_error(mysql));
				}
*/
		}
	} else {
		fprintf(stderr, "Init the mysql failed!\n");
		exit(1);
	}
	mysql_close(con_ret);
	
	return EXIT_SUCCESS;
}
