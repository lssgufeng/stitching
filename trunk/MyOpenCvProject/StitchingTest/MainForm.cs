using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace ImageStitcher
{
    public partial class MainForm : Form
    {
        String image1Path="";
        String image2Path="";
        delegate void myDelegate(bool visible);
        public MainForm()
        {
            InitializeComponent();
            //Directory to store output images
            if (!Directory.Exists("output"))
            {
                Directory.CreateDirectory("output");
            }
        }

        private void bottonBrowse1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            DialogResult result= openFileDialog.ShowDialog(this);
            if(result==DialogResult.OK){
                this.image1Path = openFileDialog.FileName;
                this.textBoxImage1.Text = this.image1Path;
            }
        }

        private void buttonBrowse2_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            DialogResult result = openFileDialog.ShowDialog(this);
            if (result == DialogResult.OK)
            {
                this.image2Path = openFileDialog.FileName;
                this.textBoxImage2.Text = this.image2Path;
            }
        }

        private void buttonStitch_Click(object sender, EventArgs e)
        {
            if (this.image1Path.Length == 0 || this.image2Path.Length == 0)
            {
                MessageBox.Show("Please select image file");
                return;
            }
            this.labelProgress.Invoke(new myDelegate(setVisible), new object[] { true });
            StitcherWrapper.Stitch(this.image1Path, this.image2Path);
            //UInt16[,] image1 = new UInt16[3, 5] { { 12, 56, 12, 97, 345 }, { 12, 145, 234, 987, 535 }, { 124, 1234, 675, 986, 331 } };
            //UInt16[,] image2 = new UInt16[3, 5] { { 122, 156, 142, 1297, 3450 }, { 112, 145, 2234, 9287, 535 }, { 1024, 1234, 6735, 9816, 3431 } };
            //StitcherWrapper.Stitch(image1, image2);
            this.labelProgress.Invoke(new myDelegate(setVisible), new object[] { false });
        }

        private void setVisible(bool visible)
        {
            this.labelProgress.Visible = visible;
            this.Refresh();
        }
    }
}
