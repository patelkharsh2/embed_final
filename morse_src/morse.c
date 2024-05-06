#include <stdio.h>
#include <string.h>
#include "blink.h"
#include "morse.h"
// Function to convert a character to Morse code
const char *char_to_morse(char c)
{
    // Define Morse code for each alphabet and digit
    static const char *morseCode[36] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
                                        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
                                        "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---",
                                        "...--", "....-", ".....", "-....", "--...", "---..", "----."};

    if ((c >= 'a' && c <= 'z'))
    {
        return morseCode[c - 'a'];
    }
    else if ((c >= 'A' && c <= 'Z'))
    {
        return morseCode[c - 'A'];
    }
    else if (c >= '0' && c <= '9')
    {
        return morseCode[c - '0' + 26];
    }
    else
    {
        return ""; // Return empty string for other characters
    }
}

// Function to convert a word to Morse code
// Use the delimiter / for any spaces between words
void translate_to_morse(const char *text, char *morseOutput, int morseOutputSize)
{

    // size of text array
    int len = strlen(text);

    // intialize to an empty array
    morseOutput[0] = '\0'; 

    // Loop through the text to convert each
    // character to morse code.
    for (int i = 0; i < len; i++)
    {
        // make sure its not binary and ASCII
        if (text[i] >= 32 && text[i] <= 126) 
        {
            //for space between words
            if (text[i] == ' ') 
            {
                strcat(morseOutput, "/ ");
            }

            //regular conversion between letters
            else if (i!=len-1)
            {
                const char *morseChar = char_to_morse(text[i]);
                strcat(morseOutput, morseChar);
                strcat(morseOutput, " ");
            }
            else
            {
                const char *morseChar = char_to_morse(text[i]);
                strcat(morseOutput, morseChar);
                
            }
        }

        //return empty string if its not ASCII
        else
        {
            strcat(morseOutput, "");
        }
    }
   
    
}

void morse_blink_led(const char *morseCode)
{
    // Blink LED based on the timing needed
    // 1 Unit = 1 Second
    // Use LED_HIGH() to turn on led
    // Use LED_LOW() to turn off led

    // Getting the length of the morse code
    int len = strlen(morseCode);

    // Printing the morse code for debugging purposes
    printf("%s\n", morseCode);

    // Initialize the LED/WiringX Libaray
    INIT_LED();

    //Loop Through each part of the morse code and turn
    // the LED on or off with respective delays
    for (int i = 0; i < len; i++)
    {
        // dots will be 1 unit
        if (morseCode[i] == '.') 
        {
            LED_HIGH();
            sleep(1);
            //printf("dot\n");
            LED_LOW();

            //if it is the same letter, add the space
            if(morseCode[i+1] != ' ' && morseCode[i+1]!='\0')
            {
                sleep(1);
                //printf("space\n"); // space in the same letter
            }
        }

        // dashes will be 3 units
        else if (morseCode[i] == '-') 
        {
            LED_HIGH();
            sleep(3);
            //printf("dash\n");
            LED_LOW();

            //if it is the right letter, add the space
            if(morseCode[i+1] != ' ' && morseCode[i+1]!='\0')
            {
                sleep(1);
                //printf("space\n"); // space in the same letter
            }
        }

        // space between letters will be 3 units
        else if (morseCode[i] == ' ') 
        {
            sleep(3);
            //printf("lspace\n"); // Letter Space
        }

        // space between words will be 7 units (3 unit for space x2 + 1 unit)
        else if (morseCode[i] == '/') 
        {
            sleep(1);
            //printf("wspace\n"); //Word space
        }

        // Keep LED off if there is an unhandled case
        else
        {
            LED_LOW();
        }

        
    }
}
