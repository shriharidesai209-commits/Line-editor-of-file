#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

typedef struct LineNode {
    char *text;
    struct LineNode *next;
} LineNode;

typedef struct {
    LineNode *head;
    int line_count;
} Document;

/* Initialize an empty document */
void init_document(Document *doc) {
    doc->head = NULL;
    doc->line_count = 0;
}

/* Free all allocated lines */
void free_document(Document *doc) {
    LineNode *curr = doc->head;
    while (curr != NULL) {
        LineNode *temp = curr;
        curr = curr->next;
        free(temp->text);
        free(temp);
    }
    doc->head = NULL;
    doc->line_count = 0;
}

/* Core: Display all lines with line numbers */
void display_document(const Document *doc) {
    if (doc->head == NULL) {
        printf("[Document is empty]\n");
        return;
    }
    LineNode *curr = doc->head;
    int index = 1;
    printf("\n--- Document Contents (%d lines) ---\n", doc->line_count);
    while (curr != NULL) {
        printf("%4d | %s\n", index++, curr->text);
        curr = curr->next;
    }
    printf("------------------------------------\n\n");
}

/* Core: Insert line at 1-based position */
int insert_line(Document *doc, int pos, const char *text) {
    if (pos < 1 || pos > doc->line_count + 1) {
        printf("Error: Invalid position. Valid range is 1 to %d.\n", doc->line_count + 1);
        return 0;
    }

    LineNode *new_node = (LineNode *)malloc(sizeof(LineNode));
    if (!new_node) {
        perror("Allocation failed");
        return 0;
    }

    new_node->text = (char *)malloc(strlen(text) + 1);
    if (new_node->text==NULL) {
        free(new_node);
        perror("Allocation failed");
        return 0;
    }
    strcpy(new_node->text, text);
    new_node->next = NULL;

    if (pos == 1) {
        new_node->next = doc->head;
        doc->head = new_node;
    } else {
        LineNode *curr = doc->head;
        for (int i = 1; i < pos - 1; i++) {
            curr = curr->next;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }

    doc->line_count++;
    printf("Line inserted at %d.\n", pos);
    return 1;
}

/* Core: Delete line at 1-based position */
int delete_line(Document *doc, int pos) {
    if (doc->head == NULL) {
        printf("Error: Document is empty.\n");
        return 0;
    }
    if (pos < 1 || pos > doc->line_count) {
        printf("Error: Invalid position. Valid range is 1 to %d.\n", doc->line_count);
        return 0;
    }

    LineNode *target = NULL;

    if (pos == 1) {
        target = doc->head;
        doc->head = doc->head->next;
    } else {
        LineNode *curr = doc->head;
        for (int i = 1; i < pos - 1; i++) {
            curr = curr->next;
        }
        target = curr->next;
        curr->next = target->next;
    }

    free(target->text);
    free(target);
    doc->line_count--;
    printf("Line %d deleted.\n", pos);
    return 1;
}

/* Core: Save to file */
void save_to_file(const Document *doc, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Unable to open file '%s' for writing.\n", filename);
        return;
    }

    LineNode *curr = doc->head;
    while (curr != NULL) {
        fprintf(fp, "%s\n", curr->text);
        curr = curr->next;
    }

    fclose(fp);
    printf("Document successfully saved to '%s'.\n", filename);
}

/* Core: Load from file (replaces current buffer) */
void load_from_file(Document *doc, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Unable to open file '%s' for reading.\n", filename);
        return;
    }

    free_document(doc);

    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0'; // Strip newlines
        insert_line(doc, doc->line_count + 1, buffer);
    }

    fclose(fp);
    printf("Loaded %d lines from '%s'.\n", doc->line_count, filename);
}

/* Bonus: Search word or phrase */
void search_text(const Document *doc, const char *query) {
    if (doc->head == NULL) {
        printf("Document is empty.\n");
        return;
    }

    LineNode *curr = doc->head;
    int index = 1;
    int matches = 0;

    printf("\n--- Search Results for '%s' ---\n", query);
    while (curr != NULL) {
        if (strstr(curr->text, query) != NULL) {
            printf("Line %d: %s\n", index, curr->text);
            matches++;
        }
        curr = curr->next;
        index++;
    }

    if (matches == 0) {
        printf("No matches found.\n");
    } else {
        printf("Found %d matching line(s).\n", matches);
    }
    printf("-------------------------------\n\n");
}

/* Bonus: Document statistics (line and word counts) */
void show_stats(const Document *doc) {
    int total_words = 0;
    int total_chars = 0;

    LineNode *curr = doc->head;
    while (curr != NULL) {
        total_chars += strlen(curr->text);
        char *copy = strdup(curr->text);
        char *token = strtok(copy, " \t\r\n");
        while (token != NULL) {
            total_words++;
            token = strtok(NULL, " \t\r\n");
        }
        free(copy);
        curr = curr->next;
    }

    printf("\n--- Document Statistics ---\n");
    printf("Total Lines     : %d\n", doc->line_count);
    printf("Total Words     : %d\n", total_words);
    printf("Total Characters: %d\n", total_chars);
    printf("---------------------------\n\n");
}

void print_help(void) {
    printf("\nAvailable Commands:\n");
    printf("  p                    - Print the entire document\n");
    printf("  i <line> <text>      - Insert text at specific line number\n");
    printf("  a <text>             - Append text at the end of the document\n");
    printf("  d <line>             - Delete the specified line number\n");
    printf("  s <filename>         - Save document to a text file\n");
    printf("  l <filename>         - Load document from a text file\n");
    printf("  f <query>            - Find/search lines matching query\n");
    printf("  stat                 - Show line, word, and character count\n");
    printf("  h                    - Show this help menu\n");
    printf("  q                    - Quit the editor\n\n");
}

int main() {
    Document doc;
    init_document(&doc);

    char input[MAX_LINE_LEN];
    printf("Minimal Line Editor (C). Type 'h' for help or 'q' to exit.\n");

    while (1) {
        printf("editor> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        input[strcspn(input, "\r\n")] = '\0';
        if (strlen(input) == 0) continue;

        char cmd[16] = {0};
        sscanf(input, "%15s", cmd);

        if (strcmp(cmd, "q") == 0) {
            break;
        } else if (strcmp(cmd, "h") == 0) {
            print_help();
        } else if (strcmp(cmd, "p") == 0) {
            display_document(&doc);
        } else if (strcmp(cmd, "stat") == 0) {
            show_stats(&doc);
        } else if (strcmp(cmd, "d") == 0) {
            int line_no = 0;
            if (sscanf(input, "%*s %d", &line_no) == 1) {
                delete_line(&doc, line_no);
            } else {
                printf("Usage: d <line_number>\n");
            }
        } else if (strcmp(cmd, "i") == 0) {
            int line_no = 0;
            int offset = 0;
            if (sscanf(input, "%*s %d %n", &line_no, &offset) == 1 && offset > 0) {
                insert_line(&doc, line_no, input + offset);
            } else {
                printf("Usage: i <line_number> <text>\n");
            }
        } else if (strcmp(cmd, "a") == 0) {
            char *text_start = input + 1;
            while (*text_start == ' ') text_start++;
            insert_line(&doc, doc.line_count + 1, text_start);
        } else if (strcmp(cmd, "s") == 0) {
            char filename[256];
            if (sscanf(input, "%*s %255s", filename) == 1) {
                save_to_file(&doc, filename);
            } else {
                printf("Usage: s <filename>\n");
            }
        } else if (strcmp(cmd, "l") == 0) {
            char filename[256];
            if (sscanf(input, "%*s %255s", filename) == 1) {
                load_from_file(&doc, filename);
            } else {
                printf("Usage: l <filename>\n");
            }
        } else if (strcmp(cmd, "f") == 0) {
            char *query = input + 1;
            while (*query == ' ') query++;
            if (strlen(query) > 0) {
                search_text(&doc, query);
            } else {
                printf("Usage: f <search_query>\n");
            }
        } else {
            printf("Unknown command '%s'. Type 'h' for help.\n", cmd);
        }
    }

    free_document(&doc);
    printf("Exited line editor.\n");
    return 0;
}