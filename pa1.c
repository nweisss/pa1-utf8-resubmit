#include <stdio.h>
#include <stdint.h>
#include <string.h>


// Milestone 1

int32_t is_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] > 127 || str[i] < 0){
            return 0;
        }
    }
    return 1;
}

int32_t capitalize_ascii(char str[]){
    int count = 0;
    for(int i = 0; str[i]; str++){
        if(str[i] >= 97 && str[i] <= 122){
            str[i] -= 32;
            count++;
        }
    }
    return count;
}

// Milestone 2

int32_t width_from_start_byte(char start_byte){
    if ((start_byte & 0b10000000) == 0) {
        return 1;
    }

    if ((start_byte & 0b11110000) == 0b11110000) {
        return 4; 
    } else if ((start_byte & 0b11100000) == 0b11100000) {
        return 3;
    } else if ((start_byte & 0b11000000) == 0b11000000) {
        return 2;
    } else {
        return -1;
    }
}

// returns number of codepoints
int32_t utf8_strlen(char str[]){
    int code_points = 0;
    for(int i = 0; str[i] != '\0';){
        int32_t width = width_from_start_byte(str[i]);
        if(width == -1){
            return -1;
        }
        code_points++;
        i += width;
    }
    return code_points;
}

// my process:
//    while the cpi is greater than or equal to zero, iterate backwards and count up the byte widths of each charachter
int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){
    int32_t ret = 0;
    for(; cpi >= 0; cpi--){
        ret += width_from_start_byte(str[cpi]);
    }
    return ret;
}


void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]){
    // first get byte index of cpi_start and cpi_end
    int32_t bi_start;
    int32_t bi_end;
    int32_t code_point_tracker = 0;
    int32_t i = 0;
    while(code_point_tracker != cpi_end){
        if(str[i] == '\0'){
            break;
        }
        if(code_point_tracker == cpi_start){
            bi_start = i;
        }
        i += width_from_start_byte(str[i]);
        code_point_tracker++;
    }
    bi_end = i;

    // beginning at the starting byte index, looping until the ending byte index
    int32_t k = 0;
    while(bi_start != bi_end){
        result[k] = str[bi_start];
        bi_start++;
        k++;
    }
    result[k] = '\0';
}



// Milestone 3

// shifting ugh
int32_t codepoint_at(char str[], int32_t cpi){
    int32_t code_point_tracker = 0;
    int32_t i = 0;
    while(code_point_tracker != cpi){
        i += width_from_start_byte(str[i]);
        code_point_tracker++;
    }

    
    int32_t width = width_from_start_byte(str[i]);
    if(width == 1){
        return str[i];
    }

    uint32_t real_code_point = 0;
    int iteration = 0;
    for(int k = i + width - 1; k >= i; k--){
        int32_t add;
        if(iteration == 0){
            add = (str[k] & 0b00111111);
            real_code_point += add;
        }
        else if(k == i && width == 3){
            add = (str[k] & 0b00011111);
            add = add << (6*iteration);
            real_code_point += add;
        }
        else if(k == i && width == 4){
            add = (str[k] & 0b00001111);
            add = add << (6*iteration);
            real_code_point += add;
        }
        else{
            add = (str[k] & 0b00111111);
            add = add << (6*iteration);
            real_code_point += add;
        }
        iteration++;
    }
    return real_code_point;
}


// U+1F40x mouse row
// U+1F43x squirel row

// U+1F98x crab row
// U+1F9Ax guide dog row (should not include last thing in row)

char is_animal_emoji_at(char str[], int32_t cpi){
    int32_t cp = codepoint_at(str, cpi);
    if((cp >= 128000 && cp <= 128063) || (cp >= 129408 && cp <= 129454)){
        return 1;
    }
    else return 0;
}




// Helper Functions
void format(char str[]){
    if(str[strlen(str) - 1] == '\n'){
        str[strlen(str) - 1] = 0;
    }
}

int32_t strlen_in_bytes(char str[]){
    int32_t bytes = 0;
    for(int i = 0; str[i] != '\0';){
        int32_t width = width_from_start_byte(str[i]);
        if(width == -1){
            return -1;
        }
        bytes += width;
        i += width;
    }
    return bytes;
}

void print_all_cps(char str[]){
    for(int i = 0; i < utf8_strlen(str); i++){
        printf("%d ", codepoint_at(str, i));
    }
    printf("\n");
}

void print_all_widths(char str[]){
    for(int i = 0; str[i] != '\0';){
        printf("%d ", width_from_start_byte(str[i]));
        i += width_from_start_byte(str[i]);
    }
    printf("\n");
}

void print_animal_emojis(char str[]){
    int32_t code_point_tracker = 0;
    for(int i = 0; str[i] != '\0';){
        //printf("Looking at codepoint %d\n", code_point_tracker);
        //printf("Current byte index: %d\n", i);
        int32_t width = width_from_start_byte(str[i]);
        if(width == 4){
            if(is_animal_emoji_at(str, code_point_tracker) == 1){
                char emoji[5];
                for(int k = 0; k < 4; k++){
                    emoji[k] = str[i];
                    i++;
                }
                emoji[4] = '\0';
                printf("%s ", emoji);
            }
            else{
                i += 4;
            }
            code_point_tracker++;
            continue;
        }
        code_point_tracker++;
        i += width;
    }
    printf("\n");
}

void next_utf8_char(char str[], int32_t cpi, char result[]){
    int byte_index = 0;
    int code_point_index = 0;

    for(; code_point_index < cpi; code_point_index++){
        byte_index += width_from_start_byte(str[byte_index]);
    }
    char* curr = str + byte_index;
    int width = width_from_start_byte(*curr);
    int32_t code_point = codepoint_at(curr, 0);

    //add, then convert
    code_point++;
    //printf("%d\n", code_point);

     if (code_point <= 0x7F) {
        result[0] = code_point;
        result[1] = '\0';
    } else if (code_point <= 0x7FF) {
        result[0] = 0xC0 | (code_point >> 6);
        result[1] = 0x80 | (code_point & 0x3F);
        result[2] = '\0';
    } else if (code_point <= 0xFFFF) {
        result[0] = 0xE0 | (code_point >> 12);
        result[1] = 0x80 | ((code_point >> 6) & 0x3F);
        result[2] = 0x80 | (code_point & 0x3F);
        result[3] = '\0';
    } else if (code_point <= 0x10FFFF) {
        result[0] = 0xF0 | (code_point >> 18);
        result[1] = 0x80 | ((code_point >> 12) & 0x3F);
        result[2] = 0x80 | ((code_point >> 6) & 0x3F);
        result[3] = 0x80 | (code_point & 0x3F);
        result[4] = '\0';
    } else {
        result[0] = '\0';
    }
    //printf("%s\n", result);
}



int main(){

    char test[1000];
    printf("Enter a UTF-8 encoded string : \n");
    fgets(test, 1000, stdin);

    format(test);

    // testing is_ascii
    // KEEP
    printf("Valid ASCII: %s", is_ascii(test) ? "true\n" : "false\n");

    char buffer[1000];
    strcpy(buffer, test);

    // testing capitilize_ascii
    // KEEP
    capitalize_ascii(buffer);
    printf("Uppercased ASCII: \"%s\"\n", buffer);

    // testing length in bytes ~ helper function
    // KEEP
    printf("Length in bytes: %d\n", strlen_in_bytes(test));

    // testing utf8_strlen and width_from_start_byte
    // KEEP
    printf("Number of code points: %d\n", utf8_strlen(test));

    // testing width_from_start_byte
    // KEEP
    printf("Bytes per code point: ");
    print_all_widths(test);

    // testing substring
    // KEEP
    char substring[1000];
    utf8_substring(test, 0, 6, substring);
    printf("Substring of the first 6 code points: \"%s\"\n", substring);

    // print code points as decimal numbers (testing codepoint_at function)
    // KEEP
    printf("Code points as decimal numbers: ");
    print_all_cps(test);

    // test animal emoji at
    //printf("this should give me a 0: %d\n", is_animal_emoji_at(test, 0));


    printf("Animal emojis: ");
    print_animal_emojis(test);

    printf("Next Character of Codepoint at Index 3: ");
    if(strlen(test) < 4){
        printf("\n");
    }
    else{
        char result[5];
        next_utf8_char(test, 3, result);
        printf("%s\n", result);
    }
    

}