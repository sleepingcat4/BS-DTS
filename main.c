#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>

#define MAX_PATH_LENGTH 256
#define MAX_SEARCH_RESULTS 10

typedef struct SearchResult {
    char path[MAX_PATH_LENGTH];
    bool isDirectory;
} SearchResult;

typedef struct SearchNode {
    char path[MAX_PATH_LENGTH];
    struct SearchNode* parent;
} SearchNode;

typedef struct Beam {
    SearchNode** nodes;
    int size;
} Beam;

Beam* create_beam(int beam_size) {
    Beam* beam = (Beam*)malloc(sizeof(Beam));
    beam->nodes = (SearchNode**)malloc(beam_size * sizeof(SearchNode*));
    beam->size = 0;
    return beam;
}

void destroy_beam(Beam* beam) {
    if (beam != NULL) {
        free(beam->nodes);
        free(beam);
    }
}

void add_node_to_beam(Beam* beam, SearchNode* node) {
    beam->nodes[beam->size++] = node;
}

SearchNode* create_search_node(const char* path, SearchNode* parent) {
    SearchNode* node = (SearchNode*)malloc(sizeof(SearchNode));
    strncpy(node->path, path, MAX_PATH_LENGTH);
    node->parent = parent;
    return node;
}

void destroy_search_node(SearchNode* node) {
    if (node != NULL)
        free(node);
}

void beam_search(const char* filename, const char* current_path, Beam* beam) {
    DIR* dir;
    struct dirent* entry;

    dir = opendir(current_path);
    if (dir == NULL) {
        printf("Unable to open directory: %s\n", current_path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char entry_path[MAX_PATH_LENGTH];
        snprintf(entry_path, MAX_PATH_LENGTH, "%s\\%s", current_path, entry->d_name);

        if (strcmp(entry->d_name, filename) == 0) {
            SearchResult* result = (SearchResult*)malloc(sizeof(SearchResult));
            strncpy(result->path, entry_path, MAX_PATH_LENGTH);
            result->isDirectory = (entry->d_type == DT_DIR);
            add_node_to_beam(beam, create_search_node(entry_path, beam->nodes[0]));
        }

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            add_node_to_beam(beam, create_search_node(entry_path, beam->nodes[0]));
        }
    }

    closedir(dir);
}

void print_search_results(Beam* beam) {
    printf("Search Results:\n");

    int result_count = 0;
    for (int i = 0; i < beam->size; i++) {
        if (beam->nodes[i] != NULL) {
            printf("%d. %s\n", ++result_count, beam->nodes[i]->path);
        }
    }

    if (result_count == 0) {
        printf("No results found.\n");
    }
}

void cleanup_beam(Beam* beam) {
    for (int i = 0; i < beam->size; i++) {
        destroy_search_node(beam->nodes[i]);
        beam->nodes[i] = NULL;
    }
    beam->size = 0;
}

int main() {
    char search_name[MAX_PATH_LENGTH];

    printf("Enter the file or folder name to search: ");
    fgets(search_name, MAX_PATH_LENGTH, stdin);
    search_name[strcspn(search_name, "\n")] = '\0';

    Beam* beam = create_beam(MAX_SEARCH_RESULTS);

    add_node_to_beam(beam, create_search_node("C:\\", NULL));

    while (beam->size > 0) {
        SearchNode* current_node = beam->nodes[0];
        beam_search(search_name, current_node->path, beam);

        if (beam->size > 1) {
            destroy_search_node(current_node);
            beam->nodes[0] = NULL;
            for (int i = 1; i < beam->size; i++) {
                beam->nodes[i - 1] = beam->nodes[i];
            }
            beam->size--;
        } else {
            break;
        }
    }

    print_search_results(beam);
    cleanup_beam(beam);
    destroy_beam(beam);

    return 0;
}
