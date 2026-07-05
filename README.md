# 🖼️ Image Steganography using C

A command-line **Image Steganography** application developed in **C** that securely hides secret messages inside a **24-bit BMP image** using the **Least Significant Bit (LSB)** algorithm. The application also supports extracting the hidden message from the encoded image while preserving the original image quality.

---

## 📖 Overview

Steganography is the technique of concealing information within another medium so that the existence of the hidden data is not noticeable. This project implements image steganography by embedding secret text into the least significant bits of image pixels.

The project demonstrates concepts of **file handling**, **bit manipulation**, **command-line programming**, and **modular software design** in C.

---

## ✨ Features

- Encode secret text into a BMP image
- Decode hidden text from a stego image
- Uses Least Significant Bit (LSB) algorithm
- Preserves the visual quality of the original image
- Supports custom output image names
- Validates image capacity before encoding
- Modular implementation using separate encoding and decoding modules
- Command-line based interface

---

## 🛠️ Technologies Used

- C Programming
- GCC Compiler
- Linux
- File Handling
- Bit Manipulation
- Structures
- Command Line Arguments

---

## 📂 Project Structure

```
Image-Steganography/
│
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── test_encode.c
├── beautiful.bmp
├── secret.txt
├── stego.bmp
├── output.txt
└── README.md
```

---

## ⚙️ Working Principle

### Encoding

1. Read the source BMP image.
2. Read the secret text file.
3. Verify image capacity.
4. Embed:
   - Magic String
   - Secret file extension
   - Secret file size
   - Secret data
5. Generate the stego image.

### Decoding

1. Read the stego image.
2. Verify the magic string.
3. Extract metadata.
4. Recover the hidden message.
5. Store the decoded message in the output file.

---

## 🚀 Compilation

```bash
gcc test_encode.c encode.c decode.c -o stego
```

---

## ▶️ Usage

### Encode a Secret File

```bash
./stego -e beautiful.bmp secret.txt stego.bmp
```

### Decode the Secret File

```bash
./stego -d stego.bmp output.txt
```

---

## 📸 Example

### Encoding

```bash
$ ./stego -e beautiful.bmp secret.txt stego.bmp

Encoding Successful!
Stego Image Created : stego.bmp
```

### Decoding

```bash
$ ./stego -d stego.bmp output.txt

Decoding Successful!
Secret Message Stored in output.txt
```

---

## 🧠 Concepts Implemented

- Least Significant Bit (LSB) Encoding
- Bitwise Operations
- File I/O
- Binary Data Processing
- Image Capacity Validation
- Modular Programming
- Command-Line Argument Parsing

---

## 🎯 Learning Outcomes

- Developed a real-world application using C.
- Learned secure data hiding using image steganography.
- Improved understanding of bit manipulation techniques.
- Gained experience with binary file handling.
- Practiced modular software development and debugging.

---

## 🔮 Future Enhancements

- Password-protected encoding and decoding
- Encryption before embedding data
- Support for PNG images
- Hide multiple files
- GUI implementation
- Audio and video steganography support

---

## 👨‍💻 Author

**Ashutosh G**

Embedded Systems | Linux | Advanced C | C++ | Microcontrollers

---

## 📄 License

This project is developed for educational and learning purposes.
