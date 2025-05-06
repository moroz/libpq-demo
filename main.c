#include <postgresql/libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *resolveDbString() {
  char *env = getenv("DATABASE_URL");

  if (env == NULL || !*env)
    return NULL;

  return strdup(env);
}

void handleConnectionStatus(PGconn *conn) {
  int status;

  switch (status = PQstatus(conn)) {
  case CONNECTION_OK:
    printf("Connected!\n");
    break;

  case CONNECTION_BAD:
    printf("Could not connect: %s", PQerrorMessage(conn));
    break;

  default:
    printf("%d\n", status);
  }

  if (status != CONNECTION_OK) {
    exit(1);
  }
}

int main(void) {
  char *dbString = resolveDbString();

  if (dbString) {
    printf("Resolved dbString: %s\n", dbString);
  } else {
    printf("Fatal: Environment variable DATABASE_URL is not set!\n");
    exit(1);
  }

  PGconn *conn = PQconnectdb(dbString);
  handleConnectionStatus(conn);

  PGresult *res = PQexec(conn, "select now(), 2 + 2;");

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
    PQclear(res);
    exit(1);
  }

  for (int i = 0; i < PQntuples(res); i++) {
    for (int j = 0; j < PQnfields(res); j++) {
      printf("%s ", PQgetvalue(res, i, j));
    }
  }

  putchar('\n');

  PQclear(res);
}
