# Simple Command-Line Line Editor in C

A modular, terminal-based text editor written in standard C for the 3rd Semester Portfolio Building Studio Course Coding Competition. The editor holds document lines dynamically in memory using a singly linked list and processes line-based commands interactively from standard input.

---

## Team Information
- **Team Member 1:** [Your Name] ([USN / Roll No])
- **Team Member 2:** [Teammate Name] ([USN / Roll No])
- **Team Member 3:** [Teammate Name] ([USN / Roll No])
- **Course:** Portfolio Building Studio Course – 3rd Semester

---

## Features Implemented

### Core Features
- [x] **Insert Line:** Insert a new line of text at any valid 1-based line number (`i <line> <text>`) or append at the end (`a <text>`), shifting subsequent lines automatically.
- [x] **Delete Line:** Remove a line at a given 1-based index (`d <line>`), bypassing and freeing the node and shifting following lines up.
- [x] **Display Document:** Formatted output of all lines with aligned 4-digit line numbers (`p`).
- [x] **Save & Load File:** Persist document buffer to a `.txt` file (`s <filename>`) and load existing text files into memory with buffer reset (`l <filename>`).

### Bonus Features
- [x] **Search Substring:** Query lines containing a specific keyword or phrase and display matching line indices (`f <query>`)[cite: 1].
- [x] **Document Statistics:** Real-time metrics reporting total lines, word count, and character counts (`stat`)[cite: 1].

---

## Data Structure & Architectural Justification

The editor models the document in memory as a **Singly Linked List** (`Document` managing dynamic `LineNode` structures)[cite: 1]:

```text
[Document]
  head --------> [ LineNode 1 ]           [ LineNode 2 ]
  line_count: 2  | text: "Line one"| ---> | text: "Line two"| ---> NULL
                 | next: ----------+      | next: NULL      |
Why a Linked List over an Array?$O(1)$ Modification Overhead: Inserting or deleting lines in an array requires contiguous memory shifting of up to $O(N)$ elements[cite: 1]. With a linked list, once traversed to the target position, operations require only $O(1)$ pointer rewiring[cite: 1].Dynamic Heap Sizing: Lines are allocated via malloc based on actual line length (strlen + 1), avoiding fixed buffer limits or wasted RAM on sparse allocations[cite: 1].Graceful Memory Reclamation: Deletions and file resets systematically invoke free() on both line strings and container nodes to eliminate memory leaks[cite: 1].
.
├── editor.c           # Complete C source code
├── HELP.md            # Command syntax guide and usage examples[cite: 1]
├── README.md          # Project overview, build instructions, and team metadata[cite: 1]
└── paper_design.jpg   # Scanned copy/photo of paper architecture and hand-written code[cite: 1]
Quick Command Reference
Command	Syntax	Description	Example
Print	p	Display document with line numbers	p
Insert	i <line> <text>	Insert text at 1-based index	i 2 Hello World
Append	a <text>	Append text at document tail	a End note
Delete	d <line>	Delete line at index	d 2
Save	s <filename>	Save buffer to disk	s notes.txt
Load	l <filename>	Load file contents to buffer	l notes.txt
Find	f <phrase>	Search lines for phrase	f syntax
Stats	stat	Show line, word, and character totals	stat
Help	h	Display interactive command help	h
Quit	q	Free memory and exit	q
Error Handling & Robustness
Invalid Line Ranges: Out-of-bounds inputs (e.g., negative indices, index greater than line_count + 1) produce graceful warning notices without segmentation faults[cite: 1].

Empty Document State: Display, search, and delete operations safely guard against NULL pointers when the list is unpopulated[cite: 1].

File System Failures: Checks fopen return pointers to prevent crashes if a target file does not exist or lacks read/write permissions[cite: 1].