/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include "sdl_file_chooser.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string directory = ".";  // Default directory
    std::string customTitle = FILECHOOSER_TITLE;  // Default title

    // Processing command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-m" && i + 1 < argc) {
            customTitle = argv[++i]; // Use the title provided after -m
        } else if (arg == "-d" && i + 1 < argc) {
            directory = argv[++i]; // Uses the directory provided after -d
        }
    }

    // Create a FileChooser instance with processed arguments
    FileChooser fileChooser(directory, customTitle);
    std::string chosenFile = fileChooser.get();
    
    // Display of selected file or message indicating that nothing has been selected
    if (!chosenFile.empty()) {
        std::cout << "Selected: " << chosenFile << '\n';
    } else {
        std::cout << "No file selected or application closed." << '\n';
    }
    
    return 0;
}
