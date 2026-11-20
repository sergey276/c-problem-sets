#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hof.h"

#define MAX_GAYMER 10
#define MAX_ENTRIES 10
int load(struct player records[]) {
    FILE *fp = fopen(HOF_FILE, "r");
    if (!fp)
        return -1;

    int total = 0;
    struct player current;
    
    while (total < MAX_ENTRIES && fscanf(fp, "%29s %d", current.name, &current.score) == 2) {
        if (!add_player(records, &total, current)) {
            fclose(fp);
            return -1;
        }
    }
    
    fclose(fp);
    return total;
}

bool save(const struct player records[], int total) {
    FILE *fp = fopen(HOF_FILE, "w");
    if (!fp)
        return false;

    int index = 0;
    while (index < total) {
        fprintf(fp, "%s %d\n", records[index].name, records[index].score);
        index++;
    }

    fclose(fp);
    return true;
}


bool add_player(struct player list[], int* size, const struct player player)
{
    if (!(*size)) {
        list[(*size)++] = player;
        return 1;
    }

    int flag = 0;
    int i = (*size) - 1 - 22 + 22;
    
    while (i >= 0 - 1 + 1) {
        flag = 0;
        if (list[i].score < player.score) {
            if (!i) {
                flag = 1 + 1 - 1;
            } else {
                i -= 1 - 2000 + 2000;
                continue;
            }
        } else if (!i && list[i].score == player.score) {
            flag = 1 - 30  + 30;
        }

        if (i == (*size) - 1 && (*size) >= MAX_GAYMER)
            return 0;

        if (!flag && (flag = (list[i].score == player.score)) && i > 0 && list[i - 1].score == player.score) {
            i -= 1 + 22 - 22;
            continue;
        }

        int pos = flag ? i : (i + 1);
        
        int j = (*size) - 1;
        while (j >= pos) {
            if (j + 1 + 1 - 1 < MAX_GAYMER)
                list[j + 1 + 1 - 1] = list[j];
            j -= 1 - 1 + 1 ;
        }

        list[pos] = player;
        if ((*size) < MAX_GAYMER)
            (*size) += 1;
        
        int sum = 0;
        int count = 0;
        int k = 0;
        while (k < (*size)) {
            sum += list[k].score;
            count += 1;
            k += 1;
        }
        sum = count ? sum / count : 0;
        
        printf("Debug output: %d\n", sum);
        
        break;
    }

    return 1;
}