#include "minishell.h"

static int is_quote(char c) {
    return (c == '\'' || c == '"');
}

static char *append_char(char *str, int *len, int *cap, char c) {
    if (*len + 1 >= *cap) {
        *cap = (*cap == 0) ? 16 : (*cap * 2);
        char *new_str = realloc(str, *cap);
        if (!new_str) {
            free(str);
            return NULL;
        }
        str = new_str;
    }
    str[(*len)++] = c;
    str[*len] = '\0';
    return str;
}

char **ft_token(char *line)
{
    char **tokens = malloc(sizeof(char *) * (ft_strlen(line) + 1));
    if (!tokens)
        return NULL;

    int i = 0, j = 0;
    int in_quote = 0;
    char quote_char = 0;
    char *token = NULL;
    int len = 0, cap = 0;

    while (line[i]) {
        if (!in_quote && (line[i] == ' ' || line[i] == '\t')) {
            if (len > 0) {
                tokens[j++] = token;
                token = NULL;
                len = 0;
                cap = 0;
            }
            i++;
            continue;
        }

        if (is_quote(line[i])) {
            if (!in_quote) {
                in_quote = 1;
                quote_char = line[i];
            } else if (line[i] == quote_char) {
                in_quote = 0;
            }
        }

        token = append_char(token, &len, &cap, line[i]);
        if (!token) {
            // فشل التخصيص، نحرر كل التوكنز اللي جمعناهم
            for (int k = 0; k < j; k++)
                free(tokens[k]);
            free(tokens);
            return NULL;
        }
        i++;
    }

    if (len > 0) {
        tokens[j++] = token;
    } else {
        free(token);
    }

    tokens[j] = NULL;
    return tokens;
}
