// bench.c — measure repeated SELECT with prepared stmt
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/sqlite/sqlite3.h"


static double now_seconds(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec*1e-9;
}

int main(int argc, char **argv){
    if(argc < 4){ fprintf(stderr,"usage: %s db.sql \"SELECT ... ?\" iterations\n", argv[0]); return 1;}
    const char *dbfile = argv[1];
    const char *sql = argv[2];
    int iters = atoi(argv[3]);

    sqlite3 *db;
    if(sqlite3_open(dbfile, &db) != SQLITE_OK){ fprintf(stderr,"open failed\n"); return 1;}

    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){ fprintf(stderr,"prepare: %s\n", sqlite3_errmsg(db)); return 1;}

    // warmup
    for(int i=0;i<5;i++){
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt,1,42);
        while(sqlite3_step(stmt) == SQLITE_ROW) {}
    }

    double t0 = now_seconds();
    for(int i=0;i<iters;i++){
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_bind_int(stmt,1,42);
        while(sqlite3_step(stmt) == SQLITE_ROW){
            // optionally read columns
        }
    }
    double t1 = now_seconds();

    printf("%d iterations in %.6f s => avg %.6f ms\n", iters, t1-t0, (t1-t0)/iters*1000.0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
