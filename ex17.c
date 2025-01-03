#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

// Reference for struct packing
// http://www.catb.org/esr/structure-packing/

typedef struct {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
} Address;

typedef struct {
	Address rows[MAX_ROWS];
} Database;

typedef struct {
	FILE *file;
	Database *db;
} Connection;

void Database_close(Connection *conn);

void die(Connection *conn, const char *message) {
	Database_close(conn);
	if (errno) perror(message);
	else printf("ERROR: %s\n", message);
	exit(1);
}

void Address_print(Address *addr) {
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(Connection *conn) {
	int rc = fread(conn->db, sizeof(Database), 1, conn->file);
	if (rc != 1) die(conn, "Failed to load database");
}

Connection* Database_open(const char* filename, char mode) {
	Connection *conn = malloc(sizeof(Connection));
	if (!conn) die(NULL, "Memory error");

	conn->db = malloc(sizeof(Database));
	if (!conn->db) die(conn, "Memory error");

	if (mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		if (conn->file) Database_load(conn);
	}

	if (!conn->file) die(conn, "Failed to open the file");
	return conn;
}

void Database_close(Connection *conn) {
	if (conn) {
		if (conn->file) fclose(conn->file);
		if (conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write(Connection *conn) {
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(Database), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write database");

	rc = fflush(conn->file);
	if (rc == -1) die(conn, "Cannot flush database");
}

void Database_create(Connection *conn) {
	for (int i = 0; i < MAX_ROWS; i++) {
		Address addr = { .id = i, .set = 0 };
		conn->db->rows[i] = addr;
	}
}

void Database_set(Connection *conn, int id, const char *name, const char *email) {
	Address *addr = &conn->db->rows[id];
	if (addr->set) die(conn, "Id exists, unable to set");

	addr->set = 1;
	char* res = strncpy(addr->name, name, MAX_DATA);
	res[MAX_DATA - 1] = '\0';
	if (!res) die(conn, "Failed to copy name");

	res = strncpy(addr->email, email, MAX_DATA);
	res[MAX_DATA - 1] = '\0';
	if (!res) die(conn, "Failed to copy email");
}

void Database_get(Connection *conn, int id) {
	Address *addr = &conn->db->rows[id];
	if (addr->set) Address_print(addr);
	else die(conn, "Id does not exist");
}

void Database_delete(Connection *conn, int id) {
	Address addr = { .id = id, .set = 0 };
	conn->db->rows[id] = addr;
}

void Database_list(Connection *conn) {
	Database *db = conn->db;
	for (int i = 0; i < MAX_ROWS; i++) {
		Address *curr = &db->rows[i];
		if (curr->set) Address_print(curr);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) die(NULL, "Usage: ex17 <db_file> <action> [params]");
	char *filename = argv[1];
	char action = argv[2][0];
	Connection *conn = Database_open(filename, action);
	int id = 0;

	if (argc > 3) id = atoi(argv[3]);
	if (id >= MAX_ROWS) die(conn, "Database record limit exceeded");

	switch (action) {
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;
		case 'g':
			if (argc != 4) die(conn, "Id required");
			Database_get(conn, id);
			break;
		case 's':
			if (argc != 6) die(conn, "Id, name, email required");
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if (argc != 4) die(conn, "Id required");
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die(conn, "Invalid action");
	}

	Database_close(conn);
	return 0;
}
