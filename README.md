# Project Overview
This project is a custom file system developed as part of the Operating System course. The main objective is to create a basic partition management system in C, with support for reading, writing, and deleting files using a custom data structure and pagination system.

--- 

# Key Features
- File Writing: Write data to files stored in the partition.
- File Reading: Read data from files stored in the partition.
- File Deletion: Remove files from the partition and mark their pages as available.
- Partition Management: Manipulate the partition, format, and manage file metadata.

---

# Project Structure
- /include: Contains header files with function declarations and data structure definitions.
- /src: Contains the source code for partition manipulation and file operations.

---

# Prerequisites
- Cygwin with e2fsprogs for formatting ext4 partitions on Windows.
- Makefile to compile the project.

---

> [!NOTE]
> The project supports up to 2048 blocks (204800 bytes) with a reserved space of 10240 bytes for metadata.
> File data is stored in linked pages, with each page containing 256 bytes minus a validity byte for availability checking.
