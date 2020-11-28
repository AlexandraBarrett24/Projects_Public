// Example usage: PathBuild (“/user/jon”, “dir1”, new_path)
// This would return new_path with the new absolute path “/user/jon/dir1”)
// new_path is an output parameter; PathBuild first empties new_path if it contains any prior data
int PathBuild (char *curr_path, char *subdir name, char *new_path);

// we recommend that you use this provided helper function: returns 1 (or true) if mode in inode (see API) indicates a directory
int S_ISDIR (mode_t mode);

#define MAX_PATH 1024
long long du (char *start_path) {
  long long totalBytes = 0;
  DIR *dir;
  struct dirent *entry;
  if (!(dir = opendir(start_path)))
      return;
  while((entry = readdir(start_path)) != NULL){
        struct stat entry_stat;
        lstat(path, &entry_stat);

        if(entry->d_type == DT_DIR){
            char *new_path[MAX_PATH];
            PathBuild (start_path, entry->d_name, new_path);
            totalBytes += du(new_path);
        }
        if(entry->d_type == DT_REG){
            totalBytes += (long long)entry_stat.st_size;
        }
      }

      closedir(start_path);
      return totalBytes;
    }
