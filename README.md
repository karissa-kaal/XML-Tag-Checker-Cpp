# XML-Tag-Checker-Cpp

The XML Tag Checker is a C++ program that allows you to check the correctness of XML tags in an XML file. It uses a stack-based approach to validate the opening and closing tags in the file and reports any errors encountered.

## Usage

To use the XML Tag Checker, follow these steps:

1. Compile and run the program in a C++ compiler.
2. When prompted, enter the filename of the XML file you want to check. Make sure to enter the filename in the proper format, e.g., `file.xml`.
3. The program will open the file and start checking the tags.
4. It will report any errors encountered, such as missing closing tags or incorrect tag formatting.
5. Once the check is complete, the program will display a message indicating whether the file has been checked successfully.

## Example

Here's an example to demonstrate the usage of the XML Tag Checker:

```cpp
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

// ... (rest of the code)

int main() {
	Stack<string> S1;

	FileCheck(S1);

	return 0;
}
```

## Conclusion

The XML Tag Checker provides a basic mechanism to validate XML tags in an XML file. It can be used as a starting point to build more complex XML processing systems or as a standalone tool to ensure XML file integrity. If you have any further questions or feedback, please don't hesitate to reach out. Happy XML checking!
