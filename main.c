#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wordsOfLength(int inputLength){//checks words in words.txt length against the desired length
    char filename[] = "words.txt";
    int counter = 0;
    FILE *filePtr  = fopen(filename, "r");
    char inputString[81];
    while(fscanf(filePtr, "%s", inputString) != EOF) {
		if(strlen(inputString) == inputLength){//if input is same length as word in file
            counter++;
        }
	}
    fclose(filePtr);
    return counter;
}

void *generateRandom(char wordToChange[81],int size, char **wordsArr){//generates random word from words.txt array
    int index = (rand() % size);
    strcpy(wordToChange, wordsArr[index]);
}

void populateArray(char **array,int size,int wordLength){//populates an array of words from txt file of correct length
    char filename[] = "words.txt";
    int counter = 0;
    FILE *filePtr  = fopen(filename, "r");
    char inputString[81];

    for(int i = 0; i < size;i++){
        array[i] = (char*) malloc ((wordLength) * sizeof(char));//allocating for each row
    }

    while(fscanf(filePtr, "%s", inputString) != EOF) {//entering words into array
        if(strlen(inputString) == wordLength){
            strcpy(array[counter],inputString);
            counter++;
        }
    }
    fclose(filePtr);
}

int isDictionary(char **dictArray, char input[81],int size){//checks if input word is in the txt file(i.e. is dictionary)
    int counter = 0;
    for(int i = 0; i < size; i++){
        if(strcmp(dictArray[i],input) != 0){//if word is not found in file
            counter++;
        }
    }
    if(counter == size){//word was never found
        return 0;
    }
    return 1;

}

int checkChanges(const char word1[81], const char word2[81], int numChars){//checks diff characters in words
    int counter = 0;
    for(int i = 0; i < numChars;i++){
        if(word1[i] != word2[i]) {//characters are different
                counter++;
        }
    }
    if(counter != 1){//there were too many or too little changes made
        return 0;
    }
    return 1;
}

void game(char start[81], char end[81], int size,char **words,int validWords){
    int counter = 1;
    char currentWord[81];
    char temp[81];
    strcpy(temp,start);

    while (strcmp(temp, end) != 0) {//word entered is not the same as goal word
            printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", counter, temp, end);
            scanf("%s", currentWord);

            if(strcmp(currentWord,"q") == 0){//end game
                return;
            }

            if (strlen(currentWord) != size) {//new word is not the correct length
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", currentWord, size);
                continue;
            }

            if (isDictionary(words, currentWord, validWords) != 1) {//new word is not dictionary
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", currentWord);
                continue;
            }

            if (checkChanges(currentWord, temp, size) == 0) {//new word has not been correctly changed
                printf("Your word, '%s', is not exactly 1 character different. Try again.\n", currentWord);
                continue;
            }
            counter++;
            strcpy(temp, currentWord);
    }

    if(strcmp(currentWord, end) == 0){
        printf("Congratulations! You changed '%s' into '%s' in %d moves.\n",start,end,counter - 1);
    }
}

//menu options 1 & 2
void options(char menuOpt[81],char start[81],char end[81],int numValid,int wordSize,char **arrayWords){
   int correctFirst = 0; int correctEnd = 0;

   //******************************* Start & End word operations ***********************//
    while(correctFirst == 0 || correctEnd == 0) {
        printf("Enter starting and ending words, or 'r' for either for a random word: ");
        scanf("%s %s", start, end);

        if (strcmp(start, "r") == 0) {//generates random word of correct length
            generateRandom(start, numValid, arrayWords);
        }
        if (strcmp(end, "r") == 0) {//generates random word of correct length
            generateRandom(end, numValid, arrayWords);
        }

        if (strcmp(start, "r") != 0 && strcmp(end, "r") != 0) {
            if (strlen(start) != wordSize) {//incorrect start word length
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", start, wordSize);
                correctFirst = 0;
                continue;
            }  if (isDictionary(arrayWords, start, numValid) != 1) {//start word is not dictionary
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", start);
                correctFirst = 0;
                continue;
            }else{//first word is ok to use
                correctFirst = 1;
            }

            if(strlen(end) != wordSize){//end word is incorrect length
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", end, wordSize);
                correctEnd = 0;
                continue;
            } if (isDictionary(arrayWords, end, numValid) != 1) {//end word is not dictionary word
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", end);
                correctEnd = 0;
                continue;
            }else{//end word is ok to use
                correctEnd = 1;
            }
        }
    }
    //******************************* Start & End word operations ***********************//
    
    printf("Your starting word is: %s.\n", start);
    printf("Your ending word is: %s.\n\n", end);

    printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
    printf("You may also type in 'q' to quit guessing.\n");
    game(start,end,wordSize,arrayWords,numValid);
}

int main() {
    srand(1);
    int numLetters = 0, numValidWords = 0, validStart = 0,validEnd = 0;
    char startWord[81] = "", endWord[81] = "", option[81] = "";

    printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
    printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
    printf("Enjoy!\n\n");

    printf("How many letters do you want to have in the words? ");
    scanf("%d", &numLetters);

    printf("Number of %d-letter words found: %d.\n\n", numLetters, wordsOfLength(numLetters));

    numValidWords = wordsOfLength(numLetters);//finds how many words of each length in words.txt

    char **wordsArray = (char **) malloc((numValidWords) * (sizeof(char *)));

    //populating array with the words of correct length from words.txt
    populateArray(wordsArray, numValidWords, numLetters);


    //********************************Validating word operations***********************************//
    while (validStart == 0 || validEnd == 0) {
        printf("Enter starting and ending words, or 'r' for either for a random word: ");
        scanf("%s %s", startWord, endWord);

        if (strcmp(startWord, "r") == 0) {//generate random start word from word file
            generateRandom(startWord, numValidWords, wordsArray);
        }
        if (strcmp(endWord, "r") == 0) {//generate random end word from word file
            generateRandom(endWord, numValidWords, wordsArray);
        }

        if (strcmp(startWord, "r") != 0 && strcmp(endWord, "r") != 0) {
            if (strlen(startWord) != numLetters) {//start word is incorrect length
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", startWord, numLetters);
                validStart = 0;
                continue;
            }
            if (isDictionary(wordsArray, startWord, numValidWords) != 1) {//start word is not dictionary word
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", startWord);
                validStart = 0;
                continue;
            } else {//start word is now ok to use
                validStart = 1;
            }

            if (strlen(endWord) != numLetters) {//end word is not correct length
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", endWord, numLetters);
                validEnd = 0;
                continue;
            }
            if (isDictionary(wordsArray, endWord, numValidWords) != 1) {//end word is not a dictionary word
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", endWord);
                validEnd = 0;
                continue;
            } else {//end word is now ok to use
                validEnd = 1;
            }
        }
    }
//*************************************************************************************//

    printf("Your starting word is: %s.\n", startWord);
    printf("Your ending word is: %s.\n\n", endWord);

    printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
    printf("You may also type in 'q' to quit guessing.\n");

    game(startWord, endWord, numLetters, wordsArray, numValidWords);//ADD WHILE LOOP

    while (strcmp(option, "3") != 0) {
        printf("\nEnter: \t1 to play again,\n");
        printf("\t2 to change the number of letters in the words and then play again, or\n");
        printf("\t3 to exit the program.\n");
        printf("Your choice --> ");

        scanf("%s", option);


        if (strcmp(option, "1") == 0) {//option 1
            options(option, startWord, endWord, numValidWords, numLetters, wordsArray);
        }
        if (strcmp(option, "2") == 0) {//option 2
            free(wordsArray);

            printf("How many letters do you want to have in the words? ");
            scanf("%d", &numLetters);

            printf("Number of %d-letter words found: %d.\n\n", numLetters, wordsOfLength(numLetters));

            numValidWords = wordsOfLength(numLetters);//finds how many words of each length in words.txt

            wordsArray = (char **) malloc((numValidWords) * (sizeof(char *)));

            //populating array with the words of correct length from words.txt
            populateArray(wordsArray, numValidWords, numLetters);

            options(option, startWord, endWord, numValidWords, numLetters, wordsArray);
        }
    }

    if (strcmp(option, "3") == 0) {
        printf("\nThanks for playing!\n");
        printf("Exiting...\n");
        exit(0);
    }

    for (int i = 0; i < numValidWords; i++) {
     free(wordsArray[i]);
    }
    free(wordsArray);
	return 0;
}