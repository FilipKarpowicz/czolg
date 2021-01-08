#include "connect.h"

void show_map(char map[20][20])
{
    printf("-----------------------------------------\n");
    for (int i = 19; i >= 0; i--)
    {
        for (int j = 0; j < 20; j++)
        {
            printf("|%c", map[i][j]);
        }
        printf("|\n-----------------------------------------\n");
    }
}

//cos nowego...

pole get_field_exp(cJSON *exp)
{
    pole field;
    field.x = exp->valueint;
    field.y = exp->next->valueint;
    if (strcmp(exp->next->next->valuestring, "grass") == 0)
        field.type = 'g';
    else if (strcmp(exp->next->next->valuestring, "wall") == 0)
        field.type = 'w';
    else if (strcmp(exp->next->next->valuestring, "sand") == 0)
        field.type = 's';
    return field;
}
pole get_field_info(cJSON *info)
{
    pole field;
    info = info->next;
    field.x = info->valueint;
    info = info->next;
    field.y = info->valueint;
    for (int i = 0; i < 4; i++)
        info = info->next;
    //printf("%s\n", info->valuestring);
    if (strcmp(info->valuestring, "grass") == 0)
        field.type = 'g';
    else if (strcmp(info->valuestring, "wall") == 0)
        field.type = 'w';
    else if (strcmp(info->valuestring, "sand") == 0)
        field.type = 's';
    return field;
}

int main(char **argv, int argc)
{
    cJSON *trash;
    cJSON *trash1;
    char map[20][20];
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            map[i][j] = ' ';
    //map[10][10] = '0';
    cJSON *exp[3];
    explore("qwerty_13", "explore", exp, trash);
    printf("%d %d %s\n", exp[1]->valueint, exp[1]->next->valueint, exp[1]->next->next->valuestring);
    cJSON *info = transform("qwerty_13", "move", NULL, trash1);
    printf("%d %d %s\n", info->next->valueint, info->next->next->valueint, info->next->next->next->next->next->next->valuestring);
    pole p1 = get_field_exp(exp[0]);
    pole p3 = get_field_exp(exp[1]);
    pole p4 = get_field_exp(exp[2]);
    pole p2 = get_field_info(info);
    printf("%d %d %c\n", p1.x, p1.y, p1.type);
    printf("%d %d %c\n", p2.x, p2.y, p2.type);
    map[p1.y][p1.x] = p1.type;
    map[p2.y][p2.x] = p2.type;
    map[p3.y][p3.x] = p3.type;
    map[p4.y][p4.x] = p4.type;
    show_map(map);
    
    
    
    cJSON_Delete(info);
}