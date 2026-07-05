
🖼️ Image Steganography using C

A command-line Image Steganography application developed in C that securely hides secret text inside a 24-bit BMP image using the Least Significant Bit (LSB) technique. The application also supports extracting the hidden message without affecting the visual quality of the image.

📌 Features
Encode secret text into a 24-bit BMP image
Decode hidden text from a stego image
Uses LSB (Least Significant Bit) algorithm
Preserves the visual appearance of the original image
Supports custom output image names
Validates input files before processing
Modular implementation with separate encoding and decoding modules
🛠️ Technologies Used
C Programming
File Handling
Bit Manipulation
Structures
Command Line Arguments
Linux/GCC
📂 Project Structure
.
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── test_encode.c
├── beautiful.bmp      # Input image
├── secret.txt         # Secret message
├── stego.bmp          # Output encoded image
├── output.txt         # Decoded message
└── README.md
⚙️ How It Works
Encoding Process
Read the source BMP image.
Read the secret text file.
Verify image capacity.
Encode:
Magic string
File extension
Secret file size
Secret data
Generate the stego image.
Decoding Process
Read the stego image.
Verify the magic string.
Extract metadata.
Recover the hidden message.
Store the decoded text in the output file.
🚀 Compilation
gcc test_encode.c encode.c decode.c -o stego
▶️ Usage
Encode
./stego -e beautiful.bmp secret.txt stego.bmp
Decode
./stego -d stego.bmp output.txt
📸 Example
Input
Image      : beautiful.bmp
Secret File: secret.txt
Encode
./stego -e beautiful.bmp secret.txt stego.bmp
Decode
./stego -d stego.bmp output.txt
Output
Secret message successfully recovered in output.txt
💡 Concepts Used
Least Significant Bit (LSB) Encoding
Binary Data Manipulation
BMP Image Format
File I/O Operations
Memory Management
Command Line Argument Parsing
Modular Programming
🎯 Learning Outcomes
Implemented steganography using bit-level operations.
Gained hands-on experience with file handling in C.
Improved understanding of image storage formats.
Strengthened modular programming and debugging skills.
Learned secure data hiding techniques.
🔮 Future Enhancements
Support PNG images
Encrypt secret message before encoding
Password-protected decoding
Hide multiple files
GUI version using Qt or GTK
Support for audio and video steganography

👨‍💻 Author

Ashutosh G

Embedded Systems | Linux | Advanced C | C++ | Microcontrollers
