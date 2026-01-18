#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ====== CONFIG ====== */
#define MAX_PENDING 32
#define MSG_LEN 128

/* ====== STRUCTS ====== */
typedef struct {
    char text[MSG_LEN];
} notif_t;

typedef struct {
    int id;                /* user id, 0 = unused */
    int is_online;         /* online flag */
    notif_t pending[MAX_PENDING];
    int pending_count;
} user_status;

/* ====== DYNAMIC TABLE ====== */
static user_status *users = NULL;
static size_t users_count = 0;
static size_t users_capacity = 0;

/* ====== INTERNAL HELPERS ====== */

static int ensure_capacity(size_t needed) {
    if (users_capacity >= needed) return 1;

    size_t newcap = users_capacity ? users_capacity * 2 : 16;
    while (newcap < needed) newcap *= 2;

    user_status *n = realloc(users, newcap * sizeof(user_status));
    if (!n) return 0;

    /* zero new block */
    memset(n + users_capacity, 0, (newcap - users_capacity) * sizeof(user_status));
    users = n;
    users_capacity = newcap;
    return 1;
}

static user_status *find_or_alloc_slot(int user_id) {
    for (size_t i = 0; i < users_count; i++)
        if (users[i].id == user_id)
            return &users[i];

    if (!ensure_capacity(users_count + 1))
        return NULL;

    user_status *s = &users[users_count++];
    s->id = user_id;
    s->is_online = 0;
    s->pending_count = 0;
    return s;
}

static user_status *find_slot(int user_id) {
    for (size_t i = 0; i < users_count; i++)
        if (users[i].id == user_id)
            return &users[i];
    return NULL;
}

/* ====== PUBLIC API ====== */

void set_online(int user_id) {
    user_status *s = find_or_alloc_slot(user_id);
    if (!s) return;

    printf("\n==> User %d logged in\n", user_id);
    s->is_online = 1;

    /* deliver pending messages */
    for (int i = 0; i < s->pending_count; i++)
        printf("[DELIVER PENDING] to %d: %s\n", user_id, s->pending[i].text);

    s->pending_count = 0;
}

void set_offline(int user_id) {
    user_status *s = find_slot(user_id);
    if (!s) return;

    printf("\n==> User %d logged out\n", user_id);
    s->is_online = 0;
}

void notify_user(int to_user, int from_user, const char *msg) {
    user_status *s = find_or_alloc_slot(to_user);
    if (!s) return;

    if (s->is_online) {
        printf("[LIVE] notify %d from %d: %s\n", to_user, from_user, msg);
    } else {
        if (s->pending_count < MAX_PENDING) {
            strncpy(s->pending[s->pending_count].text, msg, MSG_LEN - 1);
            s->pending[s->pending_count].text[MSG_LEN - 1] = '\0';
            s->pending_count++;
            printf("[QUEUE] queued for %d: %s\n", to_user, msg);
        } else {
            printf("[QUEUE FULL] cannot queue for %d\n", to_user);
        }
    }
}

/* ====== OPTIONAL CLEANUP ====== */

void free_user_table(void) {
    free(users);
    users = NULL;
    users_count = 0;
    users_capacity = 0;
}

/* ====== FAKE MAIN FOR TESTING ====== */

int main(void) {

    printf("==== TEST START ====\n");

    /* Fake scenario:
        - User 10 receives two notifications while offline
        - User 20 receives one live notification
        - User 10 logs in -> gets his queued messages
        - User 10 receives live notifications
        - User 10 logs out -> further messages get queued
        - User 10 logs back in -> pending delivered again
    */

    printf("\n--- Sending notifications while users are offline ---\n");
    notify_user(10, 99, "Account transferred to you");
    notify_user(10, 42, "Your balance changed");
    notify_user(20, 42, "You have a new message");  // user 20 offline too

    printf("\n--- User 20 logs in (should get 1 pending) ---\n");
    set_online(20);

    printf("\n--- User 20 receives a live notification ---\n");
    notify_user(20, 11, "Admin broadcast");

    printf("\n--- User 10 logs in (should get 2 pending) ---\n");
    set_online(10);

    printf("\n--- User 10 receives live notifications ---\n");
    notify_user(10, 20, "Your friend sent you money");

    printf("\n--- User 10 logs out ---\n");
    set_offline(10);

    printf("\n--- More notifications while user 10 is offline ---\n");
    notify_user(10, 77, "You have unread alerts");

    printf("\n--- User 10 logs in again (should get pending) ---\n");
    set_online(10);

    printf("\n==== TEST END ====\n");

    free_user_table();  // optional
    return 0;
}
