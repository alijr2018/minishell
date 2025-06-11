#include "minishell.h"

char *get_path(t_shell *shell) {
    struct stat st;
    char *path_env;
    char **dirs;
    char *full_cmd;
    size_t len;
    int i;

    if (ft_strchr(shell->cmd[0], '/')) {
        if (stat(shell->cmd[0], &st) == 0) {
            if (S_ISDIR(st.st_mode))
                return ft_strdup("IS_DIR");
            return ft_strdup(shell->cmd[0]);
        } else {
            return ft_strdup("k");
        }
    }

    path_env = get_env_value(shell, "PATH");
    if (!path_env)
        return NULL;

    dirs = ft_split(path_env, ':');
    if (!dirs)
        return NULL;

    i = 0;
    while (dirs[i]) {
        len = ft_strlen(dirs[i]) + ft_strlen(shell->cmd[0]) + 2;
        full_cmd = malloc(len);
        if (!full_cmd) {
            free_array(dirs);
              return NULL;
        }

        ft_strlcpy(full_cmd, dirs[i], len);
        ft_strlcat(full_cmd, "/", len);
        ft_strlcat(full_cmd, shell->cmd[0], len);

        if (stat(full_cmd, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                free(full_cmd);
                i++;
                continue;
            }
            free_array(dirs);
            return full_cmd;
        }

        free(full_cmd);
        i++;
    }

    free_array(dirs);
    return NULL;
}
