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
            int direction = -1;
            int method = -1;
            bool crop=true;
            if (this.radioButtonHorizontal.Checked)
                direction = 0;
            else if (this.radioButtonVertical.Checked)
                direction = 1;

            if (this.radioButtonKnn.Checked)
                method = 0;
            else if (this.radioButtonFlann.Checked)
                method = 1;

            if (this.checkBoxRetainImageParts.Checked)
            {
                crop = false;
            }


            StitcherWrapper.Stitch(this.image1Path, this.image2Path,direction,method,crop);
            long spentTicks = System.DateTime.Now.Ticks - ticks;
            int seconds = TimeSpan.FromTicks(spentTicks).Seconds;
            this.labelTimeTaken.Text = "Time Taken: " + seconds + " Seconds";
            UInt16[] image1 = new UInt16[6]  { 12, 56, 12, 97, 345,9856 };
            UInt16[] image2 = new UInt16[8] { 789, 2345, 142, 1297, 3450,2345,764,123};
            //StitcherWrapper.StitchRaw(image1, image2);
            this.labelProgress.Invoke(new myDelegate(setVisible), new object[] { false });
            string directory = Directory.GetCurrentDirectory() + "\\output";
            System.Diagnostics.Process.Start("explorer.exe",directory);
        }

        private void setVisible(bool visible)
        {
            this.labelProgress.Visible = visible;
            this.Refresh();
        }
    }
}
