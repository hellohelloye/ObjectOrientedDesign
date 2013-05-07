///////////////////////////////////////////////////////////////////////
// sendXMLMetaDataWindow:sendInformationAboutTheFileYouAreGoingTosent//
//                       Check whether user is the file's owner      //
// Language:    Visual C++ 2012                                      //
// Platform:    MACPRO, Virtual Machine Windows 8                    //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013             //
// Author:      YUKUI YE,            Syracuse University             //
//              (315) 751-0156, yye@syr.edu                          //
///////////////////////////////////////////////////////////////////////

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
using System.Windows.Shapes;
using System.Windows.Threading;
using System.IO;
using System.Windows.Forms;
using System.Threading;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for sendXMLMetaData.xaml
    /// </summary>
    public partial class sendXMLMetaData : Window
    {
        private mockChannel.IChannel chan;

        public sendXMLMetaData()
        {
            InitializeComponent();
        }

        private void btnSendXML_Click(object sender, RoutedEventArgs e)
        {
            chan = mockChannel.IChannel.CreateChannel();
            StringBuilder builder = new StringBuilder();
            builder.Append(txtOwnername.Text);
            builder.Append("*");
            builder.Append(txtVersionNumber.Text);
            builder.Append("*");
            builder.Append(txtDependency.Text);
            builder.Append("$");
            builder.Append(txtSendingFileName.Text);
            builder.Append("&");
            builder.Append(txtIP.Text);
            builder.Append("&");
            builder.Append(txtSrcPort.Text);
            builder.Append("&");
            builder.Append(txtRmtPort.Text);
            string msg = builder.ToString();
            chan.postMessage(msg);

            string retmsg = chan.getMessage();
            if (retmsg == null)
                while (chan.getMessage() != null) ;
            else if (retmsg == "succeed")
            {
                this.Hide();
                new MainWindow().Show();
                this.Close();
            }
            else if (retmsg == "failed")
            {
                System.Windows.MessageBox.Show("Invalid Access,UserName is different From file's ownername");
                return;
            }

        }
    }
}
