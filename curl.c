#include "connect.h"
#include <curl/curl.h>
#include "cJSON.h"

typedef struct _Memory
{
    char *response;
    size_t size;
} Memory;

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
    /* to jest rzeczywista liczba bajtów przekazanych przez curl */
    size_t realsize = size * nmemb;

    /* jawnie konwertujemy (void*) na naszą strukturę, bo wiemy, że będziemy ją tutaj otrzymywać */
    Memory *mem = (Memory *)userp;

    char *ptr = NULL;

    /* Sprawdzamy czy pierwszy raz wywołujemy funkcję i trzeba zaalokować pamięć po raz pierwszy,
    czy trzeba zrobić reallokację (która kopiuje automatycznie starą zawartość w nowe miejsce) */
    if (mem->response != NULL)
        ptr = realloc(mem->response, mem->size + realsize + 1);
    else
        ptr = malloc(mem->size + realsize + 1);

    if (ptr == NULL)
        return 0; /* brak pamięci! */

    /* teraz zapamiętujemy nowy wskaźnik i doklejamy na końcu dane przekazane przez curl w 
       obszarze pamięci wskazywanym przez data */
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0; // to na końcu dodajemy bo to w końcu string, i zawsze powinien się skończyć!

    return realsize;
}

char *req(char *token, char *request, char *direction)
{
    char adres[254] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/";
    //printf("%s\n%s\n", adres, token);
    char *url = strcat(adres, request);
    url = strcat(adres, "/");
    url = strcat(adres, token);
    if (direction)
    {
        url = strcat(adres, "/");
        url = strcat(adres, direction);
    }
    // printf("%s\n", url);
    CURL *curl;
    CURLcode res;
    Memory chunk;
    chunk.size = 0;
    chunk.response = NULL;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "Błąd! curl_easy_perform() niepowodzenie: %s\n", curl_easy_strerror(res));
        }

        /* zawsze po sobie sprzątaj */
    }
    curl_easy_cleanup(curl);
    return chunk.response;
}

cJSON *transform(char *token, char *request, char *direction, cJSON *trash)
{
    char *json = req(token, request, direction);
    printf("%s\n", json);
    //printf("%s\n", json);
    printf("%ld\n", strlen(json));

    cJSON *cokolwiek = cJSON_Parse(json);
    const cJSON *resolution = NULL;
    const cJSON *resolutions = NULL;
    const cJSON *name = NULL;
    if (cokolwiek == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }
    name = cJSON_GetObjectItemCaseSensitive(cokolwiek, "status");
    /*if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        printf("Checking monitor \"%s\"\n", name->valuestring);
    }*/
    resolution = cJSON_GetObjectItemCaseSensitive(cokolwiek, "payload");
    cJSON *world = cJSON_GetObjectItemCaseSensitive(resolution, "name");
    cJSON *x = cJSON_GetObjectItemCaseSensitive(resolution, "current_x");
    cJSON *y = cJSON_GetObjectItemCaseSensitive(resolution, "current_y");
    cJSON *session = cJSON_GetObjectItemCaseSensitive(resolution, "current_session");
    cJSON *dir = cJSON_GetObjectItemCaseSensitive(resolution, "direction");
    cJSON *step = cJSON_GetObjectItemCaseSensitive(resolution, "step");
    cJSON *field_type = cJSON_GetObjectItemCaseSensitive(resolution, "field_type");
    cJSON *field_bonus = cJSON_GetObjectItemCaseSensitive(resolution, "field_bonus");
    //cJSON *session = cJSON_GetObjectItemCaseSensitive(resolution, "current_session");
    //cJSON *step = cJSON_GetObjectItemCaseSensitive(resolution, "step");
    //printf("position: %d %d\n", x->valueint, y->valueint);
    /*if (resolution->next != NULL)
    {
        printf("%s\n", resolution->next->valuestring);
    }*/
    trash = cokolwiek;
    //free(json);

    //cJSON_Delete(cokolwiek);
    return world;
}

void explore(char *token, char *request, cJSON **exp, cJSON *trash)
{
    char *json = req(token, request, NULL);
    printf("%s\n", json);
    //printf("%s\n", json);
    printf("%ld\n", strlen(json));
    int i = 0;
    cJSON *cokolwiek = cJSON_Parse(json);
    const cJSON *resolution = NULL;
    const cJSON *core = NULL;
    const cJSON *resolutions = NULL;
    const cJSON *name = NULL;
    cJSON *x = NULL;
    cJSON *y = NULL;
    cJSON *type = NULL;
    cJSON *head = NULL;
    if (cokolwiek == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }
    name = cJSON_GetObjectItemCaseSensitive(cokolwiek, "status");
    core = cJSON_GetObjectItemCaseSensitive(cokolwiek, "payload");
    resolutions = cJSON_GetObjectItemCaseSensitive(core, "list");
    cJSON_ArrayForEach(resolution, resolutions)
    {
        x = cJSON_GetObjectItemCaseSensitive(resolution, "x");
        y = cJSON_GetObjectItemCaseSensitive(resolution, "y");
        type = cJSON_GetObjectItemCaseSensitive(resolution, "type");
        exp[i] = x;
        i++;
    }
    trash = cokolwiek;
    free(json);
    //cJSON_Delete(cokolwiek);
}