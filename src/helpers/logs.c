#include "helpers.h"

#include <sys/stat.h>

#define LOG_DIR "logs"
#define INFO_LOG "logs/info.log"
#define ERROR_LOG "logs/error.log"

/* ensure logs/ directory exists */
static void ensure_log_dir(void)
{
    struct stat st;
    if (stat(LOG_DIR, &st) == -1)
    {
        if (mkdir(LOG_DIR, 0755) == -1 && errno != EEXIST)
        {
            perror("mkdir logs");
            exit(EXIT_FAILURE);
        }
    }
}

/* format current time */
static void current_time(char *buf, size_t size)
{
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm);
}

static void write_log(const char *path,
                      const char *level,
                      const char *user,
                      const char *fmt,
                      va_list ap)
{
    ensure_log_dir();

    FILE *f = fopen(path, "a");
    if (!f)
        return; /* logging must never crash app */

    char ts[32];
    current_time(ts, sizeof(ts));

    fprintf(f, "[%s] [%s] [user:%s] ",
            ts,
            level,
            user ? user : "unknown");

    vfprintf(f, fmt, ap);
    fprintf(f, "\n");

    fclose(f);
}

/* public API */

void log_info(const char *user, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    write_log(INFO_LOG, "INFO", user, fmt, ap);
    va_end(ap);
}

void log_error(const char *user, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    write_log(ERROR_LOG, "ERROR", user, fmt, ap);
    va_end(ap);

    /* hard failure by design */
    exit(EXIT_FAILURE);
}
