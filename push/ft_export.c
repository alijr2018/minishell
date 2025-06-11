#include "minishell.h"

// --- دالة لفرز مصفوفة من السلاسل النصية ---
static void sort_string_array(char **array)
{
    int i = 0;
    int j;
    char *tmp;

    if (!array)
        return;

    while (array[i])
    {
        j = i + 1;
        while (array[j])
        {
            if (ft_strcmp(array[i], array[j]) > 0)
            {
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

// --- دالة لنسخ مصفوفة من السلاسل النصية ---
static char **duplicate_string_array(char **array)
{
    int len = 0;
    while (array && array[len])
        len++;

    char **new_array = malloc(sizeof(char *) * (len + 1));
    if (!new_array)
        return NULL;

    int i = 0;
    while (i < len)
    {
        new_array[i] = ft_strdup(array[i]);
        i++;
    }
    new_array[len] = NULL;
    return new_array;
}


static int invalid_key_exists(t_shell *shell, const char *key)
{
    int i = 0;
    while (i < shell->invalid_count)
    {
        if (ft_strcmp(shell->invalid_keys[i], key) == 0)
            return 1;
        i++;
    }
    return 0;
}

// --- إدارة المفاتيح الغير صالحة ---
void add_invalid_key(t_shell *shell, const char *key)
{
    if (invalid_key_exists(shell, key))
        return; 
    int n = shell->invalid_count;
    char **new_arr = malloc(sizeof(char *) * (n + 2));
    if (!new_arr)
        return;

    int i = 0;
    while (i < n)
    {
        new_arr[i] = shell->invalid_keys[i];
        i++;
    }
    new_arr[n] = ft_strdup(key);
    new_arr[n + 1] = NULL;

    free(shell->invalid_keys);
    shell->invalid_keys = new_arr;
    shell->invalid_count++;
}

void free_invalid_keys(t_shell *shell)
{
    int i = 0;
    while (i < shell->invalid_count)
    {
        if (shell->invalid_keys[i])
            free(shell->invalid_keys[i]);
        i++;
    }
    free(shell->invalid_keys);
    shell->invalid_keys = NULL;
    shell->invalid_count = 0;
}

// --- التحقق من صلاحية المفتاح ---
static int is_valid_key(const char *key)
{
    if (!key || !key[0])
        return 0;
    if (ft_isdigit(key[0]))
        return 0;
    int i = 0;
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

// --- إزالة علامات الاقتباس الخارجية ---
static char *strip_outer_quotes_full(const char *str)
{
    size_t len = ft_strlen(str);
    if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') ||
                     (str[0] == '\'' && str[len - 1] == '\'')))
        return ft_substr(str, 1, len - 2);
    return ft_strdup(str);
}

// --- الهروب داخل قيمة المتغير فقط ---
static char *escape_inner_quotes(const char *str)
{
    int i = 0;
    int len = 0;
    while (str[i])
    {
        if (str[i] == '"')
            len += 2;
        else
            len++;
        i++;
    }

    char *escaped = malloc(len + 1);
    if (!escaped)
        return NULL;

    i = 0;
    int j = 0;
    while (str[i])
    {
        if (str[i] == '"')
        {
            escaped[j++] = '\\';
            escaped[j++] = '"';
        }
        else
        {
            escaped[j++] = str[i];
        }
        i++;
    }
    escaped[j] = '\0';
    return escaped;
}

void print_export_format(const char *str)
{
    // منع طباعة المتغير _
    if (ft_strncmp(str, "_=", 2) == 0)
        return;

    char *eq = ft_strchr(str, '=');
    if (eq)
    {
        int key_len = eq - str;
        char *key = ft_substr(str, 0, key_len);
        char *value = eq + 1;

        char *escaped_value = escape_inner_quotes(value);

        printf("declare -x %s=\"%s\"\n", key, escaped_value);

        free(key);
        free(escaped_value);
    }
    else
    {
        printf("declare -x %s\n", str);
    }
}

void add_or_update_env(t_shell *shell, const char *arg)
{
    char *clean = strip_outer_quotes_full(arg);
    char *eq = ft_strchr(clean, '=');

    if (!eq)
    {
        free(clean);
        return;
    }

    int key_len = eq - clean;
    char *key = ft_substr(clean, 0, key_len);
    char *value = eq + 1;

    if (!is_valid_key(key))
    {
        shell->exit_status = 1;
        add_invalid_key(shell, key);
        free(key);
        free(clean);
        return;
    }

    int i = 0;
    while (shell->copy_env && shell->copy_env[i])
    {
        if (ft_strncmp(shell->copy_env[i], key, ft_strlen(key)) == 0 &&
            shell->copy_env[i][ft_strlen(key)] == '=')
        {
            free(shell->copy_env[i]);
            char *tmp = ft_strjoin(key, "=");
            shell->copy_env[i] = ft_strjoin(tmp, value);
            free(tmp);
            free(key);
            free(clean);
            return;
        }
        i++;
    }

    int j = 0;
    while (shell->copy_env && shell->copy_env[j])
        j++;

    char **new_env = malloc(sizeof(char *) * (j + 2));
    int k = 0;
    while (k < j)
    {
        new_env[k] = ft_strdup(shell->copy_env[k]);
        k++;
    }

    char *tmp = ft_strjoin(key, "=");
    new_env[j] = ft_strjoin(tmp, value);
    new_env[j + 1] = NULL;

    // free_array(shell->copy_env);
    shell->copy_env = new_env;

    free(tmp);
    free(key);
    free(clean);
}

void add_to_export_only(t_shell *shell, const char *arg)
{
    char *clean = strip_outer_quotes_full(arg);
    char *eq = ft_strchr(clean, '=');
    char *key = eq ? ft_substr(clean, 0, eq - clean) : ft_strdup(clean);

    if (!is_valid_key(key))
    {

        shell->exit_status = 1;
        add_invalid_key(shell, key);
        free(key);
        free(clean);
        return;
    }
             

    int i = 0;
    while (shell->export_env && shell->export_env[i])
    {
        if (ft_strcmp(shell->export_env[i], key) == 0)
        {
            free(key);
            free(clean);
            return;
        }
        i++;
    }

    int len = 0;
    while (shell->export_env && shell->export_env[len])
        len++;

    char **new_export = malloc(sizeof(char *) * (len + 2));
    int k = 0;
    while (k < len)
    {
        new_export[k] = ft_strdup(shell->export_env[k]);
        k++;
    }

    new_export[len] = ft_strdup(key);
    new_export[len + 1] = NULL;

    free_array(shell->export_env);
    shell->export_env = new_export;

    free(key);
    free(clean);
}

// --- تحديث المتغير _ في copy_env (بعد تنفيذ أمر) ---

// --- الدالة الرئيسية لبناء export ---
void builtin_export(t_shell *shell)
{
    free_invalid_keys(shell);

    if (!shell->cmd[1])
    {
        char **sorted_env = duplicate_string_array(shell->copy_env);
        char **sorted_export = duplicate_string_array(shell->export_env);

        sort_string_array(sorted_env);
        sort_string_array(sorted_export);

        int i = 0;
        while (sorted_env && sorted_env[i])
        {
            // منع طباعة المتغير _
            if (ft_strncmp(sorted_env[i], "_=", 2) != 0)
                print_export_format(sorted_env[i]);
            i++;
        }

        int j = 0;
        while (sorted_export && sorted_export[j])
        {
            int found = 0;
            int k = 0;
            while (shell->copy_env && shell->copy_env[k])
            {
                if (ft_strncmp(shell->copy_env[k], sorted_export[j],
                            ft_strlen(sorted_export[j])) == 0 &&
                    shell->copy_env[k][ft_strlen(sorted_export[j])] == '=')
                {
                    found = 1;
                    break;
                }
                k++;
            }
            // منع طباعة المتغير _
            if (!found && ft_strcmp(sorted_export[j], "_") != 0)
                print_export_format(sorted_export[j]);
            j++;
        }

        free_array(sorted_env);
        free_array(sorted_export);

        shell->exit_status = 0;
        return;
    }


    int i = 1;
    while (shell->cmd[i])
    {
        if (ft_strchr(shell->cmd[i], '='))
        {
            add_or_update_env(shell, shell->cmd[i]);
            add_to_export_only(shell, shell->cmd[i]);
        }
        else
        {
            add_to_export_only(shell, shell->cmd[i]);
        }
        i++;
    }

    if (shell->invalid_count > 0)
    {
        int idx = 0;
        while (idx < shell->invalid_count)
        {
            printf("minishell: export: `%s': not a valid identifier\n",
                   shell->invalid_keys[idx]);
            // free(shell->invalid_keys[idx]);
            idx++;
        }
        free_invalid_keys(shell);
        shell->exit_status = 1;
    }
    else
        shell->exit_status = 0;
}
