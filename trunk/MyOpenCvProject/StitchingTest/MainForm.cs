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
            long ticks=System.DateTime.Now.Ticks;
            StitcherWrapper.Stitch(this.image1Path, this.image2Path);
            long spentTicks = System.DateTime.Now.Ticks - ticks;
            int seconds=TimeSpan.FromTicks(spentTicks).Seconds;
            this.labelTimeTaken.Text = "Time Taken: " + seconds + " Seconds";
            //UInt16[] image1 = new UInt16[5]  { 12, 56, 12, 97, 345 };
            //UInt16[] image2 = new UInt16[6] { 122, 156, 142, 1297, 3450,2345};
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
