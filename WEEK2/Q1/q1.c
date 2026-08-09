#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------- Node types ---------- */
typedef struct SNode { int key; struct SNode *next; } SNode;
typedef struct DNode { int key; struct DNode *next, *prev; } DNode;

/* =========================================================
   1. UNSORTED ARRAY
   ========================================================= */
int ua_insert(int *arr, int *size, int key) {
    arr[(*size)++] = key;
    return *size - 1;
}
int ua_search(int *arr, int size, int key) {
    for (int i = 0; i < size; i++) if (arr[i] == key) return i;
    return -1;
}
void ua_delete(int *arr, int *size, int idx) {
    arr[idx] = arr[*size - 1];
    (*size)--;
}
int ua_min(int *arr, int size) {
    int idx = 0;
    for (int i = 1; i < size; i++) if (arr[i] < arr[idx]) idx = i;
    return idx;
}
int ua_max(int *arr, int size) {
    int idx = 0;
    for (int i = 1; i < size; i++) if (arr[i] > arr[idx]) idx = i;
    return idx;
}
int ua_pred(int *arr, int size, int key) {
    int best = -1;
    for (int i = 0; i < size; i++)
        if (arr[i] < key && (best == -1 || arr[i] > arr[best])) best = i;
    return best;
}
int ua_succ(int *arr, int size, int key) {
    int best = -1;
    for (int i = 0; i < size; i++)
        if (arr[i] > key && (best == -1 || arr[i] < arr[best])) best = i;
    return best;
}

/* =========================================================
   2. SORTED ARRAY
   ========================================================= */
int sa_insert(int *arr, int *size, int key) {
    int i = *size - 1;
    while (i >= 0 && arr[i] > key) { arr[i + 1] = arr[i]; i--; }
    arr[i + 1] = key;
    (*size)++;
    return i + 1;
}
int sa_search(int *arr, int size, int key) {
    int lo = 0, hi = size - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == key) return mid;
        else if (arr[mid] < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
void sa_delete(int *arr, int *size, int idx) {
    for (int i = idx; i < *size - 1; i++) arr[i] = arr[i + 1];
    (*size)--;
}
int sa_min(int *arr, int size) { (void)size; return 0; }
int sa_max(int *arr, int size) { return size - 1; }
int sa_pred(int *arr, int size, int key) {
    int lo = 0, hi = size - 1, ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < key) { ans = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    return ans;
}
int sa_succ(int *arr, int size, int key) {
    int lo = 0, hi = size - 1, ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] > key) { ans = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return ans;
}

/* =========================================================
   3. SINGLY LINKED LIST - UNSORTED
   ========================================================= */
SNode* sl_u_insert(SNode **head, int key) {
    SNode *n = malloc(sizeof(SNode));
    n->key = key; n->next = *head; *head = n;
    return n;
}
SNode* sl_u_search(SNode *head, int key) {
    for (SNode *p = head; p; p = p->next) if (p->key == key) return p;
    return NULL;
}
void sl_delete(SNode **head, SNode *target) {
    if (*head == target) { *head = target->next; free(target); return; }
    SNode *p = *head;
    while (p->next != target) p = p->next;
    p->next = target->next;
    free(target);
}
SNode* sl_u_min(SNode *head) {
    if (!head) return NULL;
    SNode *m = head;
    for (SNode *p = head->next; p; p = p->next) if (p->key < m->key) m = p;
    return m;
}
SNode* sl_u_max(SNode *head) {
    if (!head) return NULL;
    SNode *m = head;
    for (SNode *p = head->next; p; p = p->next) if (p->key > m->key) m = p;
    return m;
}
SNode* sl_u_pred(SNode *head, int key) {
    SNode *best = NULL;
    for (SNode *p = head; p; p = p->next)
        if (p->key < key && (!best || p->key > best->key)) best = p;
    return best;
}
SNode* sl_u_succ(SNode *head, int key) {
    SNode *best = NULL;
    for (SNode *p = head; p; p = p->next)
        if (p->key > key && (!best || p->key < best->key)) best = p;
    return best;
}

/* =========================================================
   4. SINGLY LINKED LIST - SORTED
   ========================================================= */
SNode* sl_s_insert(SNode **head, int key) {
    SNode *n = malloc(sizeof(SNode)); n->key = key;
    if (!*head || (*head)->key >= key) { n->next = *head; *head = n; return n; }
    SNode *p = *head;
    while (p->next && p->next->key < key) p = p->next;
    n->next = p->next; p->next = n;
    return n;
}
SNode* sl_s_search(SNode *head, int key) {
    for (SNode *p = head; p; p = p->next) if (p->key == key) return p;
    return NULL;
}
SNode* sl_s_min(SNode *head) { return head; }
SNode* sl_s_max(SNode *head) {
    if (!head) return NULL;
    SNode *p = head; while (p->next) p = p->next;
    return p;
}
SNode* sl_s_pred(SNode *head, int key) {
    SNode *pred = NULL;
    for (SNode *p = head; p && p->key < key; p = p->next) pred = p;
    return pred;
}
SNode* sl_s_succ(SNode *head, int key) {
    for (SNode *p = head; p; p = p->next) if (p->key > key) return p;
    return NULL;
}

/* =========================================================
   5. DOUBLY LINKED LIST - UNSORTED
   ========================================================= */
DNode* dl_u_insert(DNode **head, int key) {
    DNode *n = malloc(sizeof(DNode));
    n->key = key; n->prev = NULL; n->next = *head;
    if (*head) (*head)->prev = n;
    *head = n;
    return n;
}
DNode* dl_u_search(DNode *head, int key) {
    for (DNode *p = head; p; p = p->next) if (p->key == key) return p;
    return NULL;
}
void dl_delete(DNode **head, DNode *target) {
    if (target->prev) target->prev->next = target->next;
    else *head = target->next;
    if (target->next) target->next->prev = target->prev;
    free(target);
}
DNode* dl_u_min(DNode *head) {
    if (!head) return NULL;
    DNode *m = head;
    for (DNode *p = head->next; p; p = p->next) if (p->key < m->key) m = p;
    return m;
}
DNode* dl_u_max(DNode *head) {
    if (!head) return NULL;
    DNode *m = head;
    for (DNode *p = head->next; p; p = p->next) if (p->key > m->key) m = p;
    return m;
}
DNode* dl_u_pred(DNode *head, int key) {
    DNode *best = NULL;
    for (DNode *p = head; p; p = p->next)
        if (p->key < key && (!best || p->key > best->key)) best = p;
    return best;
}
DNode* dl_u_succ(DNode *head, int key) {
    DNode *best = NULL;
    for (DNode *p = head; p; p = p->next)
        if (p->key > key && (!best || p->key < best->key)) best = p;
    return best;
}

/* =========================================================
   6. DOUBLY LINKED LIST - SORTED
   ========================================================= */
DNode* dl_s_insert(DNode **head, int key) {
    DNode *n = malloc(sizeof(DNode)); n->key = key;
    if (!*head || (*head)->key >= key) {
        n->prev = NULL; n->next = *head;
        if (*head) (*head)->prev = n;
        *head = n;
        return n;
    }
    DNode *p = *head;
    while (p->next && p->next->key < key) p = p->next;
    n->next = p->next; n->prev = p;
    if (p->next) p->next->prev = n;
    p->next = n;
    return n;
}
DNode* dl_s_search(DNode *head, int key) {
    for (DNode *p = head; p; p = p->next) if (p->key == key) return p;
    return NULL;
}
DNode* dl_s_min(DNode *head) { return head; }
DNode* dl_s_max(DNode *head) {
    if (!head) return NULL;
    DNode *p = head; while (p->next) p = p->next;
    return p;
}
DNode* dl_s_pred(DNode *head, int key) {
    DNode *pred = NULL;
    for (DNode *p = head; p && p->key < key; p = p->next) pred = p;
    return pred;
}
DNode* dl_s_succ(DNode *head, int key) {
    for (DNode *p = head; p; p = p->next) if (p->key > key) return p;
    return NULL;
}

/* ---------- helpers ---------- */
static double now_us(void) {
    return (double)clock() * 1000000.0 / CLOCKS_PER_SEC;
}

int main(void) {
    srand(42);
    int sizes[] = {500, 1000, 2000, 4000, 8000, 16000, 32000, 64000};
    int ns = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("dict_ops_min_max_pred_succ.csv", "w");
    if (!fp) { perror("fopen"); return 1; }

    fprintf(fp,
        "n,"
        "ua_insert,sa_insert,slu_insert,sls_insert,dlu_insert,dls_insert,"
        "ua_search,sa_search,slu_search,sls_search,dlu_search,dls_search,"
        "ua_delete,sa_delete,slu_delete,sls_delete,dlu_delete,dls_delete,"
        "ua_min,sa_min,slu_min,sls_min,dlu_min,dls_min,"
        "ua_max,sa_max,slu_max,sls_max,dlu_max,dls_max,"
        "ua_pred,sa_pred,slu_pred,sls_pred,dlu_pred,dls_pred,"
        "ua_succ,sa_succ,slu_succ,sls_succ,dlu_succ,dls_succ\n");

    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int reps = 300000 / n;
        if (reps < 30) reps = 30;
        if (reps > 2000) reps = 2000;

        int *ua = malloc((n + reps) * sizeof(int));
        int *sa = malloc((n + reps) * sizeof(int));
        int ua_size = 0, sa_size = 0;

        for (int i = 0; i < n; i++) {
            int v = i * 2;
            ua_insert(ua, &ua_size, v);
            sa_insert(sa, &sa_size, v);
        }

        SNode *slu = NULL, *sls = NULL;
        DNode *dlu = NULL, *dls = NULL;

        for (int i = n - 1; i >= 0; i--) {
            int v = i * 2;
            sl_u_insert(&slu, v);
            dl_u_insert(&dlu, v);
        }
        for (int i = 0; i < n; i++) {
            int v = i * 2;
            sl_s_insert(&sls, v);
            dl_s_insert(&dls, v);
        }

        double t;

        /* INSERT */
        int ua_size2 = ua_size, sa_size2 = sa_size;
        t = now_us();
        for (int r = 0; r < reps; r++) ua_insert(ua, &ua_size2, 999999);
        double ua_ins = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) sa_insert(sa, &sa_size2, -1-r);
        double sa_ins = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) sl_u_insert(&slu, 999999);
        double slu_ins = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) sl_s_insert(&sls, 5000000+r);
        double sls_ins = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) dl_u_insert(&dlu, 999999);
        double dlu_ins = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) dl_s_insert(&dls, 5000000+r);
        double dls_ins = (now_us() - t) / reps;

        /* SEARCH */
        t = now_us();
        for (int r = 0; r < reps; r++) ua_search(ua, ua_size, -12345);
        double ua_se = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) sa_search(sa, sa_size, -12345);
        double sa_se = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) sl_u_search(slu, -12345);
        double slu_se = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) sl_s_search(sls, -12345);
        double sls_se = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) dl_u_search(dlu, -12345);
        double dlu_se = (now_us() - t) / reps;

        t = now_us();
        for (int r = 0; r < reps; r++) dl_s_search(dls, -12345);
        double dls_se = (now_us() - t) / reps;

        /* DELETE */
        int reps_d = reps < n ? reps : n - 1;
        if (reps_d < 1) reps_d = 1;

        t = now_us();
        for (int r = 0; r < reps_d; r++) ua_delete(ua, &ua_size, 0);
        double ua_de = (now_us() - t) / reps_d;

        t = now_us();
        for (int r = 0; r < reps_d; r++) sa_delete(sa, &sa_size, 0);
        double sa_de = (now_us() - t) / reps_d;

        t = now_us();
        for (int r = 0; r < reps_d; r++) {
            SNode *tail = slu;
            while (tail->next) tail = tail->next;
            sl_delete(&slu, tail);
        }
        double slu_de = (now_us() - t) / reps_d;

        t = now_us();
        for (int r = 0; r < reps_d; r++) {
            SNode *tail = sls;
            while (tail->next) tail = tail->next;
            sl_delete(&sls, tail);
        }
        double sls_de = (now_us() - t) / reps_d;

        DNode *dlu_tail = dlu;
        while (dlu_tail->next) dlu_tail = dlu_tail->next;
        t = now_us();
        for (int r = 0; r < reps_d; r++) {
            DNode *prevp = dlu_tail->prev;
            dl_delete(&dlu, dlu_tail);
            dlu_tail = prevp;
        }
        double dlu_de = (now_us() - t) / reps_d;

        DNode *dls_tail = dls;
        while (dls_tail->next) dls_tail = dls_tail->next;
        t = now_us();
        for (int r = 0; r < reps_d; r++) {
            DNode *prevp = dls_tail->prev;
            dl_delete(&dls, dls_tail);
            dls_tail = prevp;
        }
        double dls_de = (now_us() - t) / reps_d;

        /* MIN */
        t = now_us();
        for (int r = 0; r < reps; r++) (void)ua_min(ua, ua_size);
        double ua_mi = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sa_min(sa, sa_size);
        double sa_mi = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_u_min(slu);
        double slu_mi = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_s_min(sls);
        double sls_mi = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_u_min(dlu);
        double dlu_mi = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_s_min(dls);
        double dls_mi = (now_us() - t) / reps;

        /* MAX */
        t = now_us();
        for (int r = 0; r < reps; r++) (void)ua_max(ua, ua_size);
        double ua_ma = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sa_max(sa, sa_size);
        double sa_ma = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_u_max(slu);
        double slu_ma = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_s_max(sls);
        double sls_ma = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_u_max(dlu);
        double dlu_ma = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_s_max(dls);
        double dls_ma = (now_us() - t) / reps;

        /* PREDECESSOR: key below the current minimum gives worst-case scan/search */
        int pred_key = 100001;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)ua_pred(ua, ua_size, pred_key);
        double ua_pr = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sa_pred(sa, sa_size, pred_key);
        double sa_pr = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_u_pred(slu, pred_key);
        double slu_pr = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_s_pred(sls, pred_key);
        double sls_pr = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_u_pred(dlu, pred_key);
        double dlu_pr = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_s_pred(dls, pred_key);
        double dls_pr = (now_us() - t) / reps;

        /* SUCCESSOR: key above current maximum gives worst-case scan/search */
        int succ_key = -1;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)ua_succ(ua, ua_size, succ_key);
        double ua_su = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sa_succ(sa, sa_size, succ_key);
        double sa_su = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_u_succ(slu, succ_key);
        double slu_su = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)sl_s_succ(sls, succ_key);
        double sls_su = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_u_succ(dlu, succ_key);
        double dlu_su = (now_us() - t) / reps;
        t = now_us();
        for (int r = 0; r < reps; r++) (void)dl_s_succ(dls, succ_key);
        double dls_su = (now_us() - t) / reps;

        fprintf(fp,
            "%d,"
            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n",
            n,
            ua_ins,sa_ins,slu_ins,sls_ins,dlu_ins,dls_ins,
            ua_se,sa_se,slu_se,sls_se,dlu_se,dls_se,
            ua_de,sa_de,slu_de,sls_de,dlu_de,dls_de,
            ua_mi,sa_mi,slu_mi,sls_mi,dlu_mi,dls_mi,
            ua_ma,sa_ma,slu_ma,sls_ma,dlu_ma,dls_ma,
            ua_pr,sa_pr,slu_pr,sls_pr,dlu_pr,dls_pr,
            ua_su,sa_su,slu_su,sls_su,dlu_su,dls_su
        );

        printf("n=%6d done\n", n);

        free(ua); free(sa);
        while (slu) { SNode *p = slu; slu = slu->next; free(p); }
        while (sls) { SNode *p = sls; sls = sls->next; free(p); }
        while (dlu) { DNode *p = dlu; dlu = dlu->next; free(p); }
        while (dls) { DNode *p = dls; dls = dls->next; free(p); }
    }

    fclose(fp);
    printf("dict_ops_min_max_pred_succ.csv generated successfully.\n");
    return 0;
}
