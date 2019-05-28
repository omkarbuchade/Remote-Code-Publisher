# Remote-Code-Publisher
Remote code publisher that converts files to web pages on the server through the client GUI using an asynchronous Message-passing Communication channel


Remote code publisher converts C++ source code files on the server to web pages through the client GUI using an asynchronous Message-passing Communication channel

The client GUI is developed using WPF in C# and the server is developed in C++ which is capable of handling multiple concurrent clients using C++ threads

A translator/interface using C++ CLR/CLI is used for the communication between WPF Client in C# and C++ Server

The client GUI provides the functionality to navigate directories and provide a pattern and/or regex to find files for conversion. The converted files are stored on the server and the user can download one or more converted files to his local machine

Project walkthrough: https://www.youtube.com/watch?v=-MwXWKrLorU&t=73s