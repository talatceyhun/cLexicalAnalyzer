#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void write(char[],FILE*);
char* lower(char[],char[]);
int isKeyword(char[], char[], char[]);
int isIdentifier(char[], FILE*);
int isIntegerCons(char[], FILE*);




int main()
{
	FILE *fp = NULL;
	FILE *fp2 = NULL;

	fp = fopen("code.psi.txt", "r");
	fp2 = fopen("code.lex.txt", "w");


	int count = 0;
	char character;
	while (!feof(fp))
	{
		character = getc(fp);
		count++;

	}
	fclose(fp);
	fp = fopen("code.psi.txt", "r");
	char* charArray;
	charArray = (char*)malloc((count+1) * sizeof(char));
	char* ptr;
	ptr = charArray;
	while ((character = getc(fp))!=EOF)
	{
		*ptr = character;
		ptr++;

	}
	*ptr = '\0';
	ptr -= count-1;
	char keywords[] = "break case char const continue do else enum float for goto if int long record return static while" ;
	
	char *currentWord;
	currentWord = (char*)malloc(20 * sizeof(char));
	
	char *ret ;
	ret = malloc(31 * sizeof(char));

	char *chars;
	chars = (char*)malloc(31 * sizeof(char));

	char* strMemory;
	strMemory = (char*)malloc(1 * sizeof(char));

	int counter = 0;
	

	while (1)
	{
		
		
		
		if (*ptr == '\0') {
			break;
		}
		else if(*ptr !='+'&& *ptr != '-' && *ptr != '*' && *ptr != '/' && *ptr != ':' && *ptr != '(' && *ptr != ')' && *ptr != '[' &&  *ptr != ']' &&
			*ptr != '{' && *ptr != '}' && *ptr != ';' && *ptr != 32 && *ptr != '\0' && *ptr != '\"' && *ptr != '\n')
		{

			counter = 0;
		

			while (*ptr != '+'&& *ptr != '-' && *ptr != '*' && *ptr != '/' && *ptr != ':' && *ptr != '(' && *ptr != ')' && *ptr != '[' &&  *ptr != ']' &&
				*ptr != '{' && *ptr != '}' && *ptr != ';' && *ptr != 32 && *ptr != '\0'  && *ptr != '\"' && *ptr != '\n')
			{
				if (counter > 29) {
					write(" Identifier can not be longer than 30 characters", fp2);
					exit(0);
					//error
				}

				else {
					chars[counter] = *ptr;
					ptr++;
					counter++;
				}
			}

			chars[counter] = '\0';
			


			
			char *word = lower(chars,ret);
			if (isKeyword(keywords, word, currentWord)) {
				write("Keyword(",fp2);
				write(word, fp2);
				write(")\n\n", fp2);
			}
			else if (isIntegerCons(word,fp2)) {
				write("IntConst(", fp2);
				write(word, fp2);
				write(")\n\n", fp2);
			}
			else if (isIdentifier(word,fp2)) {
				write("Identifier(", fp2);
				write(word, fp2);
				write(")\n\n", fp2);
			}

			
		}
		else if (*ptr == 32 || *ptr == '\n') {
			ptr++;
		}

		else if (*ptr == ';') {
			write("EndOfLine\n\n", fp2);
			ptr++;
		}

		else if (*ptr == '(') {
			write("LeftPar\n\n", fp2);
			ptr++;
		}
		
		else if (*ptr == ')') {
			write("RightPar\n\n", fp2);
			ptr++;
		}
		else if (*ptr == '[') {
			write("LeftSquareBracet\n\n", fp2);
			ptr++;
		}
		else if (*ptr == ']') {
			write("RightSquareBracet\n\n", fp2);
			ptr++;
		}
		else if (*ptr == '{') {
			write("LeftCurlyBracet\n\n", fp2);
			ptr++;
		}
		else if (*ptr == '}') {
			write("RightCurlyBracet\n\n", fp2);
			ptr++;
		}
		else if (*ptr == '+') {
			ptr++;
			if (*ptr == '+') {
				write("Operator(++)\n\n", fp2);
				
			}
			else {
				write("Operator(+)\n\n", fp2);
				ptr--;
			}
			ptr++;

		}
		else if (*ptr == '-') {
			ptr++;
			if (*ptr == '-') {
				write("Operator(--)\n\n", fp2);

			}
			else {
				write("Operator(-)\n\n", fp2);
				ptr--;
			}
			ptr++;


		}
		else if (*ptr == '/') {  
			ptr++;
			if (*ptr == '*') {
				ptr++;
				while (1) {
					if (*ptr == '\0') {
						write("never ending comment line", fp2);
						exit(0);
					}
					if (*ptr == '*') {
						ptr++;
						if (*ptr == '/') {
							break;
						}
						else {
						
							continue;
							
						}
					}
					ptr++;
				}
				
			}
			else {
				write("Operator(/)\n\n", fp2);
				ptr--;
			}
			ptr++;

		}
		else if (*ptr == ':') { 
			ptr++;
			if (*ptr == '=') {
				write("Operator(:=)\n\n", fp2);
			}
			else {
				write(": operator error.", fp2);
				exit(0); 
			}
			ptr++;

		}
		else if (*ptr == '\"') {
			int counter = 0;
			ptr++;

			while (*ptr != '\"') {
				if (*ptr == '\0') {
					//error: missing'"'
					write("missing'\"'.", fp2);
					exit(0);
					
				}
				counter++;
				ptr++;
			}

			strMemory = realloc(strMemory, (counter + 1) * sizeof(char));
			ptr -= counter;

			for (int i = 0; i<counter; i++) {
				strMemory[i] = *ptr;
				ptr++;
			}

			strMemory[counter] = '\0';

			write("String(", fp2);
			write(strMemory, fp2);
			write(")\n\n", fp2);

			ptr++;
		}



	}



	
	
	return 0;

}

void write(char token[],FILE *file ) {
	fprintf(file, "%s", token);

}

char* lower(char text[], char ret[]) {
	
	int lenght = strlen(text);

	char character;
	for (int i = 0; i < lenght; i++) {
		character = text[i];
		
		if (character > 64 && character < 91) {
			ret[i] = character+32;
		}
		else {
			ret[i] = character;
		}

	}
	ret[lenght] = '\0';
	return ret;


}

int isIntegerCons(char word[], FILE *file) {
	for (int i = 0; i<strlen(word); i++) {
		if (!(word[i]>47 && word[i]<58)) {
			return 0;
		}
	}
	if (strlen(word) > 10) {
		write("Integer can not be longer than 10 digits.", file);
		exit(0);

	}
	return 1;
}

int isIdentifier(char word[], FILE *file) {
	if ((word[0]>64 && word[0]<91) || (word[0]>96 && word[0]<123)) {

		for (int i = 1; i<strlen(word); i++) {

			if (!((word[i]>64 && word[i]<91) || (word[i]>96 && word[i]<123) || word[i] == 95 || (word[i]>47 && word[i]<58))) {
				write("The identifier may only contain letters, numbers, or underscores.", file);
				exit(0);
				return 0;
			}
		}

		return 1;
	}
	else {
		write("The identifier must start with letter.", file);
		exit(0);
		return 0;
	}

}

int isKeyword(char keywords[], char targetWord[], char currentWord[]) {
	char *ptr = keywords;


	while (1) {
		if (*ptr == '\0') return 0;



		char* cwPtr = currentWord;

		while (*ptr != 32 && *ptr !='\0') {
			*cwPtr = *ptr;
			cwPtr++;
			ptr++;
		}
		*cwPtr = '\0';
		ptr++;

		int match = 1;

		if (strlen(targetWord) != strlen(currentWord)) { 
			continue; 
		}
		else {
			for (int i = 0; i < strlen(targetWord); i++) {
				if (targetWord[i] != currentWord[i])
				{
					match = 0; 
					break;
				}
			}
		}

		if (match) {
			return 1;
		}


	}
}
