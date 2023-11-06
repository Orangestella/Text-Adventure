


#include <cstdint>
#include <afxres.h>
#include <iterator>
#include <iostream>


/* Library routines to output text and "word wrap" so that words are not broken across lines. */

uint16_t consoleWidth;
uint16_t currentConsoleOffset;

/**
 * Set up the word wrap library.
 */
void initWordWrap() {
    /* Find the width of the console window. */
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
    /* We subtract 2 because Windows doesn't allow for the right hand scroll bar when reporting
     * width of command prompt windows. */
    consoleWidth = static_cast<uint16_t>(csbi.srWindow.Right - csbi.srWindow.Left - 2);
    currentConsoleOffset = 0;
}

/**
 * Output a string with word wrap.
 * @param text Pointer to the string to output.
 */
void wrapOut(const std::string *text) {
    size_t len = text->length(); // Length of the string
    size_t position = 0; // How much of the string we've printed
    std::ostream_iterator<char> iout(std::cout); // Iterators for the string and display
    std::string::const_iterator strInt = text->begin();
    const char *rawString = text->data();
    while (position < len) {
        // How much space do we have left on the current console line?
        size_t left = (consoleWidth - currentConsoleOffset);
        if ((len - position) < left) { // Can we fit the whole string on this line?
            std::cout << &rawString[position] << " "; // Yes, just do it.
            currentConsoleOffset += len + 1; // Update our position on this line.
            break;
        } else { // We can't fit the whole string on this line.
            size_t lastSpace = left; // This would be the last character we did fit on this line
            // Move this pointer back until we find a space
            while ((rawString[position + lastSpace] != ' ') && (lastSpace > 0)) {
                lastSpace--;
            }
            // Print up to that space using the output iterator
            std::copy(strInt + position, strInt + position + lastSpace, iout);
            // Print an end-of-line, unless the string ended exactly at the end of the line in which case the
            // cursor has automatically moved to the next line
            if (lastSpace < left) {
                std::cout << std::endl;
            }
            position += lastSpace + 1; // Mark how much of the string we've printed
            currentConsoleOffset = 0; // Since we just created a new line, we're now at the start of it
        }
    }
}

/**
 * Ends a paragraph of word wrapped output.
 */
void wrapEndPara() {
    // If we aren't at the end of a line already, end the line
    if (currentConsoleOffset != 0) {
        std::cout << std::endl;
    }
    // And print a blank line
    std::cout << std::endl;
    // Since we just started a new line, we're now at the start of it
    currentConsoleOffset = 0;
}
