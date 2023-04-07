#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) {
        return va - vb;
    }
    return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

#define MAX_LINE 128

bool is_new_word(char new_char, char last_char){
    if (new_char != ' ' && new_char != '\t' && new_char != '\n'){
        if (last_char == ' ' || last_char=='\t' || last_char =='\n') {
            return true;
        }
    }
    return false;
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    *nl = 0;
    *nc = 0;
    *nw = 0;
    char str[100000] = "";
    char last_char = '\0';
    int c = 0;
    while( ( c = fgetc(stdin)) != EOF ){
        for(int i = 0; i < strlen(str); i++){
            printf("%c\n", str[i]);
            if (is_new_word(last_char, str[i])) {
                *nw = *nw + 1;
            }
            if(str[i] == '\n'){
                *nl = *nl + 1;
            }
            *nc = *nc + 1;
            last_char = str[i];
        }
    }
}



void char_count(int char_no, int *n_char, int *cnt) {
    char str[100000] = "\0";
    read_text(str);

    int nums[MAX_CHARS];
    for(int i = 0; i < MAX_CHARS; i++){
        nums[i] = FIRST_CHAR + i;
    }
    for(int i = 0; i < strlen(str); i++){
        int c = (int)(str[i]);
        if (c > FIRST_CHAR){
            count[c] += 1;
        }
    }
    qsort(nums, MAX_CHARS, sizeof(*nums), cmp);
    * n_char = nums[char_no - 1];
    * cnt = count[nums[char_no - 1]];
}
int find_bigram_index(char bigrams[][2], char bigram[2]){
    for(int i = 0; i < MAX_BIGRAMS; i++){
        if(bigrams[i][0] == bigram[0] && bigrams[i][1] == bigram[1]){
            return i;
        }
    }
    return -1;
}

bool is_in_range(char c){
    return (int)(c) > FIRST_CHAR && (int)(c) < LAST_CHAR;
}

void bigram_count(int bigram_no, int bigram[]) {
    char str[100000] = "\0";
    read_text(str);
    char bigrams[MAX_BIGRAMS][2];
    int bigrams_indexes[MAX_BIGRAMS];
    int index = 0;
    for(int i = FIRST_CHAR; i < LAST_CHAR; i++){
        for(int j = FIRST_CHAR; j < LAST_CHAR; j ++){
            bigrams[index][0] = (char)(i);
            bigrams[index][1] = (char)(j);
            bigrams_indexes[index] = index;
            index++;
        }
    }
    for(int i = 1; i < strlen(str); i++){
        char c[2] = {str[i-1], str[i]};
        if (is_in_range(c[0]) && is_in_range(c[1])){
            count[find_bigram_index(bigrams, c)] += 1;
        }
    }
    qsort(bigrams_indexes, MAX_BIGRAMS, sizeof(*bigrams_indexes), cmp_di);

    bigram[0] = bigrams[bigrams_indexes[bigram_no-1]][0];
    bigram[1] = bigrams[bigrams_indexes[bigram_no-1]][1];
    bigram[2] = count[bigrams_indexes[bigram_no-1]];
}
void find_comments(int *line_comment_counter, int *block_comment_counter) {
    char str[100000] = "\0";
    read_text(str);
    *line_comment_counter = 0;
    *block_comment_counter = 0;
    /*
     * types of comments:
     * 0 - line
     * 1 - block
     * -1 - none
     */
    int current_comment_type = -1;
    for(int i = 1; i < strlen(str); i++) {
        char c[2] = {str[i-1], str[i]};
        if (c[0] == '/' && c[1] == '/') {
            if (current_comment_type == -1) {
                *line_comment_counter = *line_comment_counter + 1;
                current_comment_type = 0;
            }

        }
        else if (c[0] == '/' && c[1] == '*') {
            if (current_comment_type == -1) {
                *block_comment_counter = *block_comment_counter + 1;
                current_comment_type = 1;
            }
        }
        else if (c[0] == '*' && c[1] == '/') {
            current_comment_type = -1;
        }
        else if (c[0] == '\n' && current_comment_type == 0) {
            current_comment_type = -1;
        }
    }
}

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];
	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

