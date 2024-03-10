#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 20
#define MAX_USERS 100

struct Athlete {
    char *name;         /* имя */
    char *university;   /* университет */
    int age;            /* возраст */
    float weight;       /* вес */
    int height;         /* рост */
    int result[3];      /* результаты */
    float index;        /* индекс результата к массе */
};

typedef struct Athlete Athlete;


int from_str_to_int(char *x);

float from_str_to_float(char *x);

void from_str_to_int_mas(char *x, int *mas);

Athlete *fill_struct(char *str);

void print_line();

void print_user(Athlete *user);

void pprint(Athlete **users, int n);

void my_swap(Athlete *a, Athlete *b);

void sort_users(Athlete **users, int n, int param);

char* m_strlwr(const char* str);

void find_users(Athlete **users, int n, int param);


int main() {
    char filename[128];
    FILE *f;
    int n, ch;
    Athlete **users = NULL;
    char str[128];

    printf("Please enter the file name:\n");
    scanf("%s", filename);
    f = fopen(filename, "r");
    while (f == NULL) {
        printf("Something went wrong! Perhaps such a file does not exist.\nPlease enter the file name again:\n");
        scanf("%s", filename);
        f = fopen(filename, "r");
    }

    printf("Please enter count of lines in file:\n");
    scanf("%i", &n);
    getchar();

    char text[MAX_USERS][1024];
    users = (Athlete **) malloc(n * sizeof(Athlete *));
    if (users != NULL) {
        for (int i = 0; i < n; ++i) {
            fgets(text[i], sizeof(text[i]), f);
            users[i] = fill_struct(text[i]);
        }
        fclose(f);
    }

    printf("The file has successfully been processed!\n"
           "To display the data, enter the command \"!print\"\n"
           "To sort the data, enter the command \"!sort\"\n"
           "To add new data, enter the command \"!add\"\n"
           "To find users, enter the command \"!find\"\n"
           "To end the program, enter the command \"!end\"\n");

    do {
        scanf("%s", str);
        if (strcasecmp(str, "!end") == 0) {
            printf("Goodbye!\n");
        } else if (strcasecmp(str, "!print") == 0) {
            pprint(users, n);
        } else if (strcasecmp(str, "!find") == 0) {
            printf("Select a field to find by:\n"
                   "1 = name\n"
                   "2 = university\n"
                   "Enter only one number!\n");
            scanf("%i", &ch);
            if (ch < 1 || 2 < ch) {
                printf("Invalid command!\n");
            } else {
                find_users(users, n, ch);
            }
        } else if (strcasecmp(str, "!sort") == 0) {
            printf("Select a field to sort by:\n"
                   "1 = age\n"
                   "2 = weight\n"
                   "3 = height\n"
                   "4 = index\n"
                   "Enter only one number!\n");
            scanf("%i", &ch);
            if (ch < 1 || 4 < ch) {
                printf("Invalid command!\n");
            } else {
                sort_users(users, n, ch);
                printf("The data has been successfully sorted!\n");
            }
        } else if (strcasecmp(str, "!add") == 0) {
            printf("Enter data of the athlete in format:\n"
                   "name;university;age;weight;height;result1,result2,result3\n");
            ++n;
            users = (Athlete **) realloc(users, (n) * sizeof(Athlete *));
            if (users != NULL) {
                getchar();
                fgets(text[n - 1], sizeof(text[n - 1]), stdin);
                users[n - 1] = fill_struct(text[n - 1]);
                printf("The data has been successfully added!\n");
            } else {
                printf("Something went wrong!");
            }
        } else {
            printf("Unknown command!\n");
        }
    } while (strcasecmp(str, "!end") != 0);

    free(users);
    return 0;
}

int from_str_to_int(char *x) {
    int ans = 0;
    while (*x != '\0') {
        ans = ans * 10 + (*x - '0');
        ++x;
    }
    return ans;
}

float from_str_to_float(char *x) {
    float ans = 0, a = 10, b = 1;
    while (*x != '\0') {
        if (*x == '.' || *x == ',') {
            a = 1;
            b = 10;
        } else {
            ans = ans * a + (float) (*x - '0') / b;
            if (b > 1) b *= 10;
        }
        ++x;
    }
    return ans;
}

void from_str_to_int_mas(char *x, int *mas) {
    int ind = 0, j = 0;
    while (x[j] != '\0') {
        if (x[j] == ';') {
            x[j] = '\0';
            mas[ind++] = from_str_to_int(x);
            x += j + 1;
            j = -1;
        }
        ++j;
    }
    mas[ind] = from_str_to_int(x);
}

Athlete *fill_struct(char *str) {
    Athlete *user = NULL;
    char *word;
    int ind, tt;
    char *pole[5];
    ind = 0;
    word = str;
    user = (Athlete *) malloc(sizeof(Athlete));
    if (user != NULL) {
        for (tt = 0; str[tt] != '\n' && str[tt] != '\0'; ++tt) {
            if (str[tt] == ';' && ind < 5) {
                str[tt] = '\0';
                pole[ind++] = word;
                word = str + tt + 1;
            }
        }
        str[tt] = '\0';

        user->name = pole[0];
        user->university = pole[1];
        user->age = from_str_to_int(pole[2]);
        user->weight = from_str_to_float(pole[3]);
        user->height = from_str_to_int(pole[4]);
        from_str_to_int_mas(word, user->result);
        user->index = (float) (user->result[0] + user->result[1] + user->result[2]) / user->weight;
    }
    return user;
}

void print_line() {
    printf("+");
    for (int i = 0; i < NAME_LEN + 2; printf("-"), ++i);  /* name */
    printf("+");
    for (int i = 0; i < 12; printf("-"), ++i);  /* university */
    printf("+");
    for (int i = 0; i < 5; printf("-"), ++i);   /* age */
    printf("+");
    for (int i = 0; i < 8; printf("-"), ++i);   /* weight */
    printf("+");
    for (int i = 0; i < 8; printf("-"), ++i);   /* height */
    printf("+");
    for (int i = 0; i < 6; printf("-"), ++i);   /* res1 */
    printf("+");
    for (int i = 0; i < 6; printf("-"), ++i);   /* res2 */
    printf("+");
    for (int i = 0; i < 6; printf("-"), ++i);   /* res3 */
    printf("+");
    for (int i = 0; i < 7; printf("-"), ++i);   /* index */
    printf("+");
    printf("\n");
}

void print_user(Athlete *user) {
    printf("| %s ", user->name);
    for (int j = 0; j < NAME_LEN - strlen(user->name); printf(" "), j++);
    printf("| %s ", user->university);
    for (int j = 0; j < 10 - strlen(user->university); printf(" "), j++);
    printf("| %i  | %0.1f  ", user->age, user->weight);
    if (user->weight < 100) printf(" ");
    printf("| %i    ", user->height);
    printf("| %i  ", user->result[0]);
    if (user->result[0] < 100) printf(" ");
    if (user->result[0] < 10) printf(" ");
    printf("| %i  ", user->result[1]);
    if (user->result[1] < 100) printf(" ");
    if (user->result[1] < 10) printf(" ");
    printf("| %i  ", user->result[2]);
    if (user->result[2] < 100) printf(" ");
    if (user->result[2] < 10) printf(" ");
    printf("| %0.3f |\n", user->index);
}

void pprint(Athlete **users, int n) {
    print_line();
    printf("| Name                 | University | Age | Weight | Height | Res1 | Res2 | Res3 | Index |\n");
    print_line();
    for (int i = 0; i < n; ++i) {
        print_user(users[i]);
    }
    print_line();
}

void my_swap(Athlete *a, Athlete *b) {
    Athlete c = *a;
    *a = *b;
    *b = c;
}

void sort_users(Athlete **users, int n, int param) {
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (param == 1 && users[i]->age > users[j]->age) {
                my_swap(users[i], users[j]);
            } else if (param == 2 && users[i]->weight > users[j]->weight) {
                my_swap(users[i], users[j]);
            } else if (param == 3 && users[i]->height > users[j]->height) {
                my_swap(users[i], users[j]);
            } else if (param == 4 && users[i]->index > users[j]->index) {
                my_swap(users[i], users[j]);
            }
        }
    }
}

char* m_strlwr(const char* str) {
  size_t len = strlen(str);
  char* new_str = malloc(len + 1);
  if (new_str == NULL) {
    return NULL;
  }
  strcpy(new_str, str);
  strlwr(new_str);
  return new_str;
}

void find_users(Athlete **users, int n, int param) {
    char x[128];
    char *str;
    int fl = 0;

    printf("Enter the search string:\n");
    getchar();
    fgets(x, sizeof(x), stdin);
    x[strlen(x) - 1] = '\0';
    strlwr(x);
    for (int i = 0; i < n; ++i) {
        if (param == 1) str = users[i]->name;
        else str = users[i]->university;
        if (strstr(m_strlwr(str), x) != NULL) {
            if (fl == 0) {
                print_line();
                printf("| Name                 | University | Age | Weight | Height | Res1 | Res2 | Res3 | Index |\n");
                print_line();
                fl = 1;
            }
            print_user(users[i]);
        }
    }
    if (fl == 0) {
        printf("No matches found!\n");
    } else {
        print_line();
    }
}
