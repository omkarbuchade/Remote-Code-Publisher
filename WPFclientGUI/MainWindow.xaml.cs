///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Client GUI for Source Code Publisher         //
// ver 1.1                                                           //
// Omkar Buchade, CSE687 - Object Oriented Design, Spring 2019       //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based client GUI for Source Code Publisher.  
 * It's responsibilities are to:
 * - Provide a display of directories and its contents located on a remote ServerServer.
 * - It provides a subdirectory list and a filelist for the selected directory on Server.
 * - You can provide a pattern, regex and download destination to convert and store them 
 * - at the client end.
 * - You can navigate into subdirectories by double-clicking on subdirectory or the parent
 * - directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.1 : 30 April 2019
 * - Added functionality to send and receive messages from server
 * - Added the download file at client machine functionality.
 * - Fixed bugs- Previously converted files did not presist.
 * - User now has the option to open the converted file in text editor too.
 * - 
 * ver 1.0 : 8 April 2019
 * - first release
 * - Several early prototypes. Those are all superceded by this package.
 */


using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;
using System.IO;
using Path = System.IO.Path;
using System.Threading;
using MsgPassingCommunication;
using System.Threading.Tasks;

namespace WPFclientGUI
{
        public partial class MainWindow : Window
    {
        //Variables to bind the Working Directory, pattern and rege given on the Client GUI
        string Pattern { get; set; }
        string Regexes { get; set; }

        private List<string> FilesMatched = new List<string>();
        private List<String> arguments = new List<string>();
        private List<string> FilestoDisplay = new List<string>();

          public MainWindow()
        {
            InitializeComponent();
        }
        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private CsEndPoint serverEndPoint;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();

        private void init()
        {
            //Creating a Server and client endpoint and assigning them address and port number
            endPoint_ = new CsEndPoint();
            string[] args = Environment.GetCommandLineArgs();
            int selfPort = int.Parse(args[1]);
            string[] serverUrl = args[2].Split(':');
            string ServerURL = serverUrl[0];
            int ServerPort = int.Parse(serverUrl[1]);
            endPoint_.machineAddress = "localhost";
            endPoint_.port = selfPort;
            serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = ServerURL;
            serverEndPoint.port = ServerPort;
            translater = new Translater();
            translater.listen(endPoint_);

            var Ddirectory = "../../../../SaveFiles/";
            Ddirectory = Path.GetFullPath(Ddirectory);
            RegexTextBox.Text = "[A-F](.*)";
            DownloadTextBox.Text = Ddirectory;
            PatternTextBox.Text = "*.h *.cpp";

            var path = args[3];
            pathStack_.Push(path);
            var dispPath=removeFirstDir(removeFirstDir(path));
            PathTextBox.Text = dispPath;
        }

        private async void Window_Loaded(object sender, RoutedEventArgs e)
        {
            init();
            // start processing messages
            processMessages();
            // load dispatcher
            loadDispatcher();
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
            addDownloadFile();
            msg.remove("command");
            msg.remove("path");
            msg.add("path", "../ConvertedPages");
            msg.add("command", "getPrevConvFiles");
            translater.postMessage(msg);
            var retVal = await AutomatedTest();
        }
        private async Task<int> AutomatedTest()
        {
            await Task.Delay(1);
            MessageBoxResult MsgBoxRes = MessageBox.Show("Do you want to run an automated test?", "Automated Test", MessageBoxButton.YesNo);
            if (MsgBoxRes == MessageBoxResult.No)
                return 0;
            PublishButton_Click(null, null);
            return 1;
        }
        //----< Browse button to navigate to target folder for downloading files >------------------
        private void BrowseButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog dialog;
            dialog = new System.Windows.Forms.FolderBrowserDialog();

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                DownloadTextBox.Text = dialog.SelectedPath;
            }
        }
        //----< sends a post message to the translator ; Message consists of WorkingDir, Pattern, Regex >------------------
        private void PublishButton_Click(object sender, RoutedEventArgs e)
        {
            if (PatternTextBox.Text == "")
                MessageBox.Show("Please enter pattern", "Error");
            else
            {
                statusBar.Text = "Buzy converting files!";
                statusBarDisplay.Text = "Buzy converting files!";
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("path", pathStack_.Peek());
                if (PatternTextBox.Text != "")
                {
                    var patterns = PatternTextBox.Text.Split();
                    for (int i = 1; i <= patterns.Length; i++)
                    {
                        if(!(patterns[i - 1] == "*.h" || patterns[i - 1]=="*.cpp"))
                        {
                            MessageBox.Show("Please enter a valid pattern. Valid patterns are *.h and *.cpp", "Pattern error");
                            return;
                        }
                        else
                            msg.add("pattern" + i.ToString(), patterns[i - 1]);
                    }
                }
                if (RegexTextBox.Text != "")
                {
                    var regexs = RegexTextBox.Text.Split();
                    for (int i = 1; i <= regexs.Length; i++)
                        msg.add("regex" + i.ToString(), regexs[i - 1]);
                }
                if (subdirsCheckBox.IsChecked.Value)
                    msg.add("subDirs", "/s");
                else
                    msg.add("subDirs", "");
                msg.add("command", "publish");
                translater.postMessage(msg);
                msg.remove("command");
                msg.remove("path");
                msg.add("path", "../ConvertedPages");
                msg.add("command", "getPrevConvFiles");
                translater.postMessage(msg);
            }
        }
        //----<Used to getDirs and GetFiles in a Directory on the server; Achieved by sending a post message to translator>------------------
        
        //----< responsible for receiving messages from the server using the getMessage method from translator >------------------
        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }

        //----< function dispatched by child thread to main thread >-------
        private void clearDirs()
        {
            listBoxDir.Items.Clear();
        }

        //----< function dispatched by child thread to main thread >-------
        private void addDir(string dir)
        {
            listBoxDir.Items.Add(dir);
        }
        //----< function dispatched by child thread to main thread >-------
        private void insertParent()
        {
            listBoxDir.Items.Insert(0, "..");
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearFiles()
        {
            listBoxFiles.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------
        private void clearConvertedFiles()
        {
            statusBar.Text = "Ready!";
            statusBarDisplay.Text = "File conversion complete!";
            ListBoxCurrentlyConvertedFiles.Items.Clear();
            DisplayTab.IsSelected = true;
        }
        //----< function dispatched by child thread to main thread >-------
        private void clearDownloadedFiles()
        {
            ListBoxDownload.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------
        private void addFile(string file)
        {
            listBoxFiles.Items.Add(file);
        }
        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load getDirs processing into dispatcher dictionary >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }
        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }

        private void DispatcherLoadConvFiles()
        {
            Action<CsMessage> getPrevConvFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearPrevConvFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            AddPrevConvFiles(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getPrevConvFiles", getPrevConvFiles);
        }

        private void clearPrevConvFiles()
        {
            ListBoxConvertedFiles.Items.Clear();
        }

        private void AddPrevConvFiles(string file)
        {
            ListBoxConvertedFiles.Items.Add(file);
        }
        //----< load publish processing into dispatcher dictionary >-------
        private void DispatcherPublishFiles()
        {
            Action<CsMessage> publish = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearConvertedFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFileDisplay(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                    if (key.Contains("error"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            NoFilesMatched();
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }

            };
            addClientProc("publish", publish);
        }

        private void NoFilesMatched()
        {
            statusBar.Text = "Ready!";
            MessageBox.Show("No files matched to given pattern/regex", "No files converted");
        }

        //----< load downloadFiles processing into dispatcher dictionary >-------
        private void DispatcherDownloadFiles()
        {
            Action<CsMessage> download = (CsMessage rcvMsg) =>
            {
                //Dispatcher.Invoke(dwnldFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                         {
                            addDownloadFile();
                         };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("download", download);
        }

        //----< function dispatched by child thread to main thread >-------
        private void addFileDisplay(string file)
        {
            var name = Path.GetFileName(file) + ".html";
            ListBoxCurrentlyConvertedFiles.Items.Add(name);
        }

        private void addPrevFileDisplay(string file)
        {
            var name = Path.GetFileName(file) + ".html";
            ListBoxConvertedFiles.Items.Add(name);
        }

        //----< function dispatched by child thread to main thread >-------
        private void addDownloadFile()
        {
            ListBoxDownload.Items.Clear();
            var files = Directory.GetFiles(DownloadTextBox.Text);
            foreach (var file in files)
            {
                ListBoxDownload.Items.Add(Path.GetFileName(file));
            }
        }

        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherPublishFiles();
            DispatcherDownloadFiles();
            DispatcherLoadConvFiles();
        }
        //----< start Comm, fill window display with dirs and files >------

        //----< strip off name of first part of path >---------------------
        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);

            pos = modifiedPath.IndexOf("/");
            string modifiedPath1 = modifiedPath.Substring(pos + 1, modifiedPath.Length - pos - 1);

            return modifiedPath1;
        }
        //----< respond to mouse double-click on dir name >----------------
        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            statusBar.Text = "Ready!";
            statusBarDisplay.Text = "Ready!";
            // build path for selected dir
            string selectedDir = (string)listBoxDir.SelectedItem;
            if (selectedDir == null)
                return;
            string path;
            if (selectedDir == "..")
            {
                if (pathStack_.Count > 1)  // don't pop off "Storage"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedDir;
                pathStack_.Push(path);
            }
            // display path in Dir TextBlock
            PathTextBox.Text = removeFirstDir(pathStack_.Peek());

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
        }
        //----< respond to mouse double-click on converted file name; downloads file to client's machine >----------------
        private void ListBoxConvertedFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedFile = (string)ListBoxConvertedFiles.SelectedItem;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("name", selectedFile);
            msg.add("command", "download");
            msg.add("Downloadpath", DownloadTextBox.Text);
            Console.Write("\nRequirement 5: This project provides message designs appropriate for this application. This message is used for downloading files.");
            translater.postMessage(msg);
            addDownloadFile();
        }
        //----< respond to mouse double-click on downloaded files; Opends the file in browser/text editor for viewing >---------------
        private void ListBoxDownload_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (ListBoxDownload.SelectedIndex == -1) return;

            string HtmlFile = ListBoxDownload.SelectedItem.ToString();
            HtmlFile = Path.GetFullPath(DownloadTextBox.Text + HtmlFile);
            Console.Write("\nOpening file: " + HtmlFile);

            if(BrowserCheckBox.IsChecked.Value)
            {
                try
                {
                    System.Diagnostics.Process myProcess = System.Diagnostics.Process.Start(HtmlFile);
                }
                catch (Exception)
                {
                    Console.Write("Error opening the file: " + HtmlFile);
                    MessageBox.Show("Error opening the file: "+HtmlFile, "File open error");
                }
            }

            if (TextEditorCheckBox.IsChecked.Value)
            {
                try
                {
                    System.Diagnostics.Process myProcess = System.Diagnostics.Process.Start("notepad.exe",HtmlFile);
                }
                catch (Exception)
                {
                    Console.Write("Error opening the file: " + HtmlFile);
                    MessageBox.Show("Error opening the file: " + HtmlFile, "File open error");
                }
            }
        }

        private void ListBoxCurrentlyConvertedFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Console.Write("\nRequirement 6: This project supports transferring one or more converted files back to the local client, using the communication channel.");
            string selectedFile = (string)ListBoxCurrentlyConvertedFiles.SelectedItem;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("name", selectedFile);
            msg.add("command", "download");
            msg.add("Downloadpath", DownloadTextBox.Text);
            translater.postMessage(msg);
            addDownloadFile();
        }

        private void DownloadTextBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        {
            addDownloadFile();
        }
    }
}
