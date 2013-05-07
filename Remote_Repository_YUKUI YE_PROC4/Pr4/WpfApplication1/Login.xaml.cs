/////////////////////////////////////////////////////////////////////
// LoginWindow : CheckUserName and Password Validation             //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////

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
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Login : Window
    {
        private mockChannel.IChannel chan;

        public Login()
        {
            InitializeComponent();
        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            chan = mockChannel.IChannel.CreateChannel();
            StringBuilder builder = new StringBuilder();
            builder.Append(txtUsername.Text);
            builder.Append("*");
            builder.Append(txtPassword.Text);
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
                new sendXMLMetaData().Show();
                this.Close();
            }
            else if (retmsg == "failed")
            {
                System.Windows.MessageBox.Show("UserAccount Information Invalid");
                return;
            }
        }
    }
}
