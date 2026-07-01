#ifndef SRC_COMMON_RESPITE_RC_UTILS_H_
#define SRC_COMMON_RESPITE_RC_UTILS_H_

#include <glib.h>

#define RESPITE_HISTORY_FILE              "respite/history"

#define RESPITE_RC_GROUP_GENERAL          "General"
#define RESPITE_RC_GROUP_PLUGINS          "Plugins"

gchar   **respite_get_history              (void);
gchar   **respite_get_history_full         (const gchar *relpath);
void      respite_insert_line_history      (const gchar *line);
void      respite_insert_line_history_full (const gchar *relpath, const gchar *line);
void      respite_clear_history_file       (void);
void      respite_clear_history_file_full  (const gchar *relpath);

#endif /* SRC_COMMON_RESPITE_RC_UTILS_H_ */