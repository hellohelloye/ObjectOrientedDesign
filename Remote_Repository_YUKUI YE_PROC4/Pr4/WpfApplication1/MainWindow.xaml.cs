﻿/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Demo of techniques to build WPF interface  //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013       //
/////////////////////////////////////////////////////////////////////
/*
 * Note: 
 *   You must build the MockChannel DLL before building this
 *   WPF application.  If you don't the build will fail.
 *   To do that you simply right-click on the MockChannel project
 *   and select Rebuild.
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.IO;
using System.Windows.Forms;
using System.Threading;

namespace WpfApplication1
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private mockChannel.IChannel chan;
    private Action<string> myItemDelegate;

    public MainWindow()
    {
      InitializeComponent();
    }

    private void AddItem(string item)
    {
      string msg = "Received Message: " + item;
      listBox1.Items.Add(msg);
    }

    private void Run()
    {
      while (true)
      {
        string msg = chan.getMessage();
        Dispatcher.Invoke(myItemDelegate, DispatcherPriority.Background, new object[] { msg });
      }
    }

    private void Window_Loaded_1(object sender, RoutedEventArgs e)
    {
      myItemDelegate = this.AddItem;
      textBox1.Text = Directory.GetCurrentDirectory();
      string[] files = Directory.GetFiles(textBox1.Text, "*.*");
      foreach (string file in files)
        listBox1.Items.Add(System.IO.Path.GetFileName(file));
      button3.IsEnabled = false;
      chan = mockChannel.IChannel.CreateChannel();
      Thread clientReceiveThread = new Thread(new ThreadStart(this.Run));
      clientReceiveThread.IsBackground = true;
      clientReceiveThread.Start();
    }

    private void Button_Click_1(object sender, RoutedEventArgs e)
    {
      FolderBrowserDialog fbd = new FolderBrowserDialog();
      fbd.ShowNewFolderButton = false;
      fbd.SelectedPath = textBox1.Text;
      DialogResult result = fbd.ShowDialog();
      if (result == System.Windows.Forms.DialogResult.OK)
      {
        Directory.SetCurrentDirectory(fbd.SelectedPath);
        textBox1.Text = fbd.SelectedPath;
        listBox1.Items.Clear();
        string[] files = Directory.GetFiles(textBox1.Text, "*.*");
        foreach (string file in files)
          listBox1.Items.Add(System.IO.Path.GetFileName(file));
      }
    }

    private void Button_Click_2(object sender, RoutedEventArgs e)
    {
      OpenFileDialog ofd = new OpenFileDialog();
      ofd.InitialDirectory = textBox1.Text;
      ofd.Multiselect = true;
      System.Windows.Forms.DialogResult result = ofd.ShowDialog();
      if (result == System.Windows.Forms.DialogResult.OK)
      {
        listBox1.Items.Clear();
        string[] files = ofd.FileNames;
        foreach (string file in files)
          listBox1.Items.Add(System.IO.Path.GetFileName(file));
      }
      button3.IsEnabled = true;
    }

    private void Button_Click_3(object sender, RoutedEventArgs e)
    {
      int size = listBox1.Items.Count;
      for(int i=0; i<size; ++i)
      {
        string item = listBox1.Items[i] as string;
        chan.postMessage(item);  
        listBox1.Items.Add("    Posted Message: " + item);
      }
      button3.IsEnabled = false;      
    }
  }
}
